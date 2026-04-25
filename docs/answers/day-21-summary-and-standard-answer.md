# Day 21 - 学习总结与标准答案

## 今日是否完成

Day 21 可以视为完成。

完成标准是已经实现教学版分槽分配器：

- `mini_malloc` 支持小对象（group/slot）和大对象（mmap）两条路径
- `mini_free` 能正确释放 slot（bitmap）和大对象（munmap）
- 测试覆盖了小对象分配/复用、bitmap 正确性、大对象路径

## 今日总结

今天的重点是实现教学版分槽分配器，把 Day 20 的设计落成代码。

在实现方面：
- `size_to_class`：把用户 size 映射到 8 个 size class
- `group` 结构：`size_class`、`slot_count`、`free_count`、`bitmap[32]`
- `alloc_from_group`：从 bitmap 找第一个空闲 slot
- `mini_malloc`：小对象走 group/slot，大对象直接 mmap
- `mini_free`：小对象标记 bitmap，大对象 munmap

关键收获：
1. slot 无 header，空间利用率 100%——全靠 group 的 bitmap 和地址偏移
2. bitmap 用 `unsigned char[32]` 管理 256 个 slot，每 bit 管一个 slot
3. `mini_free` 通过 `(ptr - base) / size_class` 算出 slot 索引，不需要元数据
4. 大对象需要在 mmap 时多分配 8 字节存长度，free 时才能正确 munmap

踩的坑：
- 大对象 munmap 传了 length=0，永远失败
- 修复：malloc 时多分配 sizeof(size_t) 存长度，free 时取出长度传给 munmap

设计局限：
- `bitmap_find_free` 是 O(n) 逐位扫描，不是真正的 O(1)
- `mini_free` 要遍历所有 group，O(n)
- group 永远不释放（munmap），内存只进不出
- 没有线程安全（无锁）

## 标准答案

### 1. 这套设计相比 free list，真正减少了什么成本？

标准答案：

- **查找成本**：free_list 遍历 O(n) → bitmap 扫描 O(1)（实际是 O(slot_count)，但常数小）
- **空间成本**：每个块 32 字节 header → slot 无 header，空间利用率 100%
- **碎片成本**：free_list 外部碎片严重 → 同 size class slot 无外部碎片
- **元数据安全**：header 紧邻用户数据 → bitmap 和 group 分离，用户越界写不破坏元数据

### 2. 如果要支持 realloc，先考虑哪条路径？

标准答案：

realloc 的核心是"原地扩展"或"分配新块+复制"：

**小对象路径：**
- 如果新 size 在同一个 size class → 原地返回，不做事
- 如果新 size 在不同的 size class → 分配新 slot，复制数据，释放旧 slot

**大对象路径：**
- 尝试 mremap（Linux 特有）原地扩展
- 失败则 mmap 新块 + memcpy + munmap 旧块

优先考虑小对象路径，因为大多数 realloc 是小对象（字符串拼接等场景）。

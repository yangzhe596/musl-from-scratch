# Day 14 - 学习总结与标准答案

## 今日是否完成

Day 14 可以视为完成。

完成标准是已经完成分配器设计、mmap 封装和块头结构：

- 理解了 mmap 匿名内存分配的机制
- 设计了 block_header 结构（size、free、next）
- 实现了 16 字节对齐
- 画出了内存块布局图

## 今日总结

今天的重点是理解动态分配器需要解决的最小问题集合，以及 mmap vs brk 的选择。

在设计方面，我们确定了：
- 块头结构：`size`（用户数据大小）、`free`（空闲标志）、`next`（链表指针）
- 内存布局：`[block_header][user data]`，用户指针 = 块头地址 + HEADER_SIZE
- 对齐策略：16 字节对齐，HEADER_SIZE 也必须是 16 的倍数

在 mmap vs brk 方面，我们理解了：
- brk 是连续的"推土机"，中间空了没法回收
- mmap 是独立的"快递包裹"，可以 munmap 精确释放
- mmap 用 `MAP_ANONYMOUS` + `fd=-1` 做匿名内存分配
- mmap 是 lazy allocation，不立刻分配物理内存

关键收获：
1. `ALIGN_UP` 宏的本质：先加 alignment-1 再清低 4 位，等价于向上对齐
2. `free_list` 是只增不减的链表，块永远在里面，只是 free 标志切换
3. 虚拟内存和物理内存的区别：VSZ 包含 mmap 未分配部分，RSS 只算实际物理页

踩的坑：
- `sizeof(block_header)` 是 24 字节，不是 16 的倍数，导致用户数据不对齐
- 修复：`HEADER_SIZE = ALIGN_UP(sizeof(struct block_header))`

## 标准答案

### 1. malloc 至少要管理哪些元数据？

标准答案：

最少需要一个字段：**size**。

有了 size，才能：
- 知道用户数据区有多大
- 从用户指针倒推块头位置（`ptr - HEADER_SIZE`）
- 计算下一个块的位置（`ptr + size`）

`free` 和 `next` 是优化：
- `free` 标记块是否可复用，避免每次都 mmap
- `next` 串成链表，方便遍历查找

### 2. 为什么 malloc 返回的不能是块头地址？

标准答案：

如果用户拿到块头指针：
- 用户可能修改 `size`、`free`、`next` 字段，破坏分配器元数据
- 分配器无法区分"用户指针"和"块头指针"
- `free` 时不知道该从哪里开始释放

所以必须隐藏块头，只暴露用户数据区的指针。

### 3. brk 为什么无法还给 OS？

标准答案：

brk 维护一个连续的堆区域，brk 指针标记堆的末尾。

释放中间的块时：
- 空闲了，但 brk 指针无法后退（后面还有其他块）
- 那块内存变成"空洞"，只能等后面的块也释放了才能收缩

mmap 每次分配独立区域，munmap 可以精确释放任意一块，不受其他块影响。

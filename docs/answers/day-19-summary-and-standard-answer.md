# Day 19 - 学习总结与标准答案

## 今日是否完成

Day 19 可以视为完成。

完成标准是已经理解 mallocng 的核心术语和设计动机：

- 理解了 meta、group、slot、size class 的含义
- 能解释阶段四的痛点和阶段五的改进思路
- 画出了 group 管理多个 slot 的示意图

## 今日总结

今天的重点是理解 mallocng 的设计动机和核心概念。

在概念理解方面：
- **size class**：预定义的固定大小（16、32、48、64...），malloc 请求向上取整到最近的 size class
- **group**：一大块内存，切成等大的 slot，由一个 meta 管理
- **slot**：固定大小的内存单元，没有单独的 header
- **meta**：group 的元数据，记录 size_class、slot_count、free_bitmap 等

在阶段四痛点对照方面：
1. **查找慢**：free_list O(n) → mallocng bitmap O(1)
2. **header 开销**：每个块 32 字节 → mallocng slot 无单独 header
3. **碎片化**：块大小不一 → mallocng 同尺寸 slot 无外部碎片
4. **元数据安全**：header 紧邻用户数据 → mallocng meta 集中存储，和 slot 分离

关键收获：
1. 现代 allocator 不只是"找空闲块"，而是批量化和分级管理
2. 同尺寸对象分组 → bitmap 查找 O(1)，无单独 header 开销
3. 元数据和用户数据分离 → 用户越界写不会直接破坏 meta
4. 元数据集中管理 → 便于加 magic number 或 checksum 校验

## 标准答案

### 1. 为什么同尺寸对象放在同一组更高效？

标准答案：

- **查找快**：用 free_bitmap 位图找空闲 slot，O(1) 复杂度，不需要遍历链表
- **无 header 开销**：slot 大小固定，meta 记录了 size_class，不需要每个 slot 都有 header
- **无外部碎片**：所有 slot 等大，释放后直接标记 bitmap 位，不存在"碎片"概念
- **缓存友好**：同尺寸 slot 连续存储，访问局部性好

### 2. 为什么 metadata 组织方式影响鲁棒性？

标准答案：

三个因素：

1. **元数据和用户数据分离**：阶段四 header 紧邻用户数据，用户越界写直接破坏 header；mallocng meta 集中存储，和 slot 分开，用户越界写不会直接破坏 meta

2. **位图 vs 链表**：阶段四 free_list 链表的 next 指针可被用户覆盖，一个指针损坏整条链表断裂；mallocng free_bitmap 是连续数组，不在用户数据区，某个 slot 被破坏不影响其他位

3. **集中管理便于校验**：阶段四 header 分散在各处无法统一校验；mallocng meta 集中存储，可以加 magic number 或 checksum，每次访问时校验

一句话：元数据离用户越远，用户犯错时越不容易破坏分配器的核心结构。

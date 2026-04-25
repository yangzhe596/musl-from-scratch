# Day 16 - 学习总结与标准答案

## 今日是否完成

Day 16 可以视为完成。

完成标准是已经实现 free list、first-fit 搜索和块分裂：

- `my_free` 能标记块为空闲
- `find_free` 能从 free list 找到可复用的块
- `split_block` 能把大块分裂成小块
- 测试覆盖了 malloc → free → malloc 复用场景

## 今日总结

今天的重点是理解 free list 的设计和分裂策略。

在实现方面，我们已经有了：
- `find_free(size)`：first-fit 遍历 free list
- `split_block(hdr, size)`：剩余空间足够时分裂
- `my_free(ptr)`：标记 free=1，触发 merge_free

关键收获：
1. free list 是用户态数据结构，不是内核的
2. first-fit 简单但不一定最优，可能浪费大块
3. 分裂阈值（HEADER_SIZE + ALIGNMENT = 48）避免产生无法复用的碎片

设计决策：
- first-fit vs best-fit：first-fit 更快，best-fit 更省空间但 O(n)
- 分裂阈值：宁可浪费一点空间，也不制造碎片

## 标准答案

### 1. 为什么 first-fit 简单但不一定最优？

标准答案：

first-fit 找到第一个够大的块就用，可能浪费大块：

```
free_list: [块A:100字节] → [块B:16字节] → [块C:100字节]

malloc(16)：
  first-fit → 用块A，分裂后剩 84 字节
  best-fit  → 用块B，刚好 16 字节，不浪费
```

但 best-fit 要遍历整个链表找"最小的够用的"，O(n) 开销。

实际选择：大多数分配器用 first-fit 或其变体，因为性能更重要。

### 2. 分裂阈值为什么重要？

标准答案：

没有阈值的情况：
```
malloc(16) 从 100 字节块分裂 → [16][header=32][剩余52]
malloc(16) 从 52 字节块分裂  → [16][header=32][剩余4]
                                 ↑ 剩余 4 字节连 header 都放不下
                                 变成废块，永远无法复用
```

有阈值（48）：
```
剩余空间 < 48 → 不分裂，整个块给用户
浪费了空间，但块完整，下次能用
```

分裂阈值是"碎片控制阀"——宁可浪费空间，也不制造无法复用的碎片。

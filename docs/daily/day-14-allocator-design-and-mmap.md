# Day 14 - 堆分配器设计与 `mmap`

## 今天的目标

- 理解动态分配器需要解决的最小问题集合
- 确定阶段四采用 `mmap` 还是 `brk`
- 设计块头结构

## 预计投入

- 20 分钟：理解 `mmap` / `brk` 差异
- 20 分钟：设计元数据
- 20 分钟：画出内存块布局

## 今日任务

1. 在 `stage4/` 创建：
   - `malloc.c`
   - `malloc.h`
   - `main.c`
2. 先不急着写完整代码，先设计：
   - 块头字段
   - 空闲链表结构
   - 对齐策略
3. 画出布局图：

```text
[header][user data........]
```

## 推荐决策

教学上可以先理解 `brk`，但实现建议优先选 `mmap`，因为：

- 独立
- 语义更直观
- 便于后续过渡到 group/slot 思路

## 关键 syscall

- `mmap`
- 可选 `munmap`

## 验证标准

- 你能说清楚 `malloc` 至少要管理哪些元数据
- 你能解释“返回给用户的指针”和“块头指针”的关系

## musl 源码对照

- `src/mman/mmap.c`
- `src/malloc/oldmalloc/`
- `src/malloc/lite_malloc.c` 如果源码中存在

## 思考题

1. 为什么 `malloc` 返回的不能是块头地址？
2. 为什么分配器必须考虑对齐？

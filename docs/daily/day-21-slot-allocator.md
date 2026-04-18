# Day 21 - 实现教学版分槽分配器

## 今天的目标

- 实现一个最小可运行的 slot allocator
- 支持固定 size class 的分配与释放
- 把阶段五真正落成代码

## 预计投入

- 15 分钟：回顾昨天的结构设计
- 30 分钟：实现核心分配/释放逻辑
- 15 分钟：跑同尺寸和混合尺寸测试

## 今日任务

1. 实现：
   - `size_to_class`
   - `alloc_from_group`
   - `mini_malloc`
   - `mini_free`
2. 至少支持：
   - 同尺寸对象重复分配
   - 释放后再利用
   - 大对象单独路径
3. 如果时间不够，优先保证：
   - 小对象路径正确
   - bitmap 一致

## 重点理解

- 这一版不是完整 mallocng，只是“借鉴核心思想”
- 你今天最重要的工作是把“按 class 管理 slot”这件事真正跑起来

## 验证标准

- 小对象重复分配不必每次 syscall
- 释放后相同 class 能复用 slot
- 大对象路径与小对象路径分离

## musl 源码对照

- `src/malloc/mallocng/malloc.c`
- `src/malloc/mallocng/free.c`
- `src/malloc/mallocng/realloc.c`

## 思考题

1. 这套设计相比 free list，真正减少了什么成本？
2. 如果未来要支持 `realloc`，你会先考虑哪条路径？

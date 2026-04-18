# Day 20 - 设计 size class 与 group

## 今天的目标

- 设计教学版 size class
- 设计 group 元数据
- 决定大对象与小对象的分流策略

## 预计投入

- 20 分钟：确定 class 划分
- 20 分钟：设计结构体
- 20 分钟：推演分配流程

## 今日任务

1. 先选一组简单的 size class，例如：
   - 16
   - 32
   - 64
   - 128
   - 256
   - 512
   - 1024
2. 设计：
   - `size_to_class`
   - `group`
   - slot 使用位图或空闲索引
3. 决定：
   - 多大以上的对象直接单独 `mmap`

## 设计要求

- 先追求结构清晰，不追求完整功能
- 先支持小对象高频分配
- 大对象路径可以简单

## 验证标准

- 你能画出一次小对象分配的路径
- 你能说清楚 bitmap 记录的是什么

## musl 源码对照

- `src/malloc/mallocng/meta.h`
- `src/malloc/mallocng/malloc.c`

## 思考题

1. size class 太多和太少各有什么问题？
2. 大对象为什么通常不适合塞进小对象 group 里？

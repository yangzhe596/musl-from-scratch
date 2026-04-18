# Day 19 - 理解 mallocng 设计动机

## 今天的目标

- 理解 musl `mallocng` 的核心术语
- 明确阶段五不是“完整复刻”，而是“教学版抽象”
- 找出阶段四设计的主要痛点

## 预计投入

- 20 分钟：阅读概念
- 20 分钟：列出痛点对照
- 20 分钟：画出新的高层结构图

## 今日任务

1. 阅读并整理这些概念：
   - meta
   - group
   - slot
   - size class
2. 写一张对照表：
   - 阶段四的问题
   - 阶段五的对应改进思路
3. 画出“一个 group 管多个 slot”的示意图

## 重点理解

- 现代 allocator 不只是“找空闲块”
- 小对象分配的核心是批量化和分级管理
- 元数据组织方式会影响性能和安全性

## musl 源码对照

- `src/malloc/mallocng/meta.h`
- `src/malloc/mallocng/malloc.c`
- `src/malloc/mallocng/free.c`

## 验证标准

- 你能用自己的话解释什么是 size class
- 你能说明 group/slot 为什么比线性扫描 free list 更适合小对象

## 思考题

1. 为什么把很多同尺寸对象放在同一组里会更高效？
2. 为什么 metadata 的组织方式会影响 allocator 的鲁棒性？

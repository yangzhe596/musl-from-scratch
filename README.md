# musl-ai-learning

一个面向 x86_64 Linux 的手写 libc 学习项目。

目标不是复制一份完整的 musl，而是按阶段亲手实现一个“能跑、能测、能对照源码”的极简 libc 核心子集，并通过这个过程理解：

- 程序是如何在没有 glibc 的情况下启动的
- 用户态代码如何直接通过 syscall 与内核交互
- `strlen` / `memcpy` / `memset` 这类基础函数为什么是 libc 的根
- `printf` 的最小实现为什么本质上是“格式解析 + 输出后端”
- `malloc/free` 从最简单的 free list 到更现代的分槽思路是如何演进的
- musl 为什么能在极简风格下保持实现质量

## 适合谁

这个项目适合你如果：

- 已经熟悉 C 语言基础语法
- 想深入理解 Linux 系统编程和 libc 底层实现
- 希望通过手写代码建立“从 ELF 入口到堆分配器”的整体认知
- 愿意按每天约 1 小时的节奏稳定推进

不适合把这里当成“现成 libc 源码仓库”。这里的设计目标是学习，不是交付生产级实现。

## 学习方式

建议严格遵循下面的节奏：

1. 每天只做当天对应的 Markdown 任务。
2. 先自己写，再向 AI 提问，不要先索要完整答案。
3. 每完成一个阶段，都跑一遍验证命令，并对照 musl 源码。
4. 把你自己的代码、思考和踩坑记录一起保存在仓库里。

文档默认采用“先目标，再动手，再验证，再对照源码”的结构。这样做的目的是减少无效阅读，把 1 小时切成可落地的任务块。

## AI 协作方式

这个项目默认采用“苏格拉底式引导 + 手写实践 + Code Review”的学习方式。

推荐节奏如下：

1. 先阅读当天的任务文档，只关注当天问题，不提前跳阶段。
2. 先自己思考并回答关键问题，再开始手写代码。
3. 手写时只拿到思路、关键函数签名、数据结构和伪代码，不直接索要完整最终实现。
4. 完成后把代码交给 AI review，重点检查边界条件、ABI 约定、syscall 封装、对齐、未定义行为和内存管理错误。
5. 修正问题后，再进入下一天或下一阶段。

这套方式的目标不是更快拿到答案，而是把“理解原理”和“亲手实现”绑定在一起。

## 学习路线

本项目分为 5 个阶段：

1. 阶段一：点亮无 libc 的世界
2. 阶段二：纯逻辑的艺术
3. 阶段三：格式化输出
4. 阶段四：动态内存管理的基石
5. 阶段五：致敬 musl mallocng

对应的每日计划在 [docs/daily](/home/yz/code/musl-ai-learning/docs/daily)。

阶段学习结束后，可以把当天或当阶段的总结沉淀到 [docs/answers](/home/yz/code/musl-ai-learning/docs/answers)。

## 每日计划索引

- [Day 01 - 环境与目标](/home/yz/code/musl-ai-learning/docs/daily/day-01-environment-and-roadmap.md)
- [Day 02 - 认识 ELF 与入口点](/home/yz/code/musl-ai-learning/docs/daily/day-02-elf-and-entry.md)
- [Day 03 - 手写 `_start`](/home/yz/code/musl-ai-learning/docs/daily/day-03-write-start.md)
- [Day 04 - 第一个 syscall：`write` 与 `exit`](/home/yz/code/musl-ai-learning/docs/daily/day-04-first-syscalls.md)
- [Day 05 - 阶段一收尾与 musl 对照](/home/yz/code/musl-ai-learning/docs/daily/day-05-stage1-review.md)
- [Day 06 - `strlen` 朴素实现](/home/yz/code/musl-ai-learning/docs/daily/day-06-strlen-basics.md)
- [Day 07 - `memset` 与 `memcpy` 朴素实现](/home/yz/code/musl-ai-learning/docs/daily/day-07-memset-memcpy-basics.md)
- [Day 08 - 对齐与按字长优化](/home/yz/code/musl-ai-learning/docs/daily/day-08-alignment-and-word-at-a-time.md)
- [Day 09 - 阶段二测试与源码对照](/home/yz/code/musl-ai-learning/docs/daily/day-09-stage2-review.md)
- [Day 10 - 可变参数与最小输出框架](/home/yz/code/musl-ai-learning/docs/daily/day-10-stdarg-and-output-buffer.md)
- [Day 11 - 十进制与十六进制整数转字符串](/home/yz/code/musl-ai-learning/docs/daily/day-11-integer-formatting.md)
- [Day 12 - 实现最小 `printf`](/home/yz/code/musl-ai-learning/docs/daily/day-12-mini-printf.md)
- [Day 13 - 阶段三测试与源码对照](/home/yz/code/musl-ai-learning/docs/daily/day-13-stage3-review.md)
- [Day 14 - 堆分配器设计与 `mmap`](/home/yz/code/musl-ai-learning/docs/daily/day-14-allocator-design-and-mmap.md)
- [Day 15 - 实现最小 `malloc`](/home/yz/code/musl-ai-learning/docs/daily/day-15-minimal-malloc.md)
- [Day 16 - 实现 `free`、块分裂与复用](/home/yz/code/musl-ai-learning/docs/daily/day-16-free-split-reuse.md)
- [Day 17 - 合并空闲块与调试器](/home/yz/code/musl-ai-learning/docs/daily/day-17-coalesce-and-heap-checker.md)
- [Day 18 - 阶段四测试与源码对照](/home/yz/code/musl-ai-learning/docs/daily/day-18-stage4-review.md)
- [Day 19 - 理解 mallocng 设计动机](/home/yz/code/musl-ai-learning/docs/daily/day-19-mallocng-concepts.md)
- [Day 20 - 设计 size class 与 group](/home/yz/code/musl-ai-learning/docs/daily/day-20-size-class-and-group.md)
- [Day 21 - 实现教学版分槽分配器](/home/yz/code/musl-ai-learning/docs/daily/day-21-slot-allocator.md)
- [Day 22 - 阶段五对照与总结](/home/yz/code/musl-ai-learning/docs/daily/day-22-stage5-review.md)

## 推荐目录结构

建议你按阶段组织代码：

```text
.
├── README.md
├── docs/
│   └── daily/
│   └── answers/
├── stage1/
├── stage2/
├── stage3/
├── stage4/
└── stage5/
```

每个阶段内建议最少包含：

- `Makefile`
- `README.md`
- `start.S`
- `main.c`
- 与阶段相关的 `.c/.h`
- `tests/` 或最小测试入口

## 推荐编译原则

早期阶段尽量保持编译参数稳定：

```bash
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie \
    start.S main.c -o hello
```

常用验证命令：

```bash
readelf -h ./hello
objdump -d ./hello
strace ./hello
```

## 建议的提交节奏

开源项目建议采用小步提交：

- `docs: add day 01-05 learning plan`
- `stage1: add minimal crt and syscall wrappers`
- `stage2: implement basic string routines`
- `stage3: add mini printf`
- `stage4: add first-fit allocator`
- `stage5: prototype slot allocator`

这样后续回看历史时，你能同时看到“认知演进”和“代码演进”。

## 使用方式

如果你把这个仓库作为公开学习日志，建议每天完成后在对应阶段目录补充：

- 你手写的代码
- 你自己记录的 bug
- 一段 5 到 10 行的总结
- 与 musl 对照后的差异点

AI 的最佳使用方式不是索要最终答案，而是：

- 让 AI 帮你做 code review
- 让 AI 提醒你边界条件
- 让 AI 解释某个 ABI、syscall、对齐或 allocator 设计细节
- 让 AI 根据你的回答给出阶段总结和标准答案

## 参考资料

- musl 源码仓库
- System V AMD64 ABI
- Linux x86_64 syscall ABI
- *Computer Systems: A Programmer's Perspective*
- *Operating Systems: Three Easy Pieces*

## 从这里开始

建议从 [Day 01 - 环境与目标](/home/yz/code/musl-ai-learning/docs/daily/day-01-environment-and-roadmap.md) 开始，按每天约 1 小时的节奏推进。每完成一天，优先保证代码可运行、可验证、可解释，再进入下一天。

这个仓库的重点不是“尽快写完”，而是通过亲手实现和反复对照，真正理解一个最小 libc 核心是如何建立起来的。

# Day 22 - 阶段五对照与总结

## 今天的目标

- 总结整个项目
- 对照 musl 回看设计演进
- 为仓库补最后一层开源说明

## 预计投入

- 20 分钟：阶段五收尾
- 20 分钟：写最终总结
- 20 分钟：补充开源项目说明

## 今日任务

1. 为 `stage5/` 增加：
   - `Makefile`
   - `README.md`
2. 在仓库根目录补充或计划补充：
   - `LICENSE`
   - `.gitignore`
   - 最终总结文档
3. 写一页总结：
   - 你现在能独立解释哪些底层链路
   - 哪个阶段最难
   - 如果重做一次，你会怎么安排代码结构

## 对照 musl 的最终问题

1. musl 为什么坚持极简？
2. 你的教学版实现与 musl 正式实现差距最大的地方是什么？
3. 哪些差距是“工程性复杂度”，哪些差距是“原理没掌握”？

## musl 源码对照

- `crt/x86_64/crt1.s`
- `arch/x86_64/syscall_arch.h`
- `src/string/`
- `src/stdio/`
- `src/malloc/oldmalloc/`
- `src/malloc/mallocng/`

## 最终验证标准

- 你能从 `_start` 讲到 `malloc`
- 你能解释 libc 的一部分为什么可以纯逻辑实现，另一部分为什么必须与内核接口紧密耦合
- 你能清楚说明教学版实现和 musl 工业实现的边界

## 下一步建议

- 补 `memmove`
- 补 `calloc`
- 补 `realloc`
- 尝试支持 `argc/argv/envp`
- 尝试做一个最小测试框架

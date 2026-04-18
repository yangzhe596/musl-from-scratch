# Day 05 - 阶段一收尾与 musl 对照

## 今天的目标

- 收尾阶段一
- 给阶段一加一个最小 `Makefile`
- 对照 musl 理清“入口层”和“syscall 层”

## 预计投入

- 15 分钟：整理代码
- 20 分钟：补 `Makefile` 和说明
- 25 分钟：阅读 musl 对照文件

## 今日任务

1. 为 `stage1/` 增加：
   - `Makefile`
   - `README.md`
2. `Makefile` 至少支持：
   - `make`
   - `make run`
   - `make disasm`
3. 在 `README.md` 中写：
   - 当前支持功能
   - 编译方式
   - 你遇到的一个坑
4. 阅读 musl 并记录：
   - CRT 负责什么
   - syscall 封装负责什么

## 自查清单

- 你能不看资料写出 `write` 和 `exit` 的 syscall 编号吗？
- 你能解释 `_start` 和 `c_main` 的职责边界吗？
- 你知道当前实现还没处理 `argc/argv/envp` 吗？

## musl 源码对照

- `crt/x86_64/crt1.s`
- `arch/x86_64/syscall_arch.h`
- `src/exit/_Exit.c`
- `src/unistd/write.c`

## 今日产出

- 阶段一可独立编译运行
- 一份简短阶段总结

## 思考题

1. musl 的实现为什么没有你想象得“层数很多”？
2. 如果后续要支持更多 syscall，你会继续复制函数，还是抽象出通用 syscall 包装器？

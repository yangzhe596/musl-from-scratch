# Day 05 - 学习总结与标准答案

## 今日是否完成

Day 05 可以视为完成。

完成标准是已经完成阶段一的收尾、Makefile 整理和 musl 源码对照：

- stage1/ 可独立编译运行
- Makefile 支持 make / make run / make disasm
- 理解了 _start 和 c_main 的职责边界

## 今日总结

今天的重点是阶段一的收尾工作：整理代码、补 Makefile 和 README、对照 musl 源码。

在代码整理方面，stage1/ 已经包含：
- `start.S`：手写 `_start` 入口，调用 `c_main` 后将返回值传给 `sys_exit`
- `syscall.c` / `syscall.h`：`sys_write` 和 `sys_exit` 的内联汇编封装
- `main.c`：C 入口函数 `c_main`

在 musl 对照方面，我们理解了：
- CRT 负责什么：设置栈、调用 main、处理返回值
- syscall 封装负责什么：把 C 函数调用转成 Linux syscall 指令
- musl 的分层比想象中简单：`_start` → `__libc_start_main` → `main`

关键收获：
1. `_start` 不是函数，是 ELF 入口点，没有返回地址
2. syscall 编号是固定的（write=1, exit=60），参数通过寄存器传递
3. 当前实现还没处理 `argc/argv/envp`，这是后续可以补的

## 标准答案

### 1. write 和 exit 的 syscall 编号是什么？

标准答案：

- `write`：syscall 编号 1（`__NR_write`）
- `exit`：syscall 编号 60（`__NR_exit`）

参数传递遵循 x86_64 Linux syscall ABI：
- syscall 编号放 `rax`
- 参数依次放 `rdi`, `rsi`, `rdx`, `r10`, `r8`, `r9`

### 2. _start 和 c_main 的职责边界是什么？

标准答案：

- `_start`（汇编）：
  - 是 ELF 入口点，不是函数
  - 负责对齐栈（`sub $8, %rsp`）
  - 调用 `c_main`
  - 把返回值传给 `sys_exit`

- `c_main`（C）：
  - 是普通的 C 函数
  - 负责实际的程序逻辑
  - 返回值作为进程退出码

### 3. 为什么 musl 的分层没有想象中复杂？

标准答案：

musl 的 `_start` 最终也是调用 `main`，中间虽然有 `__libc_start_main` 做初始化（设置 TLS、调用构造函数等），但核心路径很直接。

我们的极简实现跳过了这些初始化，直接 `_start` → `c_main`，所以层数更少。这也说明：libc 的复杂度大部分来自"兼容性和便利性"，而不是"核心路径"。

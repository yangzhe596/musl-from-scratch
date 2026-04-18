# Day 03 - 学习总结与标准答案

## 今日是否完成

Day 3 完成。

今天成功手写了 `_start`，从汇编跳入 C 入口函数 `c_main`，并通过反汇编确认了控制流路径和返回值位置。

## 实际代码

### `stage1/start.S`

```asm
.global _start

.text
_start:
    sub $8, %rsp
    call c_main
    hlt
    jmp .-1
```

### `stage1/main.c`

```c
int c_main(void){
  return 42;
}
```

## 今日总结

今天最关键的进展是：第一次自己接住了程序真实入口。

在没有标准 CRT 的情况下，内核不会帮你调用 `main`，也不会自动处理返回路径。你必须自己定义 `_start`，并在里面显式调用 C 入口函数 `c_main`。

`_start` 调用 `c_main` 时遵守 x86_64 System V 函数调用约定，返回值放在 `rax/eax`。退出路径用 `hlt` + `jmp .-1` 做死循环暂停占位，Day 04 再用 `exit` syscall 接上。

## 踩过的坑

### 1. 忘记写 `_start:` 标签

`.global _start` 只是声明符号对外可见，**标签 `_start:` 才是定义入口位置**。两者缺一不可。

### 2. AT&T 汇编语法混淆

Intel 语法：`sub rsp, 8`
AT&T 语法：`sub $8, %rsp`

gcc 默认使用 AT&T 语法，关键规则：
- 立即数加 `$` 前缀：`$8`
- 寄存器加 `%` 前缀：`%rsp`
- 源操作数在前，目的操作数在后：`sub $src, %dst`

### 3. 函数名要和 `call` 一致

`start.S` 里 `call c_main`，`main.c` 里函数名必须是 `c_main` 而不是 `main`，链接器按符号名匹配。

## 标准答案

### 1. `_start` 调用 `c_main` 时遵守哪套调用约定？

标准答案：

遵守 x86_64 System V ABI，即普通用户态函数调用约定，而不是 Linux syscall 约定。

- 函数参数通过 rdi, rsi, rdx, rcx, r8, r9 传递
- `call` 会压入返回地址
- 返回值放在 rax

### 2. 为什么 syscall 又是另一套寄存器约定？

标准答案：

不是设计者为了区分，而是 **CPU 硬件层面的限制**。`syscall` 指令执行时，CPU 会自动把返回地址存入 rcx，把 RFLAGS 存入 r11，导致 rcx 和 r11 被硬件占用。因此 syscall 约定无法用 rcx 传第 4 个参数，改用 r10 代替。

| | 函数调用 ABI | Syscall ABI |
|---|---|---|
| 触发方式 | `call` 指令 | `syscall` 指令 |
| 参数寄存器 | rdi, rsi, rdx, **rcx**, r8, r9 | rdi, rsi, rdx, **r10**, r8, r9 |
| 返回值 | rax | rax |

### 3. `c_main` 返回后，返回值在哪个寄存器里？

标准答案：

返回值在 `rax`。如果返回类型是 `int`，通常直接看 `eax` 部分。反汇编中 `mov $0x2a, %eax` 即将 42 放入 eax。

### 4. 为什么 `_start` 不能直接 `ret`？

标准答案：

`_start` 不是被某个调用者通过 `call` 调起的，它是进程入口，栈上没有合法的返回地址。直接 `ret` 会让执行流跳到栈上的垃圾数据对应的地址，通常导致段错误。

### 5. 为什么今天不急着处理 `argc`、`argv` 和 `envp`？

标准答案：

Day 3 的目标只是先打通最短控制流路径。如果一开始就把参数解析、栈布局和退出路径全部混进来，学习重点会被冲散。先确认 `_start` 可以主动调用 C 函数且返回值能回来，再逐步补全。

### 6. `hlt` 和 `jmp .-1` 的含义？

标准答案：

- `hlt` = Halt，让 CPU 进入暂停状态，大幅降低功耗。但收到中断会唤醒继续执行下一条指令。
- `.` = 当前地址，`.-1` = 当前地址减 1，即 `hlt` 指令的地址。
- 两个配合：暂停 → 被中断唤醒 → 跳回 hlt 再次暂停，形成不浪费 CPU 算力的死循环。
- 比 `jmp .` 单纯死循环更规范，因为后者会让 CPU 100% 占用一个核心。

### 7. x86 汇编指令命名规律？

标准答案：

大多取自英文单词缩写：`sub` = Subtract，`add` = Add，`mov` = Move，`call` = Call，`ret` = Return，`jmp` = Jump，`push` = Push，`pop` = Pop，`hlt` = Halt。

## 今日最小知识闭环

- 程序真实入口是 `_start`
- 没有 CRT 时，`_start` 要自己写
- `.global` 声明符号可见性，标签定义入口位置，两者都要
- AT&T 汇编语法：`$` 立即数，`%` 寄存器，源在前目的在后
- `call` 前要 `sub $8, %rsp` 保证栈 16 字节对齐
- `_start` 不能 `ret`，用 `hlt` + `jmp .-1` 占位
- `_start` 调 `c_main` 走普通函数调用 ABI，返回值在 `rax/eax`
- syscall 约定不同是因为硬件占用 rcx/r11，不是人为区分

## 反汇编验证

```
0000000000401000 <_start>:
  401000:   48 83 ec 08          sub    $0x8,%rsp
  401004:   e8 03 00 00 00       call   40100c <c_main>
  401009:   f4                   hlt
  40100a:   eb fd                jmp    401009 <_start+0x9>

000000000040100c <c_main>:
  40100c:   f3 0f 1e fa          endbr64
  401010:   55                   push   %rbp
  401011:   48 89 e5             mov    %rsp,%rbp
  401014:   b8 2a 00 00 00       mov    $0x2a,%eax
  401019:   5d                   pop    %rbp
  40101a:   c3                   ret
```

验证通过：
- `_start` 通过 `call` 跳转到 `c_main` ✓
- `c_main` 返回值 42 (0x2a) 放入 `eax/rax` ✓

## 进入 Day 04 前，你应该已经能回答

1. 为什么 `_start` 调 C 函数时不能套用 syscall 寄存器约定？
2. 为什么 `c_main` 返回值会出现在 `rax/eax`？
3. 为什么 `_start` 不能直接 `ret`？
4. 为什么把"调用 C"与"退出进程"拆成两天更合理？
5. `hlt` + `jmp .-1` 比 `jmp .` 死循环好在哪里？

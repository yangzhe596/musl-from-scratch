# 补充知识 - argc/argv/envp 的底层机制

## 核心问题

`main(argc, argv)` 里的参数是从哪来的？

## 答案：内核在 execve 时准备

```
Shell 调用 execve("./demo", ["./demo","hello","world"], envp)
  ↓
内核处理 execve：
  1. 读取 ELF 文件，加载到内存
  2. 创建新的虚拟地址空间
  3. 在栈顶布置 argc, argv, envp, auxv
  4. 把 rsp 指向 argc
  5. 把 rip 指向 ELF 入口点（_start）
  6. 切换到用户态，开始执行 _start
```

## 栈布局

```
高地址
  ↑
  [envp 字符串...]     ← 内核从 execve 的 envp 参数复制
  [argv 字符串...]     ← 内核从 execve 的 argv 参数复制
  [auxv...]            ← 内核填入 ELF 信息
  [NULL]               ← envp 结束
  [envp[0]]            ← 指针，指向上面的字符串
  [envp[1]]
  ...
  [NULL]               ← argv 结束
  [argv[0]]            ← 指针，指向上面的字符串
  [argv[1]]
  ...
  [argc]               ← 内核从 execve 的 argv 数组长度计算
  ↓
rsp 指向这里
低地址
```

## x86_64 调用约定

```
第 1 个参数 → rdi
第 2 个参数 → rsi
第 3 个参数 → rdx
第 4 个参数 → rcx
第 5 个参数 → r8
第 6 个参数 → r9
返回值     → rax
```

## 三种实现方式

### 1. glibc 的做法

```asm
_start:
  mov (%rsp), %rdi          ← argc 放 rdi
  lea 8(%rsp), %rsi         ← argv 放 rsi
  lea 8(%rsp,%rdi,8), %rdx  ← envp 放 rdx
  call __libc_start_main
```

### 2. musl 的做法

```asm
_start:
  mov %rsp, %rdi            ← 把栈指针传给 __libc_start_main
  and $-16, %rsp
  call __libc_start_main
```

`__libc_start_main` 内部从 rdi 取出 argc, argv, envp。

### 3. 教学版的做法

```asm
_start:
  mov %rsp, %rdi            ← 把栈指针传给 c_main
  and $-16, %rsp
  call c_main
```

```c
int c_main(unsigned long *sp)
{
    int argc = (int)sp[0];
    char **argv = (char **)(sp + 1);
    char **envp = argv + argc + 1;
}
```

## 完整启动路径对比

```
glibc:
  _start → __libc_start_main → __libc_init_first → main
  初始化：TLS、atexit、构造函数、安全检查、动态链接

musl:
  _start → __libc_start_main → main
  初始化：TLS、atexit、构造函数

教学版:
  _start → c_main
  初始化：无
```

## 关键结论

1. `main(argc, argv)` 不是魔法，是 CRT 从栈上取出来传给你的
2. 栈上的数据是内核在 `execve` 时准备好的
3. 教学版跳过了所有初始化，所以没有 TLS、atexit、全局构造函数
4. musl 比 glibc 简洁，但核心流程一样

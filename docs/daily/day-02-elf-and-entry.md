# Day 02 - 认识 ELF 与入口点

## 今天的目标

- 理解 ELF 的入口点概念
- 区分 `_start` 与 `main`
- 知道程序启动时栈上大致有什么

## 预计投入

- 15 分钟：读入口点和 ABI 基础
- 25 分钟：用宿主编译一个普通程序并观察 ELF
- 20 分钟：整理 `_start` 需要做什么

## 今日任务

1. 写一个最普通的宿主程序：

```c
int main(void) { return 0; }
```

2. 编译后观察：

```bash
gcc -g -no-pie main.c -o normal
readelf -h ./normal
readelf -s ./normal | less
objdump -d ./normal | less
```

3. 回答：
   - 入口点地址是什么？
   - `_start` 是否存在？
   - `main` 是谁调用的？

## 你需要掌握的概念

- ELF header 中的 `Entry point`
- CRT 的职责
- 启动时栈上传递的 `argc` / `argv` / `envp`

## 重点理解

你后面自己手写的 `_start`，本质上是在替代宿主 CRT 的最小功能。它至少要解决两件事：

- 把控制权接住
- 在退出前显式调用 `exit` syscall

## 验证标准

- 你能在反汇编里找到 `_start`
- 你能解释为什么 `main` 不是入口点

## musl 源码对照

定位并阅读：

- `crt/x86_64/crt1.s`

阅读时只回答一个问题：

- musl 的 `_start` 把控制权交给了谁？

## 思考题

1. 如果 `_start` 直接 `ret`，会发生什么？
2. 为什么程序入口不能假定调用者已经准备好了返回地址？

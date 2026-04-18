# Day 10 - 可变参数与最小输出框架

## 今天的目标

- 理解 `stdarg.h` 的使用方式
- 搭一个最小输出缓冲框架
- 为后续 `printf` 做拆分设计

## 预计投入

- 20 分钟：理解 `va_list` 基本用法
- 20 分钟：设计输出辅助函数
- 20 分钟：写一个最小 demo

## 今日任务

1. 在 `stage3/` 创建：
   - `printf.c`
   - `printf.h`
   - `main.c`
2. 先不要直接写 `printf`
3. 先设计以下辅助接口之一：

```c
static void put_char(char c);
static void put_str(const char *s);
```

或

```c
struct out {
    char buf[...];
    unsigned long len;
};
```

4. 写一个最小可变参数实验函数，确认你会用：

```c
int demo(const char *tag, ...);
```

## 重点理解

- `stdarg.h` 是编译器支持的语言接口，不等于依赖完整 libc
- `printf` 的复杂度主要来自格式解析，不来自输出本身

## 验证标准

- 你能正确使用 `va_start` / `va_arg` / `va_end`
- 你能决定后续是“边解析边输出”还是“先写入小缓冲再统一输出”

## musl 源码对照

- `src/stdio/printf.c`
- `src/stdio/vfprintf.c`

## 思考题

1. 为什么工业实现通常会把 `printf` 委托给 `vfprintf`？
2. 如果输出后端只有 `write`，你的 `printf` 该怎么分层？

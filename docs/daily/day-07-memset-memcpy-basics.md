# Day 07 - `memset` 与 `memcpy` 朴素实现

## 今天的目标

- 理解“按字节处理”的最基础版本
- 明确 `memcpy` 与重叠内存的关系
- 建立一组可复用测试

## 预计投入

- 15 分钟：梳理语义差异
- 30 分钟：手写函数
- 15 分钟：打印测试结果

## 今日任务

实现：

```c
void *memset(void *dst, int c, unsigned long n);
void *memcpy(void *dst, const void *src, unsigned long n);
```

测试至少覆盖：

- `n == 0`
- 普通小长度
- 非 0 填充值
- `memcpy` 的源和目的不同

## 重点理解

- `memset` 中的 `int c` 实际按 `unsigned char` 使用
- `memcpy` 不保证处理重叠区域
- 返回值通常是原始 `dst`

## 验证标准

- 你能通过打印数组内容验证结果
- 你故意构造一个重叠用例，并观察到 `memcpy` 结果不应被依赖

## musl 源码对照

- `src/string/memcpy.c`
- `src/string/memset.c`
- `src/string/memmove.c`

## 思考题

1. 为什么标准库里同时存在 `memcpy` 和 `memmove`？
2. 为什么 `memset` 的第二个参数不用 `char`？

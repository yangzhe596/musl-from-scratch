# Day 06 - `strlen` 朴素实现

## 今天的目标

- 理解 `strlen` 的语义
- 写一个逐字节版本
- 复用阶段一的 `write` 做最小测试

## 预计投入

- 15 分钟：明确边界条件
- 25 分钟：实现 `strlen`
- 20 分钟：写 3 到 5 个测试用例

## 今日任务

1. 在 `stage2/` 建立最小文件：
   - `string.c`
   - `string.h`
   - `main.c`
2. 实现：

```c
unsigned long strlen(const char *s);
```

3. 至少测试：
   - 空字符串
   - 普通字符串
   - 长一点的字符串

## 重点理解

- `strlen` 返回的是不包含终止 `'\0'` 的长度
- 它依赖输入必须是合法 C 字符串
- 对 `NULL` 调用 `strlen` 不属于正常语义范围

## 建议编译

```bash
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie \
    stage1/start.S stage1/syscall.c stage2/string.c stage2/main.c -o stage2/test_strlen
```

## 验证标准

- 你能打印测试结果
- 你能区分“长度是 0”和“字符串里第一个字符就是 `'\0'`”

## musl 源码对照

- `src/string/strlen.c`

## 思考题

1. 为什么 `strlen` 看起来最简单，却常常是高频热点函数？
2. 如果编译器把 `strlen("abc")` 直接优化成常量 3，会影响你的学习目的吗？

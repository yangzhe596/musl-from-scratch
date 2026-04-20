# Day 06 - 学习总结与标准答案

## 今日是否完成

Day 6 完成。

今天实现了朴素版 `strlen`，并编写了数字转字符串的辅助函数用于测试输出，正式进入 Stage 2（字符串/内存基础）。

## 实际代码

### `stage2/string.h`

```c
#ifndef STRING_H
#define STRING_H

unsigned long strlen(const char *s);

#endif
```

### `stage2/string.c`

```c
#include "string.h"

unsigned long strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}
```

### `stage2/main.c`

```c
#include "syscall.h"
#include "string.h"

static void print_ulong(unsigned long val)
{
	char buf[20];
	int i = 0;

	do {
		buf[i++] = val % 10 + '0';
		val /= 10;
	} while (val);

	while (--i >= 0)
		sys_write(1, &buf[i], 1);
}

static void test_strlen(const char *s)
{
	unsigned long len = strlen(s);

	sys_write(1, "\"", 1);
	if (len > 0)
		sys_write(1, s, len);
	sys_write(1, "\" -> ", 5);
	print_ulong(len);
	sys_write(1, "\n", 1);
}

int c_main(void)
{
	test_strlen("");
	test_strlen("hello");
	test_strlen("a");
	test_strlen("1234567890");
	return 0;
}
```

## 今日总结

今天最关键的进展：第一次脱离 libc 的字符串函数，从零实现了 `strlen` 和数字转字符串输出。

在没有 `printf` 的环境下，验证函数正确性需要自己解决数字输出问题。这促使我们实现了 `print_ulong`——通过 `% 10` 取末位、`/ 10` 去末位的方式拆解数字，并注意了顺序反转和 `len == 0` 的边界情况。

`strlen` 的实现从手动计数 `cnt++` 演进到指针差 `p - s`，后者是 musl 的写法，更简洁也更具表达力。

## 踩过的坑

### 1. `strlen` 中忘记移动指针

初始版本只递增 `cnt` 但没有 `ptr++`，导致死循环。遍历类函数的核心是**每一步都要推进游标**。

### 2. 数字拆解顺序相反

`% 10` 从低位到高位拆出数字，先拆出的是个位。如果直接按顺序存入 buffer，打印出来是反的。解决方案：从前往后存，反向输出。

### 3. `len == 0` 的边界情况

`while (val != 0)` 循环在 `val == 0` 时一次不进，什么都不输出。改用 `do...while` 保证至少执行一次，自然处理了零值的输出。

### 4. 输出缺少换行符

没有换行时，终端提示符会拼在数字后面（如 `0%`），看起来像输出了垃圾。记得在每行输出末尾加 `\n`。

## 标准答案

### 1. 为什么 `strlen` 返回 `unsigned long` 而不是 `int`？

标准答案：

字符串长度不可能为负，用无符号类型更语义清晰。在 64 位系统上，`size_t`（即 `unsigned long`）能表达更大的范围，与 `sizeof` 等保持一致。

### 2. 为什么对 `NULL` 调用 `strlen` 是未定义行为？

标准答案：

`strlen` 的契约是"调用者保证传入合法 C 字符串"。`NULL` 不是合法字符串指针，解引用 `NULL` 会触发段错误。musl 也不做 NULL 检查，因为这种防御性检查会拖慢每次正常调用——用正确性约束换取性能。

### 3. 指针差 `p - s` 和手动计数有什么区别？

标准答案：

功能完全等价，但指针差更简洁：
- `p - s` 直接利用 C 指针算术，一次运算得到长度
- 手动计数需要额外变量 `cnt`，每次循环做两件事（计数 + 移动指针）
- musl 使用指针差写法

### 4. `print_ulong` 为什么用 `do...while` 而不是 `while`？

标准答案：

`do...while` 保证循环体至少执行一次，自然处理 `val == 0` 的情况。如果用 `while (val)` 则需要额外判断零值，增加分支。

### 5. 为什么 `buf[20]` 足够存放 `unsigned long` 的十进制表示？

标准答案：

`unsigned long` 最大值为 2^64 - 1 = 18446744073709551615，共 20 位十进制数字。因此 20 字节的 buffer 刚好够用。

## 今日最小知识闭环

- `strlen` 返回不含 `'\0'` 的长度，对 NULL 调用是 UB
- 返回类型用 `unsigned long`（即 `size_t`），语义更准确
- 指针差 `p - s` 比 `cnt++` 计数更简洁，是 musl 的写法
- 数字转字符串：`% 10` 取末位，`/ 10` 去末位，注意顺序反转
- `do...while` 自然处理零值边界
- 没有 `printf` 时需要自己实现数字输出
- 头文件要加 include guard 防止重复包含

## 进入 Day 07 前，你应该已经能回答

1. `strlen` 对 `NULL` 调用会怎样？为什么标准库不做防御？
2. 指针差 `p - s` 和手动计数 `cnt++`，哪个更接近 musl 的写法？
3. 数字转字符串时，为什么拆出来的位是反的？怎么解决？
4. `do...while` 相比 `while` 在数字转字符串中有什么优势？

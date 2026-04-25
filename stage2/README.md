# Stage 2 - 字符串/内存基础

## 支持功能

- `strlen` — 朴素逐字节实现（指针差写法）
- `memset` — 三步走优化版本（先对齐，再按 size_t 块填充，最后处理尾巴）
- `memcpy` — 朴素逐字节实现
- `print_ulong` — 无 printf 环境下的数字输出辅助函数
- `print_hex` — 十六进制输出辅助函数
- `print_memory` — 内存内容打印辅助函数

## 编译与运行

```bash
make          # 编译
make run      # 编译并运行
make disasm   # 编译并反汇编
make clean    # 清理
```

## 文件说明

| 文件 | 职责 |
|------|------|
| `string.h` | 字符串函数声明 |
| `string.c` | `strlen`、`memset`、`memcpy` 实现 |
| `main.c` | 测试入口，含辅助函数和测试用例 |

依赖 `stage1/` 的 `start.S` 和 `syscall.c`，不重复拷贝。

## 测试覆盖

### strlen 测试
- 空字符串
- 普通字符串
- 单字符字符串
- 长字符串

### memset 测试
- `n == 0`
- 填充 `0x00`、`0xFF`、自定义值
- 对齐地址
- 非对齐地址
- 小长度（小于 `size_t`）

### memcpy 测试
- `n == 0`
- 正常复制
- 复制到不同偏移

## 关键实现细节

### memset 优化（三步走）
1. **第一步**：按字节处理到对齐边界
2. **第二步**：按 `size_t` 宽度批量处理
3. **第三步**：处理剩余的尾巴

### 位运算技巧
- 对齐判断：`addr & 7 == 0`
- 填充值扩展：`val * 0x0101010101010101ULL`
- 边界计算：`align = (8 - ((unsigned long)p & 7)) & 7`

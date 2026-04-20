# Stage 2 - 字符串/内存基础

## 支持功能

- `strlen` — 朴素逐字节实现（指针差写法）
- `print_ulong` — 无 printf 环境下的数字输出辅助函数

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
| `string.c` | `strlen` 实现 |
| `main.c` | 测试入口，含 `print_ulong` 和 `test_strlen` |

依赖 `stage1/` 的 `start.S` 和 `syscall.c`，不重复拷贝。

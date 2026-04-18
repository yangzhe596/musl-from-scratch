# musl-from-scratch - AI 项目指引

## 项目定位

面向 x86_64 Linux 的手写 libc 学习项目。目标不是复制完整 musl，而是按阶段亲手实现极简 libc 核心子集，通过实践理解从 ELF 入口到堆分配器的完整链路。

## 目录结构

```text
.
├── README.md              # 项目总览与学习路线
├── docs/
│   ├── daily/             # 每日学习任务文档 (Day 01 - Day 22)
│   └── answers/           # 每日/每阶段总结与标准答案
├── stage1/                # 阶段一：无 libc 启动 (Day 01-05)
├── stage2/                # 阶段二：字符串/内存基础 (Day 06-09)
├── stage3/                # 阶段三：格式化输出 (Day 10-13)
├── stage4/                # 阶段四：动态内存管理 (Day 14-18)
└── stage5/                # 阶段五：mallocng 分槽分配器 (Day 19-22)
```

## 阶段与天数对应

| 阶段 | 天数 | 核心内容 |
|------|------|----------|
| Stage 1 | Day 01-05 | `_start`、ELF 入口、`write`/`exit` syscall |
| Stage 2 | Day 06-09 | `strlen`、`memset`、`memcpy`、对齐与字长优化 |
| Stage 3 | Day 10-13 | `stdarg`、整数格式化、最小 `printf` |
| Stage 4 | Day 14-18 | `mmap`、free list malloc/free、合并与调试 |
| Stage 5 | Day 19-22 | mallocng 设计、size class/group、分槽分配器 |

## 编译与验证

```bash
# 标准编译参数
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie \
    start.S main.c -o hello

# 验证工具链
readelf -h ./hello
objdump -d ./hello
strace ./hello
```

## AI 协作规范

本项目采用苏格拉底式引导学习，必须遵守以下原则：

### 引导优先，不给答案
- 用户问问题时，优先用反问或提示引导思考，不要直接给完整实现
- 只提供思路、关键函数签名、数据结构和伪代码
- 除非用户明确说"给我标准答案"或"我不明白，直接告诉我"

### Code Review 重点
审查代码时重点关注：
- **边界条件**：空指针、零长度、整数溢出
- **ABI 约定**：x86_64 SysV 调用约定（rdi/rsi/rdx/rcx/r8/r9 传参，rax 返回）
- **syscall 封装**：x86_64 Linux syscall ABI（rdi/rsi/rdx/r10/r8/r9 传参，syscall 号放 rax）
- **对齐**：内存访问对齐要求，结构体布局
- **未定义行为**：strict aliasing、有符号溢出、空指针解引用
- **内存管理**：泄漏、double free、use-after-free

### 阶段纪律
- 不要跨阶段引导，严格按 Day 顺序推进
- 不要提前给出后续阶段的知识或实现
- 每完成一天，确认验证通过后再进入下一天

### 语言
- 文档和解释用中文
- 代码注释和变量名用英文
- 技术术语保留英文原文（syscall、ABI、ELF、free list 等）

## Git 提交风格

```
docs: add day 01-05 learning plan
stage1: add minimal crt and syscall wrappers
stage2: implement basic string routines
stage3: add mini printf
stage4: add first-fit allocator
stage5: prototype slot allocator
```

## 学习进度

当前进度：Day 01-03 已有总结与标准答案，尚无实际代码。

## 参考资料

- musl 源码仓库（对照用，不直接复制）
- System V AMD64 ABI
- Linux x86_64 syscall ABI
- *CS:APP* / *OSTEP*

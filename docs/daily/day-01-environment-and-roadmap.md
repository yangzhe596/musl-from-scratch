# Day 01 - 环境与目标

## 今天的目标

- 准备好编译和验证环境
- 建立本项目的目录结构
- 明确五个阶段分别解决什么问题

## 预计投入

- 10 分钟：安装和检查工具
- 20 分钟：阅读总路线并创建目录
- 20 分钟：了解 `-nostdlib` / `-ffreestanding`
- 10 分钟：记录今天的理解

## 今日任务

1. 安装或确认以下工具可用：
   - `gcc`
   - `ld`
   - `objdump`
   - `readelf`
   - `strace`
   - `gdb`
2. 创建目录：
   - `stage1/`
   - `stage2/`
   - `stage3/`
   - `stage4/`
   - `stage5/`
3. 阅读根目录 `README.md`
4. 写下你对下面 3 个问题的初步理解：
   - 没有 libc 时，程序从哪里开始？
   - 为什么 `printf` 不能直接用？
   - “freestanding” 和 “hosted” 有什么区别？

## 你需要掌握的概念

- freestanding 编译环境
- 不链接宿主 glibc 的含义
- ELF 可执行文件不是“自动从 `main` 开始”

## 今日命令

```bash
gcc --version
ld --version
objdump --version
readelf --version
strace -V
gdb --version
mkdir -p stage1 stage2 stage3 stage4 stage5
```

## 验证标准

- 所有工具都能正常执行
- 阶段目录创建完成
- 你能用自己的话解释“为什么这次学习不能直接从 `printf` 开始”

## musl 源码对照

今天只做定位，不需要深读：

- `crt/`
- `arch/x86_64/`
- `src/string/`
- `src/stdio/`
- `src/malloc/`

## 思考题

1. 如果没有 libc，编译器还能生成机器码吗？
2. 如果没有 `main`，C 程序还能运行吗？

## 今日产出

- 建好目录
- 写一段 5 行以内的学习日志

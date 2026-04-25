# Day 22 - 学习总结与标准答案

## 今日是否完成

Day 22 可以视为完成。

完成标准是已经完成阶段五的收尾和整个项目的总结：

- stage5/ 有完整的 Makefile、README、测试
- 能从 `_start` 讲到 `malloc`
- 能解释教学版和 musl 的差距

## 今日总结

今天是整个项目的最终总结。

### 从 _start 到 malloc 的完整链路

```
_start (stage1)
  ↓ 调用
c_main
  ↓ 调用
sys_write / sys_exit (stage1 syscall 封装)
  ↓ 被复用
strlen / memset / memcpy (stage2 纯逻辑函数)
  ↓ 被复用
put_char / put_str / mini_printf (stage3 格式化输出)
  ↓ 被复用
my_malloc / my_free (stage4 free list 分配器)
  ↓ 演进
mini_malloc / mini_free (stage5 分槽分配器)
```

### 哪部分是纯逻辑，哪部分必须耦合内核

```
纯逻辑（不依赖内核）：
  - strlen / memset / memcpy → 纯内存操作
  - itoa10 / utoa16 → 纯数学转换
  - bitmap 操作 → 纯位运算

必须耦合内核：
  - _start → ELF 入口点，内核决定
  - sys_write / sys_exit → syscall 接口
  - mmap / munmap → 虚拟内存管理
  - printf 最终输出 → 必须 syscall
```

### 教学版与 musl 的差距

**工程性复杂度（你知道怎么做，只是没做）：**
- 加 `%ld`、`%u`、`%c`、`%p` 格式支持
- 加 `realloc`、`calloc`
- 加线程安全（锁）
- 加 group 回收（munmap 空闲 group）
- 加 `argc/argv/envp` 支持

**原理没掌握（需要深入学习）：**
- 浮点数格式化（需要大整数算术）
- TLS（线程本地存储）实现
- 位置参数 `%1$d`（需要两遍解析）
- 宽字符支持（编码转换）
- CRT 初始化的完整流程

### musl 为什么坚持极简

```
musl 的设计哲学：
  - 代码少 → bug 少 → 安全
  - 纯 C → 可移植 → 嵌入式/容器都能用
  - 不追求极致性能 → 追求正确性和可维护性

glibc 的问题：
  - 每个平台一套手写汇编 → 维护成本高
  - 某些优化在小 CPU 上反而更慢
  - 代码量大，历史悠久，bug 多

musl mallocng 不是"简单"，是"代码简洁但设计精巧"：
  - size class + group + bitmap → 现代设计
  - 代码几百行 vs glibc 几千行
  - 安全性高（元数据和用户数据分离）
```

## 下一步建议

如果继续深入，建议按这个顺序：
1. 补 `memmove`（重叠内存处理）
2. 补 `calloc`（零初始化 + overflow 检查）
3. 补 `realloc`（原地扩展 or 分配新块+复制）
4. 尝试支持 `argc/argv/envp`
5. 尝试做一个最小测试框架

# Day 20 - 学习总结与标准答案

## 今日是否完成

Day 20 可以视为完成。

完成标准是已经设计了教学版 size class 和 group 结构：

- 确定了 size class 划分（16、32、48、64、128、256、512、1024）
- 设计了 group 结构体（size_class、slot_count、bitmap、next）
- 理解了 size_to_class 的映射逻辑
- 决定了大对象直接 mmap 的分流策略

## 今日总结

今天的重点是设计 size class 和 group 结构，为 Day 21 的实现做准备。

在 size class 方面：
- 选择 8 个 size class：16、32、48、64、128、256、512、1024
- size_to_class 把用户请求的 size 向上取整到最近的 size class
- 太多 size class 难于管理，太少造成内存浪费

在 group 结构方面：
- 一个 group 管理多个同 size class 的 slot
- bitmap 用位图记录每个 slot 的空闲/占用状态
- 分配时找 bitmap 里第一个 0 的位，O(1) 复杂度

在大对象分流方面：
- 小对象（≤ 1024）→ group + slot
- 大对象（> 1024）→ 直接 mmap，不走 group
- 大对象分配频率低、复用概率低，直接 syscall 更简单

关键收获：
1. bitmap 比链表更适合小对象分配：O(1) 查找，无指针开销
2. slot 无单独 header，空间利用率高
3. 大对象不适合塞进 group：slot 数量少，bitmap 浪费，meta 开销相对大
4. size class 的选择是空间和管理复杂度的 trade-off

## 标准答案

### 1. size class 太多和太少各有什么问题？

标准答案：

**太少（比如只有 16、256、4096）：**
- 内部碎片严重：malloc(17) 拿 256，浪费 239 字节
- 但 group 数量少，管理简单

**太多（比如 16、17、18、19...1024）：**
- 内部碎片少：malloc(17) 拿 17，不浪费
- 但 group 数量多，每个 group 都有 meta 开销
- 很多 size class 可能永远用不到

**实际选择：** musl 用 30 个 size class（16、32、48、64...递增），是折中方案。

### 2. 大对象为什么不适合塞进小对象 group？

标准答案：

**小对象 group：**
```
[meta][slot:16][slot:16][slot:16]...[slot:16]
一个 group 放几百个 slot，bitmap 位很多，效率高
```

**大对象（比如 1MB）塞进 group：**
```
[meta][slot:1MB][slot:1MB]
一个 group 只能放 2-3 个 slot
bitmap 只用 2-3 位，meta 开销相对大
```

而且大对象分配频率低、复用概率低，直接 mmap/munmap 更简单，不需要 group 管理的复杂性。

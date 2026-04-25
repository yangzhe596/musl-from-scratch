# Stage 5 - 教学版分槽分配器 (mini_mallocng)

## 实现功能

- `mini_malloc(size)` — 按 size class 分配，小对象走 group/slot，大对象走 mmap
- `mini_free(ptr)` — 释放 slot 或 munmap 大对象

## 核心设计

### Size Class

```
16, 32, 48, 64, 128, 256, 512, 1024
```

### 内存布局

```
小对象 (size ≤ 1024):
  [group 元数据][slot[0]][slot[1]]...[slot[255]]
  group 记录 size_class、bitmap、free_count
  bitmap 每 bit 管一个 slot 的空闲/占用状态

大对象 (size > 1024):
  直接 mmap，不走 group
```

### 分配流程

```
mini_malloc(size)
├── size > 1024 → 直接 mmap
└── size ≤ 1024
    ├── size_to_class(size) → class_idx
    ├── 遍历 groups[class_idx] 找 free_count > 0 的 group
    ├── 找到 → alloc_from_group (bitmap O(1))
    └── 没找到 → create_group → alloc_from_group
```

## 与阶段四的对比

| 方面 | 阶段四 free_list | 阶段五 mini_mallocng |
|------|------------------|---------------------|
| 查找复杂度 | O(n) 遍历链表 | O(1) bitmap |
| header 开销 | 每块 32 字节 | slot 无单独 header |
| 碎片 | 外部碎片严重 | 无外部碎片（同 size class） |
| 大对象 | 和小对象一样走 free_list | 直接 mmap |

## 依赖

- `stage1/` 的 `start.S` 和 `syscall.c`
- `stage3/` 的 `printf.c`

## 编译与运行

```bash
make
make run
```

# 项目结构与命名规则

本仓库（c4phy）的目录组织与命名约定。新增笔记或项目时请遵循本规则。

---

## 目录

- [顶层结构](#顶层结构)
- [命名规则](#命名规则)
- [资源与路径引用](#资源与路径引用)
- [构建产物](#构建产物)
- [新项目 / 新笔记 Checklist](#新项目--新笔记-checklist)
- [附：本次整理对照表](#附本次整理对照表)

---

## 顶层结构

```
c4phy/
├── notes/      # 学习笔记（按教材章节组织）
├── project/    # 项目实践（按实现语言组织）
│   ├── c/      #   C 项目（Raylib 可视化）
│   ├── cpp/    #   C++ 项目（SDL2 可视化）
│   └── python/ #   Python 项目（matplotlib 可视化）
├── docs/       # 仓库级文档（本文件所在处）
└── README.md
```

层级固定为 **三层**：`project/<语言>/<项目名>/`。
不要再嵌套多余的层（历史遗留的 `project/c/project/`、`project/python/project/` 已扁平化）。

---

## 命名规则

| 对象 | 规则 | 示例 |
|------|------|------|
| 语言目录 | 小写，固定为 `c` / `cpp` / `python` | `project/cpp/` |
| 项目目录 | **snake_case**：全小写英文 + 下划线，**单数**，不用空格/连字符/大写 | `double_pendulum/`、`ideal_gas/` |
| 项目入口文件 | 统一 `main.c` / `main.cpp` / `main.py` | `main.py` |
| 单文件项目的源文件 | 与项目同名的 snake_case | `double_pendulum.c` |
| C/C++ 功能源文件 | snake_case，与同名头文件配对 | `bisection.c` + `bisection.h` |
| C++ 多文件项目布局 | `include/` `src/` `build/`；头文件按类名 PascalCase | `src/Physics/DoublePendulum.cpp` + `include/Physics/DoublePendulum.h` |
| 项目文档 | 统一 `document.md`，放在项目根 | `project/c/ideal_gas/document.md` |
| 笔记章节目录 | **Title Case（可含空格）**，与教材章节名一致，便于阅读 | `Monte Carlo Simulations/` |
| 笔记主文件 | 统一 `note.md`（Typora 风格） | `notes/error/error.md` |
| 笔记图片资源目录 | 与主文件配套的 `note.assets/` | `note.assets/截屏2026-06-01.png` |
| 笔记内代码子目录 | snake_case | `search_algorithm/`、`data_fitting/` |
| 仓库级文档 | `docs/` 下 snake_case | `docs/naming_conventions.md` |
| 仓库级手册类笔记 | `notes/` 根下 PascalCase + 下划线连接 | `C_Python_Workflow.md` |

**为什么项目目录用 snake_case 而笔记目录可以有空格？**

- 项目目录会进入编译命令、Makefile target、脚本路径：空格要求到处加引号，
  且不能直接用作 make 的 target/prerequisite（见根 `Makefile` 的历史注释）。
- 笔记目录只是阅读容器，章节名保持教材原文的可读性更重要。

**同一物理主题在不同语言下的项目目录名必须一致**，
例如三种语言的简谐振动项目都叫 `simple_harmonic_oscillation/`。

---

## 资源与路径引用

- 图片等资源统一放同级的 `note.assets/`（Typora 自动生成）。
- Markdown / 脚本中引用文件一律用**相对路径**，禁止硬编码绝对路径
  （如 `/Users/...`）——绝对路径在本机之外全部失效。
- Python 脚本引用其它项目的数据文件时，用 `pathlib` 基于脚本自身定位：
  ```python
  from pathlib import Path
  csv = Path(__file__).resolve().parents[2] / "c" / "random_walk" / "rw.csv"
  ```

---

## 构建产物

- C / C++ 项目产物统一输出到各项目的 `build/`，已被 `.gitignore` 忽略，不提交。
- **每个项目目录内都有自己的 Makefile**：C / C++ 项目负责编译（目标 `all` /
  `run` / `clean`）；Python 项目提供语法检查（`all`）、运行（`run`）与
  缓存清理（`clean`）。
- 顶层 `Makefile` 递归调用各子项目 Makefile，目标命名 `<语言>-<项目名>`
  （项目名中的下划线写作连字符，如 `make run-cpp-double-pendulum`）。
  新增项目 = 建目录 + 项目 Makefile + 在根 `Makefile` 对应语言列表注册一行。
- 数据文件（`*.csv`）同样不提交。

---

## 新项目 / 新笔记 Checklist

新建项目：

1. 目录：`project/<语言>/<snake_case 项目名>/`
2. 入口：`main.c` / `main.cpp` / `main.py`
3. （可选）`document.md` 记录物理模型、数值方法与运行方式
4. 项目内 `Makefile`（C / C++：编译进 `build/`；Python：`all` / `run` / `clean`），
   并在根 `Makefile` 对应语言列表注册一行
5. （C++ 多文件）`include/` + `src/` 布局，头文件按类名 PascalCase

新建笔记：

1. 目录：`notes/<教材章节 Title Case>/`
2. 主文件：`note.md`，图片自动落入 `note.assets/`
3. 配套代码：章节下建 snake_case 子目录（如 `difference/`），可独立编译

---

## 附：本次整理对照表

2026-08 目录命名统一时的重命名映射（旧 → 新）：

| 旧路径 | 新路径 |
|--------|--------|
| `project/c/project/<各项目>` | `project/c/<各项目>`（去掉多余嵌套） |
| `project/python/project/<各项目>` | `project/python/<各项目>`（去掉多余嵌套） |
| `project/*/simple harmonic oscillation(s)` | `simple_harmonic_oscillation` |
| `project/*/double pendulum` 等 | `double_pendulum`、`damped_oscillation`、`ideal_gas`、`random_walk` |
| `project/c/Spontaneous decay` | `spontaneous_decay` |
| `project/python/spontaneous dacey`（拼写错误） | `spontaneous_decay` |
| `project/c/Reflection of light in a sphere` | `reflection_of_light_in_a_sphere` |
| `random walk/·protein folding`（前导空格） | `random_walk/protein_folding` |
| `notes/Flourier Analyses`（拼写错误） | `notes/Fourier Analyses` |
| `Monte Carlo Simulations.md` 等 | 统一为 `note.md` + `note.assets/` |
| `data fitting`、`search algorithm`、`error assert` | `data_fitting`、`search_algorithm`、`error_assert` |
| `random walk.py`、`x-v.py` | `random_walk.py`、`main.py` |
| 笔记内 `/Users/...` 绝对图片路径 | 相对路径 `note.assets/...` |

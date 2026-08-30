# Python 项目没有编译产物，Makefile 提供统一入口：
#   make        语法检查（py_compile，不执行脚本）
#   make run    运行可视化脚本
#   make clean  清理 __pycache__
# 解释器可用 PY 覆盖，如：make run PY=../../.venv/bin/python
# 入口文件按命名规范应为 main.py，此处沿用历史文件名 random_walk.py

PY     ?= python3
SCRIPT := random_walk.py

all:
	$(PY) -m py_compile $(SCRIPT)

run:
	$(PY) $(SCRIPT)

clean:
	rm -rf __pycache__

.PHONY: all run clean

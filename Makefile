# ── 顶层构建入口 ──────────────────────────────────────────────
# 递归调用各子项目自己的 Makefile（项目目录名为 snake_case，
# recipe 中仍以引号包裹路径，保持稳健）。
#
# 用法：make / make double-pendulum / make run-double-pendulum / make clean / make help

DP_DIR  := project/cpp/double_pendulum
SHO_DIR := project/cpp/simple_harmonic_oscillation

.DEFAULT_GOAL := all

.PHONY: all double-pendulum sho run-double-pendulum run-sho clean help

all: double-pendulum sho

double-pendulum:
	$(MAKE) -C '$(DP_DIR)'

sho:
	$(MAKE) -C '$(SHO_DIR)'

run-double-pendulum:
	$(MAKE) -C '$(DP_DIR)' run

run-sho:
	$(MAKE) -C '$(SHO_DIR)' run

clean:
	$(MAKE) -C '$(DP_DIR)' clean
	$(MAKE) -C '$(SHO_DIR)' clean

help:
	@echo "可用目标:"
	@echo "  all                   构建全部项目（默认）"
	@echo "  double-pendulum       仅构建双摆（C++ / SDL2）"
	@echo "  sho                   仅构建简谐振动（C++ / SDL2）"
	@echo "  run-double-pendulum   构建并运行双摆"
	@echo "  run-sho               构建并运行简谐振动"
	@echo "  clean                 清理全部项目的 build/"
	@echo "  help                  显示本帮助"

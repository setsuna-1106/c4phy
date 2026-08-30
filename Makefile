# ── 顶层构建入口 ──────────────────────────────────────────────
# 递归调用各子项目自己的 Makefile。项目注册在下方三个列表里，
# 新增项目 = 建目录 + 项目内 Makefile + 在对应列表加一行。
#
# 目标命名 <语言>-<项目名>（项目名中的下划线写作连字符），如
# c-double-pendulum / cpp-simple-harmonic-oscillation / python-ideal-gas。
#
# 用法：make / make cpp / make run-cpp-double-pendulum / make clean / make help

C_PROJECTS      := damped_oscillation double_pendulum ideal_gas \
                   perturbed_damped_pendulum random_walk \
                   reflection_of_light_in_a_sphere \
                   simple_harmonic_oscillation spontaneous_decay
CPP_PROJECTS    := double_pendulum simple_harmonic_oscillation
PYTHON_PROJECTS := damped_oscillation ideal_gas perturbed_damped_pendulum \
                   random_walk simple_harmonic_oscillation spontaneous_decay

.DEFAULT_GOAL := all

# ── 由列表自动生成 per-project 目标 ──────────────────────────
# <语言>-<项目> 构建；run-<语言>-<项目> 构建并运行；
# clean-<语言>-<项目> 清理。$(1)=语言 $(2)=连字符名 $(3)=目录名
define PROJECT_rules
$(1)-$(2):
	$$(MAKE) -C 'project/$(1)/$(3)'

run-$(1)-$(2):
	$$(MAKE) -C 'project/$(1)/$(3)' run

clean-$(1)-$(2):
	$$(MAKE) -C 'project/$(1)/$(3)' clean
endef

$(foreach p,$(C_PROJECTS),$(eval $(call PROJECT_rules,c,$(subst _,-,$(p)),$(p))))
$(foreach p,$(CPP_PROJECTS),$(eval $(call PROJECT_rules,cpp,$(subst _,-,$(p)),$(p))))
$(foreach p,$(PYTHON_PROJECTS),$(eval $(call PROJECT_rules,python,$(subst _,-,$(p)),$(p))))

# 与上面生成的目标名保持一致（下划线写作连字符）
C_TARGETS      := $(foreach p,$(C_PROJECTS),c-$(subst _,-,$(p)))
CPP_TARGETS    := $(foreach p,$(CPP_PROJECTS),cpp-$(subst _,-,$(p)))
PYTHON_TARGETS := $(foreach p,$(PYTHON_PROJECTS),python-$(subst _,-,$(p)))

# ── 语言分组与总目标 ─────────────────────────────────────────
all: c cpp python

c:      $(C_TARGETS)
cpp:    $(CPP_TARGETS)
python: $(PYTHON_TARGETS)

clean-c:      $(C_TARGETS:%=clean-%)
clean-cpp:    $(CPP_TARGETS:%=clean-%)
clean-python: $(PYTHON_TARGETS:%=clean-%)

clean: clean-c clean-cpp clean-python

.PHONY: all c cpp python clean clean-c clean-cpp clean-python help \
        $(C_TARGETS) $(CPP_TARGETS) $(PYTHON_TARGETS) \
        $(C_TARGETS:%=run-%) $(CPP_TARGETS:%=run-%) $(PYTHON_TARGETS:%=run-%) \
        $(C_TARGETS:%=clean-%) $(CPP_TARGETS:%=clean-%) $(PYTHON_TARGETS:%=clean-%)

help:
	@echo "可用目标:"
	@echo "  all                构建全部项目（默认；Python 项目只做语法检查）"
	@echo "  c / cpp / python   仅构建某一语言的全部项目"
	@echo "  <语言>-<项目>       构建指定项目，如 make cpp-double-pendulum"
	@echo "  run-<语言>-<项目>   构建并运行指定项目，如 make run-c-double-pendulum"
	@echo "  clean              清理全部项目"
	@echo "  clean-<语言>       仅清理某一语言的项目"
	@echo ""
	@echo "已注册项目（目标名中下划线写作连字符）："
	@$(foreach p,$(C_PROJECTS),echo "  c:      $(p)";)
	@$(foreach p,$(CPP_PROJECTS),echo "  cpp:    $(p)";)
	@$(foreach p,$(PYTHON_PROJECTS),echo "  python: $(p)";)

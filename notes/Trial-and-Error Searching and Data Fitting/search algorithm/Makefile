CC      = gcc
CFLAGS  = -Wall -O2
LDLIBS  = -lm

# ── 可执行目标 ──
all: bisection test_newton magnetization

bisection: main.o bisection.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test_newton: test_newton.o newton.o difference.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

magnetization: magnetization.o bisection.o newton.o difference.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# ── 头文件依赖 ──
main.o:          bisection.h
bisection.o:     bisection.h
newton.o:        newton.h difference.h
difference.o:    difference.h
test_newton.o:   newton.h
magnetization.o: bisection.h newton.h

# ── 清理 ──
.PHONY: clean
clean:
	rm -f bisection test_newton magnetization *.o
	rm -rf *.dSYM

.PHONY: all clean install uninstall

TARGET = i386-aeros
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
AR = $(TARGET)-ar
AS = $(TARGET)-gcc
STRIP = $(TARGET)-strip

MAKEFLAGS =

OPTIMIZATION_LEVEL = 0
FLAGS = -fdiagnostics-color=auto -O$(OPTIMIZATION_LEVEL) -ftree-vectorize
FLAGS += -Wall -Wextra -Wpointer-arith -Winit-self -Wcast-align -Wfloat-equal -Wsystem-headers -Wbad-function-cast -Wcast-align -Wmissing-format-attribute -Wnested-externs -Wdisabled-optimization -Winline -Wunreachable-code -Wswitch-enum
FLAGS += -Werror=implicit -Werror=redundant-decls -Werror=ignored-qualifiers -Werror=double-promotion -Werror=nonnull -Werror=shadow -Werror=missing-braces -Werror=return-type -Werror=incompatible-pointer-types -Werror=int-conversion -Werror=pointer-arith -Werror=discarded-qualifiers -Werror=type-limits -Werror=overflow
FLAGS += -iquote include -I/usr/lib/gcc/$(TARGET)/$(shell $(CC) -v 2>&1|grep version|cut -d' ' -f3)/include
FLAGS += --sysroot=$(SYSROOT)
LDFLAGS = --sysroot=$(SYSROOT)
DEBUGFLAGS = -DDEBUG -g

CFLAGS = $(DEBUGFLAGS) -std=gnu99 $(FLAGS)
ASFLAGS = -c -Iquoteinclude $(DEBUGFLAGS)

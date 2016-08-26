.PHONY: clean install uninstall

TARGET = i386-aeros
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
AR = $(TARGET)-ar
AS = $(TARGET)-gcc
STRIP = $(TARGET)-strip

MAKEFLAGS =

OPTIMIZATION_LEVEL = 0
FLAGS = -fdiagnostics-color=auto -O$(OPTIMIZATION_LEVEL)
FLAGS += -Wall -Wextra -Wpointer-arith -Winit-self -Wcast-align -Wfloat-equal -Wsystem-headers -Wbad-function-cast -Wcast-align -Wmissing-format-attribute -Wnested-externs -Wdisabled-optimization -Winline -Wunreachable-code -Wswitch-enum
FLAGS += -Werror=implicit -Werror=redundant-decls -Werror=ignored-qualifiers -Werror=double-promotion -Werror=nonnull -Werror=shadow -Werror=missing-braces -Werror=return-type

CFLAGS = $(FLAGS) -std=gnu99
ASFLAGS = -c -Iinclude
DEBUGFLAGS = -DDEBUG -g

%.kernel:

.PHONY: all clean install uninstall

TARGET = i386-aeros
CC = $(TARGET)-gcc
CXX = $(TARGET)-g++
LD = $(TARGET)-ld
AR = $(TARGET)-ar
AS = $(TARGET)-gcc
STRIP = $(TARGET)-strip

MAKEFLAGS =

OPTIMIZATION_LEVEL = 0

CPPFLAGS = -fdiagnostics-color=auto -O$(OPTIMIZATION_LEVEL) -ftree-vectorize
CPPFLAGS += -Wall -Wextra -Wpointer-arith -Winit-self -Wcast-align -Wfloat-equal -Wsystem-headers -Wcast-align -Wmissing-format-attribute -Wdisabled-optimization -Winline -Wunreachable-code -Wswitch-enum
CFLAGS = -Wbad-function-cast -Wnested-externs
CPPFLAGS += -Werror=implicit -Werror=redundant-decls -Werror=ignored-qualifiers -Werror=double-promotion -Werror=nonnull -Werror=shadow -Werror=missing-braces -Werror=return-type -Werror=incompatible-pointer-types
CPPFLAGS += -nostdinc -I/usr/lib/gcc/$(TARGET)/$(shell $(CC) -v 2>&1|grep version|cut -d' ' -f3)/include
CFLAGS += -std=gnu99

DEBUGFLAGS = -DDEBUG -g

CPPFLAGS += $(DEBUGFLAGS)

ASFLAGS = -c -Iinclude $(DEBUGFLAGS)

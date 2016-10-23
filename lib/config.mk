-include ../../config.mk

SYSROOT = $(abspath $(CURDIR)/../../sysroot)

C_FILES = $(shell find -type f -name '*.c' -print)
CC_FILES = $(shell find -type f -name '*.cc' -print)
S_FILES = $(shell find -type f -name '*.S' -print)
OBJECTS = $(C_FILES:.c=.o) $(CC_FILES:.cc=.o) $(S_FILES:.S=.o)

LIBNAME = $(lastword $(subst /, , $(CURDIR)))

CPPFLAGS += -I$(PWD)/include -ffreestanding -fbuiltin

all:: lib$(LIBNAME).a

%.a: $(OBJECTS)
	$(AR) rcs $@ $?

depends:
	makedepend -- -Y$(SYSROOT)/usr/include -I$(shell $(CC) --print-search-dir|grep install|cut -d':' -f2)include $(CFLAGS) -- $(C_FILES) $(CC_FILES)

install:: all
	@mkdir -p $(SYSROOT)/usr/lib
	@cp -uv lib$(LIBNAME).a $(SYSROOT)/usr/lib

uninstall::
	@rm -fv $(SYSROOT)/usr/lib/lib$(LIBNAME).a

clean::
	@rm -fv lib$(LIBNAME).a
	@rm -fv $(OBJECTS)

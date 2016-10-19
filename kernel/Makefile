SYSROOT = $(CURDIR)/../sysroot

-include ../config.mk

C_FILES = $(shell find -name modules -prune -o -type f -name '*.c' -print)
CC_FILES = $(shell find -name modules -prune -o -type f -name '*.cc' -print)
S_FILES = $(shell find -name modules -prune -o -type f -name '*.S' -print)
OBJECTS = $(C_FILES:.c=.o) $(CC_FILES:.cc=.o) $(S_FILES:.S=.o)

CPPFLAGS += -I$(SYSROOT)/usr/include -Iinclude -ffreestanding -fbuiltin
LDFLAGS += -nostdlib -lgcc

all: aeros-i686.kernel

depends:
	makedepend -- -Y$(SYSROOT)/usr/include -I$(shell $(CC) --print-search-dir|grep install|cut -d':' -f2)include $(CFLAGS) -- $(C_FILES) $(CC_FILES)

kernel.o: CFLAGS += -DMULTIBOOT1

boot.o: ASFLAGS += -DMULTIBOOT1

aeros-i686.kernel: $(OBJECTS) linker.ld $(SYSROOT)/usr/lib/libstdc++.a
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) -lstdc++ -Wl,-d -T linker.ld -ffreestanding -fbuiltin

install: all
	@mkdir -p $(SYSROOT)/boot/
	@cp -uv aeros-i686.kernel $(SYSROOT)/boot/
uninstall:
	@rm -fv $(SYSROOT)/boot/aeros-i686.kernel

clean:
	@rm -fv aeros-i686.kernel
	@rm -fv $(OBJECTS)

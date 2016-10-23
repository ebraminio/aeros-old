SYSROOT = $(abspath ../sysroot)

C_FILES = $(shell find -name modules -prune -o -type f -name '*.c' -print)
S_FILES = $(shell find -name modules -prune -o -type f -name '*.S' -print)
OBJECTS = $(C_FILES:.c=.o) $(S_FILES:.S=.o)

-include ../config.mk

CFLAGS += -I$(PWD)/include -ffreestanding -fbuiltin
LDFLAGS += -nostdlib -lgcc -lk

all: aeros-i686.kernel

depends:
	makedepend -- -Y$(SYSROOT)/usr/include -I$(shell $(CC) --print-search-dir|grep install|cut -d':' -f2)include $(CFLAGS) -- $(C_FILES)

kernel.o: CFLAGS += -DMULTIBOOT1

boot.o: ASFLAGS += -DMULTIBOOT1

aeros-i686.kernel: $(OBJECTS) linker.ld $(SYSROOT)/usr/lib/libc.a
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) -Wl,-d -T linker.ld -ffreestanding -fbuiltin

install: all
	@mkdir -p $(SYSROOT)/boot/
	@cp -uv aeros-i686.kernel $(SYSROOT)/boot/
uninstall:
	@rm -fv $(SYSROOT)/boot/aeros-i686.kernel

clean:
	@rm -fv aeros-i686.kernel
	@rm -fv $(OBJECTS)

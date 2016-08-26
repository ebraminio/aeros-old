SYSROOT = $(shell realpath ../sysroot)

C_FILES = $(shell find -name modules -prune -o -type f -name '*.c' -print)
S_FILES = $(shell find -name modules -prune -o -type f -name '*.S' -print)
OBJECTS = $(C_FILES:.c=.o) $(S_FILES:.S=.o)

-include ../config.mk

CFLAGS += -I$(shell pwd)/include -ffreestanding -fbuiltin
LDFLAGS += -nostdlib -L$(KERNEL_DIR)/ -lgcc

kernel.o: CFLAGS += -DMULTIBOOT1

boot.o: ASFLAGS += -DMULTIBOOT1

all: aeros-i686.kernel

aeros-i686.kernel: $(OBJECTS) linker.ld
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) -Wl,-d -T linker.ld -ffreestanding -fbuiltin

install: all
	@mkdir -p $(SYSROOT)/boot/
	@cp -uv aeros-i686.kernel $(SYSROOT)/boot/
uninstall:
	@rm -fv $(SYSROOT)/boot/aeros-i686.kernel

clean:
	@rm -fv aeros-i686.kernel
	@rm -fv $(OBJECTS)

$(shell grep "TARGET =\|PHONY" config.mk)

.PHONY: kernel hdd qemu qemu-hdd travis uncrustify bochs

SYSROOT = $(shell pwd)/sysroot
QEMUFLAGS = -vga std -serial mon:stdio -soundhw pcspk -net dump,file=netdump.pcap -net nic,model=e1000 -net user,hostfwd=tcp::5555-:5555 -s -S

.DELETE_ON_ERROR: hdd.img

qemu: hdd.img $(SYSROOT)/boot/aeros-i686.kernel
	qemu-system-i386 $(QEMUFLAGS) -hda hdd.img -kernel kernel/aeros-i686.kernel

qemu-hdd: hdd.img
	qemu-system-i386 $(QEMUFLAGS) -hda hdd.img

travis:
	@echo [TRAVIS] Removing \$$HOME/local ...
	@echo [TRAVIS] Testing binutils deb build...
	rm -rf $$HOME/local/*
	make -C toolchain binutils-deb
	@echo [TRAVIS] Binutils deb build OK
	@echo [TRAVIS] Installing binutils in \$$HOME/local prefix...
	rm -rfv toolchain/build-binutils/Makefile
	make -C toolchain binutils-unpatch
	make -C toolchain binutils-install PREFIX="$$HOME/local"
	@echo [TRAVIS] Binutils install OK
	@echo [TRAVIS] Testing GCC deb build...
	make -C toolchain gcc-deb
	@echo [TRAVIS] GCC deb build OK
	@echo [TRAVIS] Installing GCC in \$$HOME/local prefix...
	make -C toolchain/build-gcc distclean
	rm -rfv toolchain/build-gcc/Makefile
	make -C toolchain gcc-unpatch
	make -C toolchain gcc-install PREFIX="$$HOME/local"
	@echo [TRAVIS] GCC install OK

uncrustify: uncrustify.cfg
	find kernel lib/c lib/stdc++ sysroot/usr/include -type f -name '*.c' -o -name '*.h' ! -name 'multiboot.h' ! -name 'multiboot2.h' ! -name 'elf.h' | $@ -c $< -F - --replace --no-backup
	sed -i 's/) ;/);/g' $$(grep ' ;' -rl kernel lib/c lib/stdc++)

aeros-i386.sym: $(SYSROOT)/boot/aeros-i686.kernel
	nm $< | grep -i " t " | awk '{print $$1" "$$3}' > $@

bochs: bochsrc.bxrc aeros-i386.sym
	$@ -f $<

hdd: hdd.img sysroot/
	kpartx -av $<
	mkdir -p /media/loop
	sleep 2
	mount /dev/mapper/loop0p1 /media/loop
	@# Touch these files to make sure next 'rm' get at one thing to do
	@touch /media/loop/a /media/loop/boot/a
	rm -r $$(find /media/loop/ -mindepth 1 -maxdepth 1 ! -name boot)
	rm -r $$(find /media/loop/boot/ -mindepth 1 -maxdepth 1 ! -name grub)
	cp -r $(SYSROOT)/* /media/loop/
	umount /media/loop
	kpartx -dv $<

hdd.img:
	dd if=/dev/zero of=$@ bs=1M count=34
	@echo 'n\n\n\n\n\nt\nc\na\nw\n' | fdisk $@ --color=auto
	losetup /dev/loop0 $@
	kpartx -av /dev/loop0
	sleep 2
	mkfs.fat -F32 /dev/mapper/loop0p1 -n AerOS
	mkdir -p /media/loop
	mount /dev/mapper/loop0p1 /media/loop
	grub-install /dev/loop0 --root-directory=/media/loop --target=i386-pc
	umount /media/loop
	kpartx -dv /dev/loop0
	losetup -d /dev/loop0
	@if [ -n "$$SUDO_USER" ]; then chown "$$SUDO_USER" $@; fi

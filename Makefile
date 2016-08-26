include config.mk

.PHONY: kernel hdd qemu toolchain

SYSROOT = $(shell pwd)/sysroot
QEMUFLAGS = -vga std -monitor stdio -net dump,file=netdump.pcap -net nic,model=e1000 -net user,hostfwd=tcp::5555-:5555 -s -S

.PHONY: qemu
.DELETE_ON_ERROR: hdd.img

qemu: hdd.img $(SYSROOT)/boot/aeros-i686.kernel
	qemu-system-i386 $(QEMUFLAGS) -hda hdd.img -kernel kernel/aeros-i686.kernel

hdd: hdd.img sysroot/
	kpartx -av hdd.img
	mkdir -p /media/loop
	sleep 2
	mount /dev/mapper/loop0p1 /media/loop
	cp -r $(SYSROOT)/* /media/loop/
	umount /media/loop
	kpartx -dv hdd.img

hdd.img:
	dd if=/dev/zero of=hdd.img bs=1M count=34
	@echo 'n\n\n\n\n\nt\nc\na\nw\n' | fdisk hdd.img --color=auto
	kpartx -av hdd.img
	sleep 2
	mkfs.fat -F32 /dev/mapper/loop0p1 -n AerOS
	kpartx -dv hdd.img
	@if [ -n "$$SUDO_USER" ]; then chown "$$SUDO_USER" hdd.img; fi

grub: hdd.img
	losetup /dev/loop0 hdd.img
	kpartx -av /dev/loop0
	mkdir -p /media/loop
	mount /dev/mapper/loop0p1 /media/loop
	grub-install /dev/loop0 --root-directory=/media/loop --target=i386-pc
	umount /media/loop
	kpartx -dv /dev/loop0
	losetup -d /dev/loop0

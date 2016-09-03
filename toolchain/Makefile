-include ../config.mk

SYSROOT = $(shell pwd)/../sysroot

BINUTILS := $(shell command -v $(TARGET)-as 2> /dev/null)

BINUTILS_VERSION = 2.27
GCC_VERSION = 6.1.0
ISL_VERSION = 0.16.1

BINUTILS_PACKAGE = $(TARGET)-binutils-$(BINUTILS_VERSION)-$(PACKAGE_VERSION)
GCC_PACKAGE = $(TARGET)-gcc-$(GCC_VERSION)-$(PACKAGE_VERSION)

PACKAGE_VERSION= 0.1-$(shell date +'%Y%m%d')

LINTIAN_FLAGS = --suppress-tags embedded-library,no-copyright-file,extended-description-is-empty,missing-depends-line

.SECONDARY: dist/$(TARGET)-binutils-$(BINUTILS_VERSION).deb binutils-$(BINUTILS_VERSION)/ binutils-$(BINUTILS_VERSION).tar.gz isl-$(ISL_VERSION)/ isl-$(ISL_VERSION).tar.bz2 gcc-$(GCC_VERSION)/ gcc-$(GCC_VERSION).tar.gz
.SUFFIXES:

all:

binutils-%.tar.gz:
	wget -c ftp://ftp.gnu.org/gnu/binutils/$@

gcc-%.tar.gz:
	wget -c ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@

isl-%.tar.bz2:
	wget -c ftp://gcc.gnu.org/pub/gcc/infrastructure/$@

binutils-%/: binutils-%.tar.gz
	tar xzf $<

gcc-%/: gcc-%.tar.gz
	tar xzf $<

isl-%/: isl-%.tar.bz2
	tar xjf $<

binutils-$(BINUTILS_VERSION)/isl: isl-$(ISL_VERSION) binutils-$(BINUTILS_VERSION)/
	ln -s ../$< $@

binutils-configure: binutils-$(BINUTILS_VERSION)/ binutils-$(BINUTILS_VERSION)/isl patches/binutils-$(BINUTILS_VERSION).patch
ifeq ($(NO_AUTOMAKE),1)
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION)-automake.patch
else
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION).patch
		cd binutils-$(BINUTILS_VERSION)/ld && AUTOCONF=autoconf2.64 automake-1.11
endif
	mkdir -p build-binutils
	cd build-binutils && ../binutils-$(BINUTILS_VERSION)/configure --target=$(TARGET) --prefix=/usr --disable-nls --disable-werror --with-isl

build-binutils/Makefile:
	make binutils-configure

binutils-make: build-binutils/Makefile
	cd build-binutils && make

binutils-install: binutils-make
	cd build-binutils && make install

dist/$(BINUTILS_PACKAGE).deb:
	make binutils-deb

binutils-deb: binutils-make
	mkdir -p dist/$(BINUTILS_PACKAGE)/DEBIAN
	cd build-binutils && make install-strip DESTDIR="$(shell pwd)/dist/$(BINUTILS_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/binutils/g' | sed 's/\$$SOFT_VERSION\$$/$(BINUTILS_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(BINUTILS_PACKAGE)/DEBIAN/control
	find dist/$(BINUTILS_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	rm -r dist/$(BINUTILS_PACKAGE)/usr/share/info
	fakeroot dpkg-deb -b dist/$(BINUTILS_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(BINUTILS_PACKAGE).deb

binutils-install-deb: dist/$(BINUTILS_PACKAGE).deb
	dpkg -i $<

gcc-configure: gcc-$(GCC_VERSION)/ $(SYSROOT)/usr/include/ patches/gcc-$(GCC_VERSION).patch
ifndef BINUTILS
	$(MAKE) -C . binutils-install
endif
ifeq ($(NO_AUTOMAKE),1)
	patch -p0 -N -r- < patches/gcc-$(GCC_VERSION)-automake.patch
else
	patch -p0 -N -r- < patches/gcc-$(GCC_VERSION).patch
	cd gcc-$(GCC_VERSION)/libstdc++-v3 && autoconf2.64
endif
	cd gcc-$(GCC_VERSION) && ./contrib/download_prerequisites
	mkdir -p build-gcc
	cd build-gcc && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-sysroot="$(SYSROOT)" --prefix=/usr --enable-languages=c --disable-nls --disable-werror --with-isl

build-gcc/Makefile:
	make gcc-configure

gcc-make: build-gcc/Makefile $(SYSROOT)/usr/include/
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc

gcc-install: gcc-make
	cd build-gcc && make install-gcc
	cd build-gcc && make install-target-libgcc

dist/$(GCC_PACKAGE).deb:
	#make gcc-deb

gcc-deb: gcc-make
	mkdir -p dist/$(GCC_PACKAGE)/DEBIAN
	cd build-gcc && make install-strip-gcc DESTDIR="$(shell pwd)/dist/$(GCC_PACKAGE)"
	cd build-gcc && make install-strip-target-libgcc DESTDIR="$(shell pwd)/dist/$(GCC_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/gcc/g' | sed 's/\$$SOFT_VERSION\$$/$(GCC_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(GCC_PACKAGE)/DEBIAN/control
	echo Depends: $(TARGET)-binutils >> dist/$(GCC_PACKAGE)/DEBIAN/control
	rm -r dist/$(GCC_PACKAGE)/usr/share/man/man7
	find dist/$(GCC_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	fakeroot dpkg-deb -b dist/$(GCC_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(GCC_PACKAGE).deb

gcc-install-deb: dist/$(GCC_PACKAGE).deb
	dpkg -i $<

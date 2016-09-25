-include ../config.mk

SYSROOT = $(shell cd ../sysroot && pwd)

BINUTILS := $(shell command -v $(TARGET)-as 2> /dev/null)

BINUTILS_VERSION = 2.27
GCC_VERSION = 6.1.0
ISL_VERSION = 0.16.1

BINUTILS_PACKAGE = $(TARGET)-binutils-$(BINUTILS_VERSION)-$(PACKAGE_VERSION)
GCC_PACKAGE = $(TARGET)-gcc-$(GCC_VERSION)-$(PACKAGE_VERSION)

PACKAGE_VERSION= 0.1-$(shell date +'%Y%m%d')

LINTIAN_FLAGS = --suppress-tags embedded-library,no-copyright-file,extended-description-is-empty,missing-depends-line

.SUFFIXES:

all:

binutils-%.tar.gz:
	wget -nv -c ftp://ftp.gnu.org/gnu/binutils/$@

gcc-%.tar.gz:
	wget -nv -c ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@

binutils-%/configure: binutils-%.tar.gz
	tar xzf $<

gcc-%/configure: gcc-%.tar.gz
	tar xzf $<

build-binutils/Makefile: binutils-$(BINUTILS_VERSION)/configure patches/binutils-$(BINUTILS_VERSION).patch
ifeq ($(NO_AUTOMAKE),1)
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION)-automake.patch
else
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION).patch
		cd binutils-$(BINUTILS_VERSION)/ld && AUTOCONF=autoconf2.64 automake-1.11
endif
	mkdir -p build-binutils
	cd build-binutils && ../binutils-$(BINUTILS_VERSION)/configure --target=$(TARGET) --prefix=/usr --with-sysroot=/usr/$(TARGET) --with-build-sysroot="$(SYSROOT)" --disable-nls --disable-werror

binutils-deb: dist/$(BINUTILS_PACKAGE).deb

dist/$(BINUTILS_PACKAGE).deb: build-binutils/Makefile
	cd build-binutils && make
	mkdir -p dist/$(BINUTILS_PACKAGE)/DEBIAN
	cd build-binutils && make install-strip DESTDIR="$(shell pwd)/dist/$(BINUTILS_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/binutils/g' | sed 's/\$$SOFT_VERSION\$$/$(BINUTILS_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(BINUTILS_PACKAGE)/DEBIAN/control
	find dist/$(BINUTILS_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	rm -r dist/$(BINUTILS_PACKAGE)/usr/share/info
	chmod 0755 $$(find dist/$(BINUTILS_PACKAGE)/usr -type d)
	fakeroot dpkg-deb -b dist/$(BINUTILS_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(BINUTILS_PACKAGE).deb

binutils-install-deb: dist/$(BINUTILS_PACKAGE).deb
	dpkg -i $<

build-gcc/Makefile: gcc-$(GCC_VERSION)/configure $(SYSROOT)/usr/include/ patches/gcc-$(GCC_VERSION).patch
ifndef BINUTILS
	$(MAKE) binutils-install
endif
ifeq ($(NO_AUTOMAKE),1)
	patch -p0 -N -r- < patches/gcc-$(GCC_VERSION)-automake.patch
else
	patch -p0 -N -r- < patches/gcc-$(GCC_VERSION).patch
	cd gcc-$(GCC_VERSION)/libstdc++-v3 && autoconf2.64
endif
	cd gcc-$(GCC_VERSION) && ./contrib/download_prerequisites
	mkdir -p build-gcc
	cd build-gcc && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-sysroot=/usr/$(TARGET) --with-build-sysroot="$(SYSROOT)" --prefix=/usr --enable-languages=c --disable-nls --disable-werror --with-isl

gcc-install: build-gcc/Makefile
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc
	cd build-gcc && make install-gcc
	cd build-gcc && make install-target-libgcc

dist/$(GCC_PACKAGE).deb: build-gcc/Makefile
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc
	mkdir -p dist/$(GCC_PACKAGE)/DEBIAN
	cd build-gcc && make install-strip-gcc DESTDIR="$(shell pwd)/dist/$(GCC_PACKAGE)"
	cd build-gcc && make install-strip-target-libgcc DESTDIR="$(shell pwd)/dist/$(GCC_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/gcc/g' | sed 's/\$$SOFT_VERSION\$$/$(GCC_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(GCC_PACKAGE)/DEBIAN/control
	echo Depends: libc6, $(TARGET)-binutils >> dist/$(GCC_PACKAGE)/DEBIAN/control
	rm -r dist/$(GCC_PACKAGE)/usr/share/info
	rm -r dist/$(GCC_PACKAGE)/usr/share/man/man7
	find dist/$(GCC_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	mkdir -p dist/$(GCC_PACKAGE)/usr/$(TARGET)/include
	cp -r $(SYSROOT)/usr/include/* dist/$(GCC_PACKAGE)/usr/$(TARGET)/include
	chmod 0755 $$(find dist/$(GCC_PACKAGE)/usr -type d)
	chmod 0644 dist/$(GCC_PACKAGE)/usr/libexec/gcc/i386-aeros/6.1.0/liblto_plugin.so.0.0.0 $$(find dist/$(GCC_PACKAGE)/usr/lib/gcc/$(TARGET)/$(GCC_VERSION)/include -type f -name '*.h') $$(find dist/$(GCC_PACKAGE)/usr/lib/gcc/$(TARGET)/$(GCC_VERSION)/include-fixed -type f -name '*.h') $$(find dist/$(GCC_PACKAGE)/usr/$(TARGET)/include -type f -name '*.h')
	fakeroot dpkg-deb -b dist/$(GCC_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(GCC_PACKAGE).deb

gcc-deb: dist/$(GCC_PACKAGE).deb

gcc-install-deb: dist/$(GCC_PACKAGE).deb
	dpkg -i $<

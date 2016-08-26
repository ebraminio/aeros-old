-include ../config.mk

SYSROOT = $(shell pwd)/../sysroot

BINUTILS_VERSION = 2.27
GCC_VERSION = 6.1.0

BINUTILS_PACKAGE = $(TARGET)-binutils-$(BINUTILS_VERSION)-$(PACKAGE_VERSION)
GCC_PACKAGE = $(TARGET)-gcc-$(GCC_VERSION)-$(PACKAGE_VERSION)

PACKAGE_VERSION= 0.1-$(shell date +'%Y%m%d')

LINTIAN_FLAGS = --suppress-tags embedded-library,no-copyright-file,extended-description-is-empty,missing-depends-line

.SECONDARY: binutils-$(BINUTILS_VERSION)/ gcc-$(GCC_VERSION)/

binutils-%.tar.gz:
	wget -c ftp://ftp.gnu.org/gnu/binutils/$@

gcc-%.tar.gz:
	wget -c ftp://ftp.gnu.org/gnu/gcc/$@

binutils-%/: binutils-%.tar.gz
	tar xzf $<

gcc-%/: gcc-%.tar.gz
	tar xzf $<

%-patch: patches/%.patch %/
	patch -p0 -N  -r- < $<

binutils-configure: binutils-$(BINUTILS_VERSION)-patch
	cd binutils-$(BINUTILS_VERSION)/ld && AUTOCONF=autoconf2.64 automake-1.11
	mkdir -p build-binutils
	cd build-binutils && ../binutils-$(BINUTILS_VERSION)/configure --target=$(TARGET) --prefix=/usr --disable-nls --disable-werror

binutils-make: build-binutils/
	cd build-binutils && make

binutils-install: build-binutils/
	cd build-binutils && make install

binutils-deb: binutils-make
	mkdir -p dist/$(BINUTILS_PACKAGE)/debian
	cd build-binutils && make install-strip DESTDIR="$(shell pwd)/dist/$(BINUTILS_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/binutils/g' | sed 's/\$$SOFT_VERSION\$$/$(BINUTILS_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(BINUTILS_PACKAGE)/debian/control
	find dist/$(BINUTILS_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	rm -r dist/$(BINUTILS_PACKAGE)/usr/share/info
	fakeroot dpkg-deb -b dist/$(BINUTILS_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(BINUTILS_PACKAGE).deb

gcc-configure: gcc-$(GCC_VERSION)-patch $(SYSROOT)/usr/include/
	cd gcc-$(GCC_VERSION)/libstdc++-v3 && autoconf2.64
	mkdir -p build-gcc
	cd build-gcc && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-sysroot="$(SYSROOT)" --prefix=/usr --enable-languages=c --disable-nls --disable-werror

gcc-make: build-gcc/ $(SYSROOT)/usr/include/
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc

gcc-install: gcc-make
	cd build-gcc && make install-gcc
	cd build-gcc && make install-target-libgcc

gcc-deb: gcc-make
	mkdir -p dist/$(GCC_PACKAGE)/debian
	cd build-gcc && make install-strip-gcc DESTDIR="$(shell pwd)/ dist/$(GCC_PACKAGE)"
	cd build-gcc && make install-strip-target-libgcc DESTDIR="$(shell pwd)/dist/$(GCC_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/gcc/g' | sed 's/\$$SOFT_VERSION\$$/$(GCC_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(GCC_PACKAGE)/DEBIAN/control
	echo Depends: $(TARGET)-binutils >> dist/$(GCC_PACKAGE)/debian/control
	rm -r dist/$(GCC_PACKAGE)/usr/share/man/man7
	find dist/$(GCC_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	fakeroot dpkg-deb -b dist/$(GCC_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(GCC_PACKAGE).deb

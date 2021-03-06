TARGET = $(shell grep "^TARGET =" ../config.mk| cut -d' ' -f3)

SYSROOT = $(abspath ../sysroot)
PREFIX = /usr

BINUTILS_VERSION = 2.27
GCC_VERSION = 6.1.0
ISL_VERSION = 0.16.1

BINUTILS_PACKAGE = $(TARGET)-binutils-$(BINUTILS_VERSION)-$(PACKAGE_VERSION)
GCC_PACKAGE = $(TARGET)-gcc-$(GCC_VERSION)-$(PACKAGE_VERSION)

PACKAGE_VERSION = 0.1-$(shell date +'%Y%m%d')

BINUTILS_CONFIG_ARGS = --target=$(TARGET) --with-build-sysroot="$(SYSROOT)" --prefix="$(PREFIX)" --disable-nls --disable-werror
GCC_CONFIG_ARGS = --target=$(TARGET) --with-build-sysroot="$(SYSROOT)" --prefix="$(PREFIX)" --enable-languages=c,c++ --disable-nls --disable-werror --with-isl

LINTIAN_FLAGS = --suppress-tags embedded-library,no-copyright-file,extended-description-is-empty,missing-depends-line

.SUFFIXES:

AUTOMAKE = automake-1.11
ifeq ("$(shell which $(AUTOMAKE))", "")
  AUTOMAKE = $(CURDIR)/prefix/bin/automake-1.11
else
  ifneq ($(shell $(AUTOMAKE) --version|head -n1|cut -d'.' -f3), 6)
    AUTOMAKE = $(CURDIR)/prefix/bin/automake-1.11
  endif
endif

all:

automake_present:
ifneq ($(NO_AUTOMAKE),1)
  ifneq ($(shell $(AUTOMAKE) --version|head -n1|cut -d'.' -f3), 6)
	wget -nv -c ftp://ftp.gnu.org/gnu/automake/automake-1.11.6.tar.gz
	tar xzf automake-1.11.6.tar.gz
	mkdir -p build-automake
	cd build-automake && ../automake-1.11.6/configure --prefix="$(CURDIR)/prefix"
	cd build-automake && make && make install
	touch automake_present
  else
	@echo Valid automake version found
	touch automake_present
  endif
else
	@echo Building without automake
endif

binutils-%.tar.gz:
	wget -nv -c ftp://ftp.gnu.org/gnu/binutils/$@

gcc-%.tar.gz:
	wget -nv -c ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@

binutils-%/configure: binutils-%.tar.gz
	tar xzf $<

gcc-%/configure: gcc-%.tar.gz
	tar xzf $<

build-binutils/Makefile: automake_present binutils-$(BINUTILS_VERSION)/configure patches/binutils-$(BINUTILS_VERSION).patch
ifeq ($(NO_AUTOMAKE),1)
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION)-automake.patch
else
		patch -p0 -N -r- < patches/binutils-$(BINUTILS_VERSION).patch
		cd binutils-$(BINUTILS_VERSION)/ld && AUTOCONF=autoconf2.64 $(AUTOMAKE)
endif
	mkdir -p build-binutils
	cd build-binutils && ../binutils-$(BINUTILS_VERSION)/configure $(BINUTILS_CONFIG_ARGS)

binutils-unpatch: patches/binutils-$(BINUTILS_VERSION).patch
ifeq ($(NO_AUTOMAKE),1)
	patch -p0 -N -r- -R < patches/binutils-$(BINUTILS_VERSION)-automake.patch
else
	patch -p0 -N -r- -R < patches/binutils-$(BINUTILS_VERSION).patch
endif

binutils-install: build-binutils/Makefile
	cd build-binutils && make
	cd build-binutils && make install

binutils-deb: dist/$(BINUTILS_PACKAGE).deb

dist/$(BINUTILS_PACKAGE).deb: build-binutils/Makefile
	cd build-binutils && make
	mkdir -p dist/$(BINUTILS_PACKAGE)/DEBIAN
	cd build-binutils && make install-strip DESTDIR="$(CURDIR)/dist/$(BINUTILS_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/binutils/g' | sed 's/\$$SOFT_VERSION\$$/$(BINUTILS_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(BINUTILS_PACKAGE)/DEBIAN/control
	find dist/$(BINUTILS_PACKAGE)/usr/share/man -type f | while read f; do gzip -9 $$f; done
	rm -rfv dist/$(BINUTILS_PACKAGE)/usr/share/info
	chmod 0755 $$(find dist/$(BINUTILS_PACKAGE)/usr -type d)
	fakeroot dpkg-deb -b dist/$(BINUTILS_PACKAGE)
	lintian $(LINTIAN_FLAGS) dist/$(BINUTILS_PACKAGE).deb

binutils-install-deb: dist/$(BINUTILS_PACKAGE).deb
	dpkg -i $<

gcc-unpatch: patches/gcc-$(GCC_VERSION).patch
ifeq ($(NO_AUTOMAKE),1)
	patch -p0 -N -r- -R < patches/gcc-$(GCC_VERSION)-automake.patch
else
	patch -p0 -N -r- -R < patches/gcc-$(GCC_VERSION).patch
endif

build-gcc/Makefile: automake_present gcc-$(GCC_VERSION)/configure $(SYSROOT)/usr/include/ patches/gcc-$(GCC_VERSION).patch
ifeq ("$(shell which $(TARGET)-as)", "")
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
	cd build-gcc && ../gcc-$(GCC_VERSION)/configure $(GCC_CONFIG_ARGS)

gcc-install: build-gcc/Makefile
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc
	cd build-gcc && make install-gcc
	cd build-gcc && make install-target-libgcc

dist/$(GCC_PACKAGE).deb: build-gcc/Makefile
	cd build-gcc && make all-gcc
	cd build-gcc && make all-target-libgcc
	mkdir -p dist/$(GCC_PACKAGE)/DEBIAN
	cd build-gcc && make install-strip-gcc DESTDIR="$(CURDIR)/dist/$(GCC_PACKAGE)"
	cd build-gcc && make install-strip-target-libgcc DESTDIR="$(CURDIR)/dist/$(GCC_PACKAGE)"
	cat control-template | sed 's/\$$TARGET\$$/$(TARGET)/g' | sed 's/\$$SOFT\$$/gcc/g' | sed 's/\$$SOFT_VERSION\$$/$(GCC_VERSION)/g' | sed 's/\$$PACKAGE_VERSION\$$/$(PACKAGE_VERSION)/g' | sed 's/\$$ARCH\$$/amd64/g' > dist/$(GCC_PACKAGE)/DEBIAN/control
	echo Depends: libc6, $(TARGET)-binutils >> dist/$(GCC_PACKAGE)/DEBIAN/control
	rm -rfv dist/$(GCC_PACKAGE)/usr/share/info
	rm -rfv dist/$(GCC_PACKAGE)/usr/share/man/man7
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

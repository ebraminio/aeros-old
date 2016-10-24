C_FILES = $(shell find -type f -name '*.c' -! -name 'crt*.c' -print)
S_FILES = $(shell find -type f -name '*.S' -! -name 'crt*.S' -print)

-include ../config.mk

all:: libk.a crt0.o crti.o crtn.o

libk.a: $(filter-out ./unistd/syscalls.o, $(OBJECTS))
	$(AR) rcs $@ $?

install:: libk.a crt0.o crti.o crtn.o
	@cp -uv $? $(SYSROOT)/usr/lib/

clean::
	@rm -fv libk.a
	@rm -fv crt0.o crti.o crtn.o

-include ../config.mk

all:: libk.a

libk.a: $(filter-out ./unistd/syscalls.o, $(OBJECTS))
	$(AR) rcs $@ $?

install:: libk.a
	@cp -uv $< $(SYSROOT)/usr/lib/

clean::
	@rm -fv libk.a

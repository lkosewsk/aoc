
.PHONY: all clean test

MYLIB:=libaoc.a
all: $(MYLIB)

OBJS=aocinput.o

$(MYLIB): $(OBJS)
	rm -f $@
	$(AR) rcs $@ $^

test: CPPFLAGS+=-DWVTEST_CONFIGURED -DHAVE_VALGRIND_MEMCHECK_H
test: wvtestmain
	./wvtestrun ./wvtestmain

TESTOBJS=$(patsubst %.c,%.o,$(wildcard t/*.c))

wvtestmain: wvtestmain.o wvtest.o $(TESTOBJS) $(MYLIB)

O_LEVEL=-O1
CPPFLAGS=-I.
CFLAGS=$(O_LEVEL)

clean:
	rm -f *.o t/*.o $(MYLIB) wvtestmain

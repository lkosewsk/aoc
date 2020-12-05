
.PHONY: all clean test

MYLIB:=libaoc.a
all: $(MYLIB)

OBJS=aocinput.o board.o

$(MYLIB): $(OBJS)

test: CPPFLAGS+=-DWVTEST_CONFIGURED -DHAVE_VALGRIND_MEMCHECK_H
test: wvtestmain
	./wvtestrun ./wvtestmain

TESTOBJS=$(patsubst %.c,%.o,$(wildcard t/*.c))

libwvtest.a: CPPFLAGS+=-DWVTEST_CONFIGURED -DHAVE_VALGRIND_MEMCHECK_H
libwvtest.a: wvtestmain.o wvtest.o

wvtestmain: libwvtest.a $(TESTOBJS) $(MYLIB)
	$(CC) $(LDFLAGS) $^ -o $@

O_LEVEL=-O1
CPPFLAGS=-I.
CFLAGS=$(O_LEVEL)

clean:
	rm -f *.o t/*.o *.a wvtestmain

%.a:
	rm -f $@
	$(AR) rcs $@ $^

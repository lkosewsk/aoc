
.PHONY: all clean

MYLIB:=libaoc.a
all: $(MYLIB)

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

$(MYLIB): $(OBJS)
	rm -f $@
	$(AR) rcs $@ $^

O_LEVEL=-O1
CFLAGS=$(O_LEVEL)

clean:
	rm -f $(OBJS) $(MYLIB)

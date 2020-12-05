
.PHONY: all test clean force

LIBAOCTARGET=../../libaoc/libaoc.a
CPPFLAGS=-I../../libaoc
LDFLAGS=-L../../libaoc -laoc

CFLAGS=-O1 -Wall

PROG=main

all: $(PROG)

test: $(PROG)
	./$(PROG)

OBJS=$(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))

$(PROG): main.o $(OBJS) $(LIBAOCTARGET)
	$(CC) -o $@ $(filter-out $(LIBAOCTARGET),$^) $(LDFLAGS)

$(LIBAOCTARGET): force
	$(MAKE) -C ../../libaoc

force:;

clean:
	rm -f $(PROG) main.o $(OBJS)

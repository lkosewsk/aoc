
.PHONY: all test clean

LIBAOCTARGET=../../libaoc/libaoc.a
CPPFLAGS=-I../../libaoc
LDFLAGS=-L../../libaoc -laoc

CFLAGS=-O1 -Wall

PROG=main

all: $(PROG)

test: $(PROG)
	./$(PROG)

OBJS=$(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))

$(PROG): main.o $(OBJS) .stamp-libaoc
	$(CC) -o $@ $(filter-out .stamp-%,$^) $(LDFLAGS)

.stamp-libaoc: $(LIBAOCTARGET)
	touch $@

$(LIBAOCTARGET):
	$(MAKE) -C ../../libaoc

clean:
	rm -f $(PROG) main.o $(OBJS) .stamp-libaoc

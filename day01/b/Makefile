
.PHONY: all clean

CPPFLAGS=-I../../libaoc
LDFLAGS=-L../../libaoc -laoc

CFLAGS=-O1

all: fuel_counter

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

fuel_counter: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f fuel_counter $(OBJS)

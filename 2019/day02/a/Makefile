
.PHONY: all clean

CPPFLAGS=-I../../libaoc
LDFLAGS=-L../../libaoc -laoc

CFLAGS=-O1

PROG=computer

all: $(PROG)

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROG) $(OBJS)

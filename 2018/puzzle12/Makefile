
ifndef DEBUG
	CFLAGS=-O3
endif

part1: part1.o

part2: part2.o
	$(CC) -o $@ $^ $(CFLAGS)

%: %.o
	$(CC) -o $@ $^ $(CFLAGS)
%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

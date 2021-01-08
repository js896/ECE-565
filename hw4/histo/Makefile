# Mingxiang Dai (md382), Jiawei Sun (js896)

ALL=histo histo_locks histo_atomic histo_creative
CC = gcc
CFLAGS = -fno-stack-protector -O3 -fopenmp -o
RULES=$@ $^


all: $(ALL)

histo: histo.c
	$(CC) $(CFLAGS) $(RULES)

histo_locks: histo_locks.c
	$(CC) $(CFLAGS) $(RULES)

histo_atomic: histo_atomic.c
	$(CC) $(CFLAGS) $(RULES)

histo_creative: histo_creative.c
	$(CC) $(CFLAGS) $(RULES)

clean:
	rm $(ALL)





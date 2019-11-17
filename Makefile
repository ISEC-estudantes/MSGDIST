CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o msgdist main.c
stable:clean
	$(CC) $(CFLAGS) -o msgdist main.c
clean:
	rm -vfr *~ msgdist

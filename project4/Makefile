CC = gcc
CFLAGS = -Wall -std=c99

all: controller compute

controller: controller.c sig.h
	$(CC) $(CFLAGS) -o controller controller.c

compute: compute.c sig.h
	$(CC) $(CFLAGS) -o compute compute.c

clean:
	rm -f controller compute
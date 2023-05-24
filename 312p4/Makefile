CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

all: controller

controller: controller.o synchronizer.o compute.o
    $(CC) $(CFLAGS) -o controller controller.o synchronizer.o

controller.o: controller.c synchronizer.h
    $(CC) $(CFLAGS) -c controller.c

synchronizer.o: synchronizer.c synchronizer.h
    $(CC) $(CFLAGS) -c synchronizer.c

compute.o: compute.c synchronizer.h
    $(CC) $(CFLAGS) -c compute.c

clean:
    rm -f controller *.o
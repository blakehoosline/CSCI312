all: ServerC ServerG client

client: client.c
    $(CC) $(CFLAGS) -o client client.c

ServerC: ServerC.c
    $(CC) $(CFLAGS) -o ServerC ServerC.c

ServerG: ServerG.c
    $(CC) $(CFLAGS) -o ServerG ServerG.c

clean:
    rm -f ServerC
    rm -f ServerG
    rm -f client

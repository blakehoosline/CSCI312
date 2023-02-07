all: interface db

interface: interface.c
	gcc interface.c -o interface -std=c99

db: db.c
	gcc db.c -o db -std=c99

run: interface
	./interface

clean:
	rm -f interface db
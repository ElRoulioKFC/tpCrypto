CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lcrypto -lssl -lm

all: exec

exec: main.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

run: exec
	./exec $(ARGS)

clean:
	rm -f exec

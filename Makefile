CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: clean

all: lanParty

lanParty: main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f lanParty *.o

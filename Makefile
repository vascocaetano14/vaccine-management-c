CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -Wno-unused-result

proj: main.c proj.h
	$(CC) $(CFLAGS) -o proj main.c

run: proj
	./proj < examples/example.in

clean:
	rm -f proj

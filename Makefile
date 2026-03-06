CC = g++
CFLAGS = -Wall -Wextra -Werror -g

SRC = src/main.cpp

all: build

build: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o calc

run: build
	./calc

clean:
	rm -f test
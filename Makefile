CC = g++
CFLAGS = -Wall -Wextra -Werror -g

SRC = src/*.cpp src/*.c
INCLUDE = -Idependencies/include
LIB = -Ldependencies/lib -l:libglfw3.a -l:libglm.a

all: run

build: $(SRC)
	$(CC) $(SRC) $(INCLUDE) $(LIB) -o calc

run: build
	./calc

clean:
	rm -f calc
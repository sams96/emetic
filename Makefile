CC = gcc
CSTD = c99

CFLAGS =  -Wall -g -Werror -std=$(CSTD) $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lGL -lGLEW

MAIN = main.c

game: main.o
	$(CC) $(LDFLAGS) -o game main.o
	rm main.o

main.o: $(MAIN)
	$(CC) $(CFLAGS) -c $(MAIN)

clean:
	rm main.o game
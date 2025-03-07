build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -g -lm -o renderer

run:
	./renderer

clean:
	rm renderer


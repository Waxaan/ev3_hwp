all: main.o
	gcc main.o -o bin
	
main.o: main.c
	gcc -I . -c main.c -lev3dev-c

#gcc hello.c -lev3dev-c -o hello

clean:
	rm -rf *.o
	rm bin
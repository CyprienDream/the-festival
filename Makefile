all: p23
main.o: main.c
	gcc -c main.c
linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c
test1: main.o linkedlist.o
	 gcc main.o linkedlist.o -o p23.exe

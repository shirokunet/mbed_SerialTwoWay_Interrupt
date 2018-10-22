CC := gcc -g

main: main.o serial.o
	g++ -Wall -o main main.o serial.o 
serial.o: serial.c serial.h
	g++ -Wall -c serial.c serial.h 
main.o: main.cpp
	g++ -Wall -c main.cpp
clean:
	rm -f *.o main


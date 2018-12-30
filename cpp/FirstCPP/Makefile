# Declare some variables to help construct our compile command
CC=g++         # the compiler
LIBS=-lm       # the libraries we will ref

menu: Menu.o
	$(CC) -o Menu Menu.o Database.o -std=c++11

Menu.o: Menu.cpp Database.o
	$(CC) -c Menu.cpp -std=c++11

Database.o: Database.h Database.cpp
	$(CC) -c Database.cpp -std=c++11

run: Menu
	./Menu



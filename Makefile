CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable

all: main

main: main.o othello_cut.h
	g++ main.o -o main

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean

clean:
	rm *o main
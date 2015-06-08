CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable

all: main

hash: main_hash

main_hash: main_hash.o othello_cut.h
	g++ main_hash.o -o main_hash

main_hash.o: main_hash.cpp
	g++ -c main_hash.cpp

main: main.o othello_cut.h
	g++ main.o -o main

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean

clean:
	rm *o main
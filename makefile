CC=LANG=end g++
FLAGS=-O3 -ggdb -std=c++1z
all: test

test: const_ton.hpp test.cpp
	$(CC) $(FLAGS) test.cpp -o test

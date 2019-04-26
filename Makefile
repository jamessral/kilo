CC = g++
FLAGS = -std=c++14 -Wall -Werror -Wpedantic
FILES = main.cpp
OUTPUT = kilo

all:
	$(CC) $(FLAGS) -o $(OUTPUT) $(FILES)

clean:
	rm $(OUTPUT)

# Run Project SO

all: compile

compile: main
	gcc main.c -o main -lpthread
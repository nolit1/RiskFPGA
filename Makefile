FPGARisk : main.o
	gcc -o FPGARisk main.o

main.o : main.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o main.o main.c

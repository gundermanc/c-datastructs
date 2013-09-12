#!/bin/sh
gcc -c -o stack.o stack.c
gcc -c -o ll.o ll.c
gcc -c -o sb.o sb.c
ar rcs datastructs.a stack.o ll.o sb.o

gcc -o test_app main.c datastructs.a

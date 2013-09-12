#!/bin/sh
gcc -c -o out.o ll.c
ar rcs datastructs.a out.o

gcc -o test_app main.c datastructs.a

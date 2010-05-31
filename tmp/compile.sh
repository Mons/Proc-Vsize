#!/bin/sh

#gcc -lkvm -o test test.c
gcc -fno-common -ffast-math \
  -W -Wall -Wshadow -Wcast-align -Wredundant-decls \
  -Wbad-function-cast -Wcast-qual -Wwrite-strings -Waggregate-return \
  -Wstrict-prototypes -Wmissing-prototypes -o test test.c

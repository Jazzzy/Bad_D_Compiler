#!/bin/bash

flex d.lex 
gcc lex.yy.c 
./a.out ../../../bin/arguments/regression.d

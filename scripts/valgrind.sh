#!/usr/bin/env bash

if [ $1 = "C" ]  
then  
    /usr/bin/clang -ggdb -include ./symbolTable/symbolTable.h -include ./utils/bHashTable.h  main.c ./symbolTable/symbolTable.c ./utils/bHashTable.c  -Wall -Werror -o program && /usr/bin/valgrind --leak-check=full ./program
elif [ $1 = "Cpp" ]  
then  
    /usr/bin/clang++ -std=c++11 -stdlib=libc++ main.cc -Wall -Werror -o program && /usr/bin/valgrind ./program
fi
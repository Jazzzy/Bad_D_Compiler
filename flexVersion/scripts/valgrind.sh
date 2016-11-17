#!/usr/bin/env bash

if [ $1 = "C" ]  
then  
    /usr/bin/clang -ggdb  -include ./errorManager/errorManager.h -include ./lexicalAnalyzer/lexicalHelper.h -include ./utils/defineParser.h  -include ./lexicalAnalyzer/lexicalAnalyzer.h -include ./readerSystem/readerSystem.h -include ./symbolTable/symbolTable.h -include ./utils/bHashTable.h  main.c ./errorManager/errorManager.c ./lexicalAnalyzer/lexicalHelper.c ./utils/defineParser.c  ./symbolTable/symbolTable.c ./utils/bHashTable.c ./readerSystem/readerSystem.c ./lexicalAnalyzer/lexicalAnalyzer.c  -Wall  -o program && /usr/bin/valgrind --leak-check=full --show-leak-kinds=all  ./program /home/jazzzy/GitProjects/Bad_D_Compiler/flexVersion/bin/arguments/regression.d  /home/jazzzy/GitProjects/Bad_D_Compiler/flexVersion/src/DLang/D_DEFINE_RESERVED_WORDS.h  /home/jazzzy/GitProjects/Bad_D_Compiler/src/DLang/d.ope
elif [ $1 = "Cpp" ]  
then  
    /usr/bin/clang++ -std=c++11 -stdlib=libc++ main.cc -Wall -Werror -o program && /usr/bin/valgrind ./program
fi

rm program
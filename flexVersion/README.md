# Bad D Compiler

In order to compile and run this compiler you just need to go to the /scripts folder located in the root of the project. There you can find three scripts:

1. compile.sh
2. run.sh
3. runValgrind.sh (compiles, runs and checks for memory leaks)

Executing the first two scripts in that order will use the cmakelists.txt file on the /src folder to create a makefile inside the /bin folder. Then it will execute a make in order to compile the whole program. After that, when you execute run.sh you will simply run the program with the arguments provided in the folder /bin/arguments.


Another way to compile it would be to execute on the /src folder this command:

	gcc  main.c  utils/bHashTable.c utils/bHashTable.h symbolTable/symbolTable.c symbolTable/symbolTable.h readerSystem/readerSystem.c readerSystem/readerSystem.h lexicalAnalyzer/lexicalAnalyzer.c lexicalAnalyzer/lexicalAnalyzer.h DLang/D_DEFINE_RESERVED_WORDS.h utils/defineParser.c utils/defineParser.h lexicalAnalyzer/lexicalHelper.c lexicalAnalyzer/lexicalHelper.h DLang/D_DEFINE_NON_RESERVED_WORDS.h errorManager/errorManager.c errorManager/errorManager.h utils/colours.h -o ../bin/bdc

If you want to compile d.lex first you have to execute the next command inside the /src/lexicalAnalyzer/flex folder:

	flex d.lex

And you could execute the whole program (inside the /bin folder) with:

	./bdc ./arguments/regression.d ./arguments/D_DEFINE_RESERVED_WORDS.h ./arguments/d.ope

But the scripts ./run.sh and ./runValgrind.sh that are in the /scripts folder should work too :)

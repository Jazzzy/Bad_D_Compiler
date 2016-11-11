# Bad D Compiler

In order to compile and run this compiler you just need to go to the /scripts folder located in the root of the project. There you can find two scripts:

1. compile.sh
2. run.sh

Executing those two scripts in that order will use the cmakelists.txt file on the /src folder to create a makefile inside the /bin folder. Then it will execute a make in order to compile the whole program. After that, when you execute run.sh you will simply run the program with the arguments provided in the folder /bin/arguments.

Another way to compile it would be to execute on the /src folder this command:

	gcc -Werror -Wall -Wextra main.c utils/bHashTable.c utils/bHashTable.h symbolTable/symbolTable.c symbolTable/symbolTable.h readerSystem/readerSystem.c readerSystem/readerSystem.h lexicalAnalyzer/lexicalAnalyzer.c lexicalAnalyzer/lexicalAnalyzer.h DLang/D_DEFINE_RESERVED_WORDS.h utils/operatorParser.c utils/operatorParser.h utils/defineParser.c utils/defineParser.h lexicalAnalyzer/lexicalHelper.c lexicalAnalyzer/lexicalHelper.h DLang/D_DEFINE_NON_RESERVED_WORDS.h errorManager/errorManager.c errorManager/errorManager.h -o ../bin/bdc

And you could execute it (inside the /bin folder) with:

	./bdc ./arguments/regression.d ./arguments/D_DEFINE_RESERVED_WORDS.h ./arguments/d.ope
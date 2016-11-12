cmake_minimum_required(VERSION 3.6)

project(bdc)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES main.c  utils/bHashTable.c utils/bHashTable.h symbolTable/symbolTable.c symbolTable/symbolTable.h readerSystem/readerSystem.c readerSystem/readerSystem.h lexicalAnalyzer/lexicalAnalyzer.c lexicalAnalyzer/lexicalAnalyzer.h DLang/D_DEFINE_RESERVED_WORDS.h utils/defineParser.c utils/defineParser.h lexicalAnalyzer/lexicalHelper.c lexicalAnalyzer/lexicalHelper.h DLang/D_DEFINE_NON_RESERVED_WORDS.h errorManager/errorManager.c errorManager/errorManager.h utils/colours.h)

add_executable(bdc ${SOURCE_FILES})




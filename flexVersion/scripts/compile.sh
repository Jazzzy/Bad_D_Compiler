#!/usr/bin/env bash

cd ../src/lexicalAnalyzer/flex/
flex d.lex
cd ../../../bin
cmake ../src/
make

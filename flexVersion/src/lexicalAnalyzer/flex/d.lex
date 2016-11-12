/* This is a lex scanner designed for (part of) the D language */

%option noyywrap

%{


#include "../../DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "../../symbolTable/symbolTable.h"

extern symbolTable *global_st;

#define MAX_STR_CONST 1024

int numLine = 1,numCharacter=0;

char array_buf[MAX_STR_CONST];
char *array_buf_ptr;

void addChars();
void readBlockComment();
void readNestedComment(short first);
void updateWith(char c);

%}

  /*--------------DEFINITIONS--------------*/

%x StringMODE

Character .

WhiteSpace [ \t\v\n\f]

BegOfBlockComment "/*"

BegOfDocumentationComment "/**"[^/]

BegOfNestedComment "/+"

BegOfLineComment "//"

StringDelimiter \"

IdentifierStart [[:alpha:]_]

IdentifierChar [[:digit:][:alpha:]_]

Identifier {IdentifierStart}{IdentifierChar}*

CharacterLiteral \'(.|\\n|\\t|\\r|\\b|\\f|\\a|\\v)\'

Digit [0-9]

SciNo [Ee][+-]?{Digit}+


%%		

  /*-----------------RULES-----------------*/
	
	/*COMMENTS*/

{BegOfBlockComment}	{
					readBlockComment();
					}

{BegOfDocumentationComment}	{
					readBlockComment();
					return(DOCUMENTATION_COMMENT);
					}


{BegOfLineComment}.*\n {addChars();}

{BegOfNestedComment}	{
						readNestedComment(1);
						}

	/*IDENTIFIERS OR RESERVED WORDS*/

{Identifier}    {
				//Comprobar si puede ser una palabra reservada.
				addChars();

				symbolData *sd = searchLex(global_st, yytext);

				if (sd == NULL) {
					sd = (symbolData *) malloc(sizeof(symbolData));
                    sd->lexicalComponent = IDENTIFIER;
                    sd->lexeme = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
                    strcpy(sd->lexeme,yytext);
					addLex(&global_st, sd->lexeme, sd);
                    return(IDENTIFIER);
				}else{
					if (sd->lexicalComponent == IDENTIFIER) {
						return(IDENTIFIER);
					}else{
						return(sd->lexicalComponent);
					}
				}


				
				}

	/*INTEGER LITERALS*/

{Digit}({Digit}|_)* 	{
						addChars();
						//printf("Integer Literal: %s\n", yytext);
						return(INTEGER_LITERAL);
						}

	/*BINARY LITERALS*/

0(b|B)(0|1|_)+		{
					addChars();
					//printf("Binary Literal: %s\n", yytext);
					return(BINARY_LITERAL);
					}

	/*FLOATING LITERALS*/

{Digit}*"."{Digit}+({SciNo})?		|
{Digit}+("."{Digit}*)?({SciNo})?	{
									addChars();
									return(FLOAT_LITERAL);
									}


	/*SINGLE CHARACTER LITERALS*/

{CharacterLiteral} {
					addChars();
					//printf("Character Literal: %s\n", yytext);
					return (CHARACTER_LITERAL);			
					}

	/*STRING LITERALS*/
	
{StringDelimiter}	{ 
					//printf("Empezamos a leer string\n", yytext);
					array_buf_ptr= array_buf;
					BEGIN(StringMODE);
					}

<StringMODE>{

{StringDelimiter} {
					BEGIN(INITIAL);
					*array_buf_ptr= '\0';
					addChars();
					//printf("Acabamos: %s\n", array_buf);
					return(STRING_LITERAL);
					}

\n 					{
					addChars();
					manageNonFatalErrorWithLine(ERR_JUMP_LINE_IN_LITERAL, "Found a jump line character inside a string literal", numLine, numCharacter);
					}

\\n 	{
		addChars();
		*array_buf_ptr++ = '\n';
		}


\\t 	{
		addChars();
		*array_buf_ptr++ = '\t';
		}

\\r 	{
		addChars();
		*array_buf_ptr++ = '\r';
		}

\\b 	{
		addChars();
		*array_buf_ptr++ = '\b';
		}

\\f 	{
		addChars();
		*array_buf_ptr++ = '\f';
		}

\\(.|\n) 	{
		addChars();
		*array_buf_ptr++ = yytext[1];
		}

[^\\\n\"]+			{
	char *yptr = yytext;
	while(*yptr)
		*array_buf_ptr++ = *yptr++;
					}
}


	/*OPERATORS*/

"/"		{ addChars();  return('/');}
"/="	{ addChars();  return(OPE_SLASH_EQ);}
"."		{ addChars();  return('.');}
".."	{ addChars();  return(OPE_TWO_POINTS);}
"..."	{ addChars();  return(OPE_THREE_POINTS);}
"&"		{ addChars();  return('&');}
"&="	{ addChars();  return(OPE_AND_EQ);}
"&&"	{ addChars();  return(OPE_AND_AND);}
"|"		{ addChars();  return('|');}
"|="	{ addChars();  return(OPE_VERT_EQ);}
"||"	{ addChars();  return(OPE_VERT_VERT);}
"-"		{ addChars();  return('-');}
"-="	{ addChars();  return(OPE_MINUS_EQ);}
"--"	{ addChars();  return(OPE_MINUS_MINUS);}
"+"		{ addChars();  return('+');}
"+="	{ addChars();  return(OPE_PLUS_EQ);}
"++"	{ addChars();  return(OPE_PLUS_PLUS);}
"<"		{ addChars();  return('<');}
"<="	{ addChars();  return(OPE_LESSTHAN_EQ);}
"<<"	{ addChars();  return(OPE_LESSTHAN_LESSTHAN);}
"<<="	{ addChars();  return(OPE_LESSTHAN_LESSTHAN_EQ);}
"<>"	{ addChars();  return(OPE_LESSTHAN_MORETHAN);}
"<>="	{ addChars();  return(OPE_LESSTHAN_MORETHAN_EQ);}
">"		{ addChars();  return('>');}
">="	{ addChars();  return(OPE_MORETHAN_EQ);}
">>="	{ addChars();  return(OPE_MORETHAN_MORETHAN_EQ);}
">>>="	{ addChars();  return(OPE_MORETHAN_MORETHAN_MORETHAN_EQ);}
">>"	{ addChars();  return(OPE_MORETHAN_MORETHAN);}
">>>"	{ addChars();  return(OPE_MORETHAN_MORETHAN_MORETHAN);}
"!"		{ addChars();  return('!');}
"!="	{ addChars();  return(OPE_EXCL_EQ);}
"!<>"	{ addChars();  return(OPE_EXCL_LESSTHAN_MORETHAN);}
"!<>="	{ addChars();  return(OPE_EXCL_LESSTHAN_MORETHAN_EQ);}
"!<"	{ addChars();  return(OPE_EXCL_LESSTHAN);}
"!<="	{ addChars();  return(OPE_EXCL_LESSTHAN_MORETHAN_EQ);}
"!>"	{ addChars();  return(OPE_EXCL_MORETHAN);}
"!>="	{ addChars();  return(OPE_EXCL_MORETHAN_EQ);}
"("		{ addChars();  return('(');}
")"		{ addChars();  return(')');}
"["		{ addChars();  return('[');}
"]"		{ addChars();  return(']');}
"{"		{ addChars();  return('{');}
"}"		{ addChars();  return('}');}
"?"		{ addChars();  return('?');}
","		{ addChars();  return(',');}
";"		{ addChars();  return(';');}
":"		{ addChars();  return(':');}
"$"		{ addChars();  return('$');}
"="		{ addChars();  return('=');}
"=="	{ addChars();  return(OPE_EQ_EQ);}
"*"		{ addChars();  return('*');}
"*="	{ addChars();  return(OPE_TIMES_EQ);}
"%"		{ addChars();  return('%');}
"%="	{ addChars();  return(OPE_PERC_EQ);}
"^"		{ addChars();  return('^');}
"^="	{ addChars();  return(OPE_HAT_EQ);}
"^^"	{ addChars();  return(OPE_HAT_HAT);}
"^^="	{ addChars();  return(OPE_HAT_HAT_EQ);}
~		{ addChars();  return('~');}
~=		{ addChars();  return(OPE_VIRG_EQ);}
@		{ addChars();  return('@');}
=>		{ addChars();  return(OPE_EQ_MORE);}
#		{ addChars();  return('#');}


	/*SPACES*/

{WhiteSpace}	{addChars();}


<<EOF>>			{return(END_OF_FILE);}

	/*WE DON'T KNOW*/

.	{addChars();}



%%		


  /*---------------USER_CODE---------------*/

void readBlockComment(){

	array_buf_ptr= array_buf;

	char *yptr = yytext;
	while(*yptr){
		*array_buf_ptr++ = *yptr++;
		updateWith(*yptr);
	}

	char c;

	for(;;){
		while( (c=input()) != '*' && c != EOF ){
			*array_buf_ptr++=c;
			updateWith(c);
		}

		*array_buf_ptr++=c;
		updateWith(c);

		if(c == '*'){

			while ( (c=input()) == '*' ){
				*array_buf_ptr++=c;
				updateWith(c);
			}

			*array_buf_ptr++=c;
			updateWith(c);

			if(c == '/'){
				*array_buf_ptr= '\0';
				return;
			}
		}
		if (c == EOF){
			manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a comment", numLine, numCharacter);
			return;
		}
	}

}

void readNestedComment(short first){


	if(first){
		array_buf_ptr= array_buf;
		char *yptr = yytext;
		while(*yptr){
			*array_buf_ptr++ = *yptr++;
			updateWith(*yptr);
		}
	}


	char c;

	for(;;){
		while( (c=input()) != '+' && c != EOF ){
			if(c == '/'){
				*array_buf_ptr++=c;
				updateWith(c);
				while ( (c=input()) == '/' ){
					*array_buf_ptr++=c;
					updateWith(c);
				}

				*array_buf_ptr++=c;
				updateWith(c);

				if(c == '+'){
					readNestedComment(0);
				}
			}else{
				*array_buf_ptr++=c;
				updateWith(c);
			}
		}

		*array_buf_ptr++=c;
		updateWith(c);

		if(c == '+'){
			while ( (c=input()) == '+' ){
				*array_buf_ptr++=c;
				updateWith(c);
			}

			*array_buf_ptr++=c;
			updateWith(c);

			if(c == '/'){
				*array_buf_ptr= '\0';
				return;
			}
		}
		if (c == EOF){
			manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a comment", numLine, numCharacter);
			return;
		}
	}
}

void updateWith(char c){
	if (c == '\n'){
			numCharacter = 0;
			numLine++;
		}else if (c == '\t'){
			numCharacter += 4 - (numCharacter % 4);
		}else{
			numCharacter++;
		}
}

void addChars(){
	int i;
	for (i = 0; yytext[i] != '\0'; i++)
		updateWith(yytext[i]);
}


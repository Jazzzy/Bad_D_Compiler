/* This is a lex scanner designed for (part of) the D language */

%option noyywrap

%{


//#include "../../DLang/D_DEFINE_NON_RESERVED_WORDS.h"

#define MAX_STR_CONST 512

int numLinea = 1,numCharacter=0;


char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

%}

  /*--------------DEFINITIONS--------------*/

%x CommentMODE

%x StringMODE

Character .

WhiteSpace [:blank:]

BegOfBlockComment "/*"

EndOfBlockComment "*/"

BegOfNestedComment "/+"

EndOfNestedComment "+/"

BegOfLineComment "/*"

StringDelimiter \"

IdentifierStart [[:alpha:]_]

IdentifierChar [[:digit:][:alpha:]_]

Identifier {IdentifierStart}{IdentifierChar}*



%%		

  /*-----------------RULES-----------------*/

{WhiteSpace}	

{Identifier}    {

				printf("Detectado identificador! %s\n", yytext);

				}
	
{StringDelimiter}	{ 
					printf("Empezamos a leer string\n", yytext);
					string_buf_ptr= string_buf;
					BEGIN(StringMODE);
					}

<StringMODE>{

{StringDelimiter} {
					BEGIN(INITIAL);
					*string_buf_ptr= '\0';
					printf("Acabamos: %s\n", string_buf);
					}

\n 		{
					printf("ERROR: barraene en una string!");

					}

\\n *string_buf_ptr++ = '\n';

\\t *string_buf_ptr++ = '\t';

\\r *string_buf_ptr++ = '\r';

\\b *string_buf_ptr++ = '\b';

\\f *string_buf_ptr++ = '\f';

\\(.|\n) *string_buf_ptr++ = yytext[1];

[^\\\n\"]+		{
	char *yptr = yytext;
	while(*yptr)
		*string_buf_ptr++ = *yptr++;
					}
}

\n	numLinea++; numCharacter++;

.	numCharacter++;



%%		

  /*---------------USER_CODE---------------*/

void main(int argc, char **argv){

	++argv,	--argc;/* se salta el nombre del programa */
	if ( argc > 0 )
		yyin = fopen( argv[0], "r" );
	else
		yyin = stdin;
	yylex();

	printf("Lineas: %d, Caracteres: %d\n",numLinea, numCharacter);
}
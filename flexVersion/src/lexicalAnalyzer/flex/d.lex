/* This is a lex scanner designed for (part of) the D language */

%option noyywrap

%{


#include "../../DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "../../symbolTable/symbolTable.h"

extern symbolTable *global_st;

int numLine = 1,numCharacter=0;
int nestedLevel = 1;


void addChars();
void updateWith(char c);

%}

  /*--------------DEFINITIONS--------------*/

%x StringMODE
%x DocumentationCommentMODE
%x BlockCommentMODE
%x NestedCommentMODE


Character .

WhiteSpace [ \t\v\n\f]

BegOfBlockComment "/*"

EndOfBlockComment "*/"

BegOfDocumentationComment "/**"[^/]

BegOfNestedComment "/+"

EndOfNestedComment "+/"

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
					BEGIN(BlockCommentMODE);
					yymore();
					}

<BlockCommentMODE>{
	
{EndOfBlockComment}	{
					addChars();
					BEGIN(INITIAL);
					}

<<EOF>>				{
					addChars();
					BEGIN(INITIAL);
					manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a comment", numLine, numCharacter);
					}

.|\n				yymore();

}


{BegOfDocumentationComment}	{
					BEGIN(DocumentationCommentMODE);
					yymore();
					}

<DocumentationCommentMODE>{
	
{EndOfBlockComment}	{
					BEGIN(INITIAL);
					addChars();
					return(DOCUMENTATION_COMMENT);
					}

<<EOF>>				{
					addChars();
					BEGIN(INITIAL);
					manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a comment", numLine, numCharacter);
					yyrestart(yyin);
					}

.|\n				{
					yymore();
					}
}




{BegOfLineComment}.*\n {addChars();}

{BegOfNestedComment}	{
						yymore();
						BEGIN(NestedCommentMODE);
						nestedLevel = 1;
						}

<NestedCommentMODE>{
	
{BegOfNestedComment}	{
						nestedLevel++;
						yymore();
						}

{EndOfNestedComment}	{
						if( (--nestedLevel) == 0){
							addChars();
							BEGIN(INITIAL);
						}else{
							yymore();
							}
						}
						

<<EOF>>               	{
						addChars();
						BEGIN(INITIAL);
						manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a comment", numLine, numCharacter);
						}

.|\n					yymore();

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
						return(INTEGER_LITERAL);
						}

	/*BINARY LITERALS*/

0(b|B)(0|1|_)+		{
					addChars();
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
					return (CHARACTER_LITERAL);			
					}

	/*STRING LITERALS*/
	
{StringDelimiter}	{ 
					BEGIN(StringMODE);
					yymore();
					}

<StringMODE>{

{StringDelimiter} {
					BEGIN(INITIAL);
					addChars();
					return(STRING_LITERAL);
					}

\n 					{
					manageNonFatalErrorWithLine(ERR_JUMP_LINE_IN_LITERAL, "Found a jump line character inside a string literal", numLine, numCharacter);
					yymore();
					}

\\\'				|
\\\"				|
\\\?				|
\\\\				|
\\0					|
\\a					|
\\b					|
\\f					|
\\n					|
\\r					|
\\t					|
\\v 				{
					yymore();
					}

\\.					{
					manageNonFatalErrorWithLine(ERR_SCAPE_CHAR, "Found a malformed scape character", numLine, numCharacter);
					yymore();
					}

. 					{
					yymore();
					}


<<EOF>>		{
			addChars();
			yyrestart(yyin);
			manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "Found end of file inside a literal string", numLine, numCharacter);
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


	/*END OF FILE*/

<<EOF>>			{return(END_OF_FILE);}

	/*SOME OTHER THINGS*/

.	{addChars();}



%%		


  /*---------------USER_CODE---------------*/


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


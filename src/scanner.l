%{
#include "requiredStructs.h"
#include "astclasses.cpp"
#include "parser.tab.h"
%}

%%
"declblock"				{return DECLBLOCK;}
"codeblock"				{return CODEBLOCK;}
"int"					{return DATATYPE;}

"and"	{yylval.namer = strdup(yytext);return AND;}
"or" 	{yylval.namer = strdup(yytext);return OR;}

"for"	{yylval.namer = strdup(yytext);return FOR;}
"while"	{yylval.namer = strdup(yytext);return WHILE;}
"if"	{yylval.namer = strdup(yytext);return IF;}
"else"	{yylval.namer = strdup(yytext);return ELSE;}

"print"		{yylval.namer = strdup(yytext);return PRINT_WITHOUT_NEW_LINE;}
"println"	{yylval.namer = strdup(yytext);return PRINT_WITH_NEW_LINE;}
"read"		{yylval.namer = strdup(yytext);return READ;}
"goto"		{yylval.namer = strdup(yytext);return GOTO;}

"{"		{yylval.namer = strdup(yytext);return '{';}
"}"		{yylval.namer = strdup(yytext);return '}';}
"("		{yylval.namer = strdup(yytext);return '(';}
")"		{yylval.namer = strdup(yytext);return ')';}
"\["	{yylval.namer = strdup(yytext);return '[';}
"\]"	{yylval.namer = strdup(yytext);return ']';}
";"		{yylval.namer = strdup(yytext);return SEMICOLON;}
":"		{yylval.namer = strdup(yytext);return COLON;}
"\."	{yylval.namer = strdup(yytext);return DOT;}
"\?"	{yylval.namer = strdup(yytext);return QUESTIONMARK;}
","		{yylval.namer = strdup(yytext);return COMMA;}
">="	{yylval.namer = strdup(yytext);return GREATER_EQUAL;}
"<="	{yylval.namer = strdup(yytext);return LESSER_EQUAL;}
"=="	{yylval.namer = strdup(yytext);;return DOUBLE_EQUAL;}
"="		{yylval.namer = strdup(yytext);return EQUAL;}
"<"		{yylval.namer = strdup(yytext);return LESSER;}
">"		{yylval.namer = strdup(yytext);return GREATER;}
"&"		{yylval.namer = strdup(yytext);return LOGICAL_AND;}
"|"		{yylval.namer = strdup(yytext);return LOGICAL_OR;}
"\^"	{yylval.namer = strdup(yytext);return LOGICAL_XOR;}

"+"		{yylval.namer = strdup(yytext);return '+';}
"-"		{yylval.namer = strdup(yytext);return '-';}
"\*"	{yylval.namer = strdup(yytext);return '*';}
"\/"	{yylval.namer = strdup(yytext);return '/';}
"\%"	{yylval.namer = strdup(yytext);return '%';}
"\""	{yylval.namer = strdup(yytext);return DOUBLEQUOTE;}
"\'"	{yylval.namer = strdup(yytext);return SINGLEQUOTE;}

[a-zA-Z][a-zA-Z0-9]*[[0-9][0-9]*]	{
							yylval.namer = strdup(yytext);
							return ARRAY_NUM;
}

[0-9][0-9]*	{
            yylval.namer = strdup(yytext);
			return NUMBER;
                }
[a-zA-Z][a-zA-Z0-9]* {
			yylval.namer = strdup(yytext);
			return IDENTIFIER;
		     }

[\"][^\"\']*[\"]		{yylval.namer = strdup(yytext);;return STRING;}


[ \t]		{ /* Do nothing */ }
.		{
		  printf("Unexpected token encountered: %s\n", yytext);
		  return ETOK;
		}
[\n]	{ /* Do nothing */ }

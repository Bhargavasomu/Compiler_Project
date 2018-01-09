/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    IDENTIFIER = 259,
    ETOK = 260,
    DATATYPE = 261,
    ARRAY_NUM = 262,
    ARRAY_LETTER = 263,
    SEMICOLON = 264,
    COLON = 265,
    DOT = 266,
    QUESTIONMARK = 267,
    COMMA = 268,
    DECLBLOCK = 269,
    CODEBLOCK = 270,
    LESSER = 271,
    GREATER = 272,
    EQUAL = 273,
    DOUBLE_EQUAL = 274,
    GREATER_EQUAL = 275,
    LESSER_EQUAL = 276,
    AND = 277,
    OR = 278,
    LOGICAL_AND = 279,
    LOGICAL_OR = 280,
    LOGICAL_XOR = 281,
    FOR = 282,
    WHILE = 283,
    IF = 284,
    ELSE = 285,
    PRINT_WITHOUT_NEW_LINE = 286,
    PRINT_WITH_NEW_LINE = 287,
    READ = 288,
    GOTO = 289,
    SINGLEQUOTE = 290,
    DOUBLEQUOTE = 291,
    STRING = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 46 "parser.y" /* yacc.c:1909  */

    char* namer;
    Ref structType;
    A* temp;
    variable* VAR;
    Numerical_Predicate* NP;
    GenArray* GA;
    Numerical_Expression* NE;
    expr* EXPR;
    T* TT;
    readClass* RS;
    argsClass* ARGS;
    OptionalArgsClass* OE;
    PrintSyntaxClass* PS;
    IfBlockSyntaxClass* IB;
    ElseBlockSyntaxClass* EB;
    StatementListClass* SL;
    WhileLoopSyntaxClass* WL;
    ForLoopDetailsClass* FLD;
    ForLoopSyntaxClass* FLS;
    AssignmentSyntaxClass* AS;
    possibilitiesClass* POS;
    GoToSyntaxClass* GT;
    LabelDeclarationSyntaxClass* LDS;
    OptionalIfClass* OIF;
    P* ppt;
    codeblock* CBLOCK;

#line 121 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

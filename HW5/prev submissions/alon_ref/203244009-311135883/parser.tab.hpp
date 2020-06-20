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

#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "parser.ypp" /* yacc.c:1909  */

	#include<string.h> 
	#include "hw3_output.hpp" 
	#include "hw5_common.hpp"
	#include "bp.hpp"
	
	using namespace std;
	using namespace hw5_stuff;
	
	#define YYSTYPE allTypes

#line 56 "parser.tab.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VOID = 258,
    INT = 259,
    BYTE = 260,
    BOOL = 261,
    ENUM = 262,
    RETURN = 263,
    IF = 264,
    ELSE = 265,
    WHILE = 266,
    BREAK = 267,
    CONTINUE = 268,
    SC = 269,
    COMMA = 270,
    LBRACE = 271,
    RBRACE = 272,
    ASSIGN = 273,
    OR = 274,
    AND = 275,
    RELOP_LEVEL_8_NEQ = 276,
    RELOP_LEVEL_8_EQ = 277,
    RELOP_LEVEL_9_GE = 278,
    RELOP_LEVEL_9_LE = 279,
    RELOP_LEVEL_9_GT = 280,
    RELOP_LEVEL_9_LT = 281,
    BINOP_LEVEL_11_PLUSE = 282,
    BINOP_LEVEL_11_MINUS = 283,
    BINOP_LEVEL_12_MUL = 284,
    BINOP_LEVEL_12_DIV = 285,
    NOT = 286,
    RPAREN = 287,
    LPAREN = 288,
    NUM = 289,
    STRING = 290,
    ID = 291,
    B = 292,
    TRUE = 293,
    FALSE = 294
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */

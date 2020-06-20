%{
	#include <stdio.h>
	#include <string.h>
	#include <iostream> 
	#include <sstream> 
	#include "parser.tab.hpp" 
	#include "hw3_output.hpp" 
	
	using namespace hw5_stuff;
	using namespace std;
	
	extern YYSTYPE yylval;
	
	extern void output::errorLex(int lineno);
%}
 
%option yylineno 
%option noyywrap 

id_good [a-zA-Z][a-zA-Z0-9]*
number_good (0|[1-9][0-9]*)
string_good ((["]([^\n\r\"\\]|(\\[nr\"\\]))+["])|(["]["]))
comment_good \/\/[^\r\n]*([\r]|[\n]|[\r\n])?
white_space ((\r\n)|[\t]|[\n]|[\r]|[ ])

%%

"void"			return VOID;
"int" 			return INT; 
"byte" 			return BYTE; 
"bool" 			return BOOL; 
"enum" 			return ENUM;
"and" 			return AND;
"or" 			return OR;
"not" 			return NOT;
"true" 			return TRUE;
"false" 		return FALSE;
"return" 		return RETURN;
"if" 			return IF;
"else" 			return ELSE;
"while" 		return WHILE;
"break" 		return BREAK;
[b] 			return B;
"continue" 		return CONTINUE;
";" 			return SC;
"," 			return COMMA;
"(" 			return LPAREN;
")" 			return RPAREN;
"{" 			return LBRACE;
"}" 			return RBRACE;
"==" 			return RELOP_LEVEL_8_EQ;
"!=" 			return RELOP_LEVEL_8_NEQ;
">=" 			return RELOP_LEVEL_9_GE;
"<=" 			return RELOP_LEVEL_9_LE;
">" 			return RELOP_LEVEL_9_GT;
"<" 			return RELOP_LEVEL_9_LT;
"=" 			return ASSIGN;
"+" 			return BINOP_LEVEL_11_PLUSE;
"-" 			return BINOP_LEVEL_11_MINUS;
"/" 			return BINOP_LEVEL_12_DIV;
"*" 			return BINOP_LEVEL_12_MUL;
{id_good}		{ yylval.string = strdup(yytext); return ID; }
{number_good}	{ stringstream num_str(yytext); num_str >> yylval.number; return NUM; }
{string_good}	{ yylval.string = strdup(yytext); return STRING; }
{comment_good} ;
{white_space} ;
. { output::errorLex(yylineno); exit(0); } 

%% 
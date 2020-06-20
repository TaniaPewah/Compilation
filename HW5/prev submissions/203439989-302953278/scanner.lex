%{

/* Declarations section */
#include <stdio.h>

//#define YYSTYPE Node *
//#define YYSTYPE Node_Az

#include "yystype.hpp"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
#include "Node.hpp"
int stringToNum(char* str);
%}

%option yylineno
%option noyywrap
digit   		([0-9])
digitExZiro     ([1-9])
letter  		([a-zA-Z])
whitespace		([\t\n\r ]|\n)
number ({digitExZiro}{digit}*)

%%
void                        {   yylval.type = "VOID";
                                yylval.lineno = yylineno;
                                return VOID;
                            }
int                         {
                                yylval.type = "INT";
                                yylval.lineno = yylineno;
                                return INT;}
byte                        {   yylval.type = "BYTE";
                                yylval.lineno = yylineno;
                                return BYTE;}
b 							{
                                yylval.lineno = yylineno;
                                return B;}
bool                        {
                                yylval.type = "BOOL";
                                yylval.lineno = yylineno;
                                return BOOL;}
enum                        {   yylval.type = "ENUM";
                                yylval.lineno = yylineno;
                                return ENUM;}
and                         {
                                yylval.binaryOp.setType(yytext);
                                return AND;}
or                          {
                                yylval.binaryOp.setType(yytext);
                                return OR;}
not                         {
                                yylval.binaryOp.setType(yytext);
                                return NOT;}
true                        {   yylval.type = "BOOL";
                                yylval.name = "true";
                                yylval.lineno = yylineno;
                                return TRUE;}
false                       {   yylval.type = "BOOL";
                                yylval.name = "false";
                                yylval.lineno = yylineno;
                                return FALSE;}
return                      {   yylval.lineno = yylineno;
                                return RETURN;}
if                          return IF;
else                        return ELSE;
while                       {   yylval.name = string(yytext);

                                return WHILE;}
break                       {   yylval.lineno = yylineno;
                                return BREAK;}
continue                    {   yylval.lineno = yylineno;
                                return CONTINUE;}
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
=                           {
                                yylval.name = string(yytext);
                                return ASSIGN;
                            }
(>|<|<=|>=)                 {   yylval.name = string(yytext);
                                yylval.lineno = yylineno;
                                yylval.type = "RELOPNONASSOC";
                                return RELOPNONASSOC;
                            }
(==|!=)                     {   yylval.name = string(yytext);
                                yylval.lineno = yylineno;
                                yylval.type = "RELOPASSOC";
                                return RELOPASSOC;
                            }
(\+|\-)                      {  yylval.name = string(yytext);
                                yylval.lineno = yylineno;
                                yylval.type = "ADDSUB";
                                yylval.binaryOp.setType(yytext);
                                return ADDSUB;
                             }
(\*|\/)                      {  yylval.name = string(yytext);
                                yylval.lineno = yylineno;
                                yylval.type = "MULDIV";
                                yylval.binaryOp.setType(yytext);
                                return MULDIV;
                             }
{letter}({letter}|{digit})* {   yylval.name = string(yytext);
                                yylval.lineno = yylineno;
                                yylval.type = "ID";
                                return ID;}
{number}|0    				{   yylval.name = string(yytext);
                                yylval.type = "NUM";
                                yylval.lineno = yylineno;
                                return NUM;
                            }
\"([^\n\r\"\\]|\\[rnt\"\\])+\" {   string val ="";
                                    int len_size =string(yytext).size()-2;
                                    val = string(yytext).substr(1,len_size);
                                    yylval.name = val;

                                    yylval.type = "STRING";
                                    yylval.lineno = yylineno;
                                    return STRING;}
{whitespace}				;
\/\/[^\r\n]*[\r|\n|\r\n]?     ;


.							{output::errorLex(yylineno);exit(0);}
%%

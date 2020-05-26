%{

/* Declarations section */
#include "hw3_output.hpp"
#include "parser.hpp"
#include "parser.tab.hpp"

void handleGeneralError();
%}

%option yylineno
%option noyywrap

%%
void                        { yylval =  new TypeNode( yylineno, string(yytext));  return VOID;}
int                         { yylval =  new TypeNode( yylineno, string(yytext));  return INT;}
byte                        { yylval =  new TypeNode( yylineno, string(yytext));  return BYTE;}
b 							{ yylval =  new TypeNode( yylineno, string(yytext));  return B;}
bool                        { yylval =  new TypeNode( yylineno, string(yytext));  return BOOL;}
and                         return AND;
or                          return OR;
not                         return NOT;
true                        { yylval =  new ExpNode( yylineno, string("bool"));  return TRUE; } 
false                       { yylval =  new ExpNode( yylineno,  string("bool")); return FALSE; }
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
continue                    return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
=                           return ASSIGN;
(==)|(!=)                   return RELOPLEFT;
(<|>|(<=)|(>=))             return RELOPNOTASS;
(\+|\-)                     { yylval =  new BinopNode( yylineno, string(yytext)); return ADDITIVE; }
(\*|\/)                     { yylval =  new BinopNode( yylineno, string(yytext)); return MUL; }
[a-zA-Z]([a-zA-Z0-9])* 		{ yylval =  new IdNode( yylineno, string(yytext)); return ID; }
0|[1-9][0-9]*   			{ yylval = new NumNode( yylineno, string(yytext), string("int")); return NUM; }
\"([^\n\r\"\\]|\\[rnt\"\\])+\" { yylval = new StringNode( yylineno, string(yytext)); return STRING; }
[\t\n\r ]|\n				;
\/\/[^\r\n]*[\r|\n|\r\n]?   ;
.							handleGeneralError();

%%

void handleGeneralError(){
    output::errorLex( yylineno );
    exit(0);
}

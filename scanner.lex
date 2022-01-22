%{
/* Declarations section */
#include "NonTerminalClasses.h"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
#include <string>
%}

%option yylineno
%option noyywrap

digit                                           ([0-9])
letter                                          ([a-zA-Z])
whitespace                                      ([\t\n\r ])

%%
void                                            return VOID;
int                                             return INT;
byte                                            return BYTE;
b                                               return B;
bool                                            return BOOL;
const                                           return CONST;
and                                             return AND;
or                                              return OR;
not                                             return NOT;
true                                            return TRUE;
false                                           return FALSE;
return                                          return RETURN;
if                                              return IF;
else                                            return ELSE;
while                                           return WHILE;
break                                           return BREAK;
continue                                        return CONTINUE;
;                                               return SC;
,                                               return COMMA;
\(                                              return LPAREN;
\)                                              return RPAREN;
\{                                              return LBRACE;
\}                                              return RBRACE;
=                                               return ASSIGN;
([!=]=)                                         return RELOP_EQ;
((([<>])=)|<|>)                                 return RELOP_RELATION;
([-\+])                                         return PLUS_MINUS;
([\*/])                                         return PRODUCT_DEVISION;
(({letter})({letter}|{digit})*)                 yylval = new Id(yytext); return ID;
((([1-9]({digit})*))|0)                         yylval = new Num(atoi(yytext)); return NUM;
(\"([^\n\r\"\\]|\\[rnt\"\\])+\")                return STRING;
({whitespace})                                  ;
\/\/[^\r\n]*(\r|\n|\r\n)?                       ;
.                                               output::errorLex(yylineno); exit(0);
%%

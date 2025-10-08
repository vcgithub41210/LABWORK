
%{
#include "bnf.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}
%option noyywrap

%%
[0-9]+                  { yylval.intval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.strval = strdup(yytext); return ID; }
[ \t]                   ;                // ignore spaces
\n                      { return '\n'; } // newline ends expression
"+"                     { return '+'; }
"-"                     { return '-'; }
"="                     { return '='; }
"("                     { return '('; }
")"                     { return ')'; }
.                       { /* ignore unknown */ }
%%


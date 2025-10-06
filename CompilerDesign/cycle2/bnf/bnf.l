%{
#include "bnf.tab.h"
%}

%%

[0-9]+      { yylval.num = atoi(yytext); return NUMBER; }
[ \t]+      ;            /* ignore spaces and tabs */
\n          ;            /* ignore newlines */
.           { return yytext[0]; }   /* return single-character operators like + - * / ( ) */

%%

int yywrap(void) { return 1; }


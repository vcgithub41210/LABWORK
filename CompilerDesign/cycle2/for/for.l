%{
#include <string.h>
#include "for.tab.h"
char *datatypes[] = {"int","char","float","double"};
int size = sizeof(datatypes)/sizeof(datatypes[0]);
%}
%option noyywrap
%%

"for"       { return FOR; }
"("         { return LPAREN; }
")"         { return RPAREN; }
";"         { return SEMICOLON;}
"="         {return ASSIGN;}
"<"|">"|"<="|">="|"!="|"==" { return COMPARE;}
"+="|"-="|"*="|"/=" { return UPDATE;}
"--"|"++" { return UNARY_OP;}
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { for(int i=0;i<size;i++) {
                            if(strcmp(yytext,datatypes[i])==0)
                                return DATATYPE;
                        }
                        return IDENTIFIER; }

[ \t\n]     ;   /* ignore whitespace */
.           { return yytext[0]; }

%%

/*

lex for.l
yacc -d for.y
gcc lex.yy.c y.tab.c -o for
./for

*/

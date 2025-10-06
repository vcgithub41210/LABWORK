%{
 #include <stdio.h>
%}
%option noyywrap

%%
[0-9]+  {return NUMBER;}
[a-zA-Z][a-zA-Z0-9]* {return IDENTIFIER;}
%%

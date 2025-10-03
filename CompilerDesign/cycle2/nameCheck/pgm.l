
%{
#include <stdio.h>
%}

%%
.*vina.* {printf("Contains first four characters as substring \n");}
.* {printf("Accepted \n");}
%%

int yywrap(){
  return 1;
}
int main(){
  yylex();
  return 0;
}

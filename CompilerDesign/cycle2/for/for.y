%{
#include <stdio.h>
#include <string.h>
int yylex();
void yyerror(const char *s);
%}
%token FOR RPAREN LPAREN NUMBER IDENTIFIER SEMICOLON DATATYPE UPDATE ASSIGN COMPARE UNARY_OP
%%
start: FOR LPAREN i_expr SEMICOLON c_expr SEMICOLON u_expr RPAREN { printf("Valid for loop\n");return 0; }
     ;
c_expr: IDENTIFIER COMPARE IDENTIFIER
      | IDENTIFIER COMPARE NUMBER
      ;

u_expr: IDENTIFIER UPDATE NUMBER
      | IDENTIFIER UPDATE IDENTIFIER
      | IDENTIFIER UNARY_OP

i_expr: variable ASSIGN NUMBER
      | variable ASSIGN IDENTIFIER
      ;
variable: DATATYPE IDENTIFIER
        | IDENTIFIER
        ;
initialization: DATATYPE expr
              | expr
              ;
expr: IDENTIFIER ASSIGN NUMBER
    | IDENTIFIER ASSIGN IDENTIFIER
    ;
%%

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
}
int main(){
  printf("Enter the for loop: ");
  yyparse();
  return 0;
}

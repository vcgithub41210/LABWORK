%{
    #include <stdio.h>
    extern int yylex();
    extern int yyerror(const char *);
%}

%token digit letter

%%

start : letter s {printf("Valid variable\n");}
      ;
s :     letter s 
    |   digit s
    |   /* empty */
    ;

%%

int yyerror(const char *s) {
    printf("\nIt's not an identifier!\n");
    return 0;
}

int main() {
    printf("\nEnter a name to test if it's a valid identifier: ");
    yyparse();
    return 0;
}

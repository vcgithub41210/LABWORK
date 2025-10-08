
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct AST {
    int intval;
    char *strval;
    int type;
    struct AST *left, *right;
} AST;

AST* makeNode(char* op, AST* left, AST* right){
    AST* node = (AST*)malloc(sizeof(AST));
    node->strval = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

AST* makeIDLeaf(char* id){
    AST* node = (AST*)malloc(sizeof(AST));
    node->strval = strdup(id);
    node->type = 0; // ID
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST* makeNUMLeaf(int val){
    AST* node = (AST*)malloc(sizeof(AST));
    node->intval = val;
    node->type = 1; // NUM
    node->left = NULL;
    node->right = NULL;
    return node;
}
void printTree(AST* root, int level) {
    if (root == NULL) return;
    for (int i = 0; i < level; i++) 
        printf("  ");
    if (root->left == NULL && root->right == NULL) {
        if (root->type == 0)
            printf("ID(%s)\n", root->strval);
        else
            printf("NUM(%d)\n", root->intval);
    } else {
        printf("OP(%s)\n", root->strval);
    }
    printTree(root->left, level + 1);
    printTree(root->right, level + 1);
}

void yyerror(const char *s);
int yylex();
%}

%union {
    int intval;
    char* strval;
    struct AST* AST;
}

%token <intval> NUM
%token <strval> ID
%type <AST> E T

%%
start:
      /* empty */
    | start E '\n' { printTree($2,0); printf("\nParsed successfully\n"); }
    ;

E:
      E '+' E  { $$ = makeNode("+", $1, $3); }
    | E '-' E  { $$ = makeNode("-", $1, $3); }
    | E '=' E  { $$ = makeNode("=", $1, $3); }
    | T        { $$ = $1; }
    ;

T:
      '(' E ')'  { $$ = $2; }
    | ID         { $$ = makeIDLeaf($1); }
    | NUM        { $$ = makeNUMLeaf($1); }
    ;
%%
void yyerror(const char *s) {
   fprintf(stderr, "Error: %s\n", s);
}
int main(){
  printf("Enter expression:\n");
  yyparse();
  return 0;
}


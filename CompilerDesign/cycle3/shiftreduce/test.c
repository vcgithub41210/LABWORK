#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int prodNum, top = -1, len = 0;
char stack[10], prod[10][10], expression[10], start, action[50];
char *rhs;

void push(char c) {
  stack[++top] = c;
  stack[top + 1] = '\0';
}
void pop(int len) {
  top -= len;
  if (top < -1)
    top = -1;
  stack[top + 1] = '\0';
}

int match() {
  len = strlen(rhs);
  if (top + 1 < len)
    return 0;
  for (int i = 0; i < len; i++)
    if (stack[top - len + 1 + i] != rhs[i])
      return 0;
  return 1;
}

int reduce() {
  for (int i = 0; i < prodNum; i++) {
    char lhs = prod[i][0];
    rhs = prod[i] + 1;
    if (match()) {
      pop(len);
      push(lhs);
      sprintf(action, "Reduce: %c->%s", lhs, rhs);
      return 1;
    }
  }
  return 0;
}

int parse() {
  int i = 0;
  while (expression[i] != '\0') {
    push(expression[i++]);
    sprintf(action, "Shift: %c", expression[i - 1]);
    printf("\n%-25s %-25s %-25s\n", stack, expression + i, action);
    while (reduce())
      printf("\n%-25s %-25s %-25s\n", stack, expression + i, action);
  }
  while (reduce())
    printf("\n%-25s %-25s %-25s\n", stack, expression + i, action);
  if (top == 0 && stack[top] == start)
    return 1;
  return 0;
}

int main() {
  FILE *fp = fopen("rules.txt", "r");
  while (fscanf(fp, " %c->%s", &prod[prodNum][0], prod[prodNum] + 1) != EOF)
    prodNum++;
  for (int i = 0; i < prodNum; i++)
    printf("Production %d: %c -> %s\n", i + 1, prod[i][0], prod[i] + 1);
  printf("Enter the expression: ");
  scanf("%s", expression);
  printf("Enter the starting symbol: ");
  scanf(" %c", &start);
  printf("\n%-25s %-25s %-25s\n", "Stack", "Input", "Action");
  if (parse())
    printf("Accepted\n");
  else
    printf("Rejected\n");
}

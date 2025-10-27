#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char lhs;
  char rules[10];
} Production;

int numterminals, numnonterminals, numrules;
char terminals[10], nonterminals[10], firsts[10][10], firstcounts[10];
Production productions[10];

int isterminal(char c) {
  for (int i = 0; i < numterminals; i++) {
    if (terminals[i] == c)
      return 1;
  }
  return 0;
}

int indexof(char c, char string[10], int size) {
  for (int i = 0; i < size; i++)
    if (string[i] == c)
      return i;
  return -1;
}

int alreadyin(char c, char array[10], int size) {
  for (int i = 0; i < size; i++)
    if (array[i] == c)
      return 1;
  return 0;
}

void find_first(char c, int num) {
  int anyProdEpsilon = 0;
  for (int i = 0; i < numrules; i++) {
    if (productions[i].lhs == c) {
      int idx;
      int canbeepsilon = 1;
      for (int k = 0; productions[i].rules[k] != '\0'; k++) {
        char symbol = productions[i].rules[k];

        if (symbol == 'e' && !alreadyin('e', firsts[num], firstcounts[num])) {
          firsts[num][firstcounts[num]] = 'e';
          firstcounts[num]++;
        } else if (isterminal(symbol)) {
          if (!alreadyin(symbol, firsts[num], firstcounts[num]))
            firsts[num][firstcounts[num]++] = symbol;
          canbeepsilon = 0;
          break;
        } else {
          int hasepsilon = 0;
          idx = indexof(symbol, nonterminals, numnonterminals);
          find_first(symbol, idx);
          for (int j = 0; j < firstcounts[idx]; j++) {
            if (!alreadyin(firsts[idx][j], firsts[num], firstcounts[num]) &&
                firsts[idx][j] != 'e') {
              firsts[num][firstcounts[num]] = firsts[idx][j];
              firstcounts[num]++;
            }
          }
          for (int j = 0; j < firstcounts[idx]; j++) {
            if (firsts[idx][j] == 'e')
              hasepsilon = 1;
          }
          if (!hasepsilon) {
            canbeepsilon = 0;
            break;
          }
        }
      }
      if (canbeepsilon)
        anyProdEpsilon = 1;
    }
  }
  if (anyProdEpsilon && !alreadyin('e', firsts[num], firstcounts[num]))
    firsts[num][firstcounts[num]++] = 'e';
}

void main() {
  FILE *f = fopen("test.txt", "r");

  printf("Enter number of terminals and nonterminals: ");
  scanf("%d %d", &numterminals, &numnonterminals);

  printf("Enter terminals and nonterminals: ");
  scanf("%s %s", terminals, nonterminals);

  printf("Enter number of rules: ");
  scanf("%d", &numrules);

  for (int i = 0; i < numrules; i++)
    fscanf(f, " %c %s", &productions[i].lhs, productions[i].rules);

  for (int i = 0; i < numnonterminals; i++)
    find_first(nonterminals[i], i);

  for (int i = 0; i < numnonterminals; i++) {
    printf("First(%c) = { ", nonterminals[i]);
    for (int j = 0; j < firstcounts[i]; j++) {
      printf("%c ", firsts[i][j]);
    }
    printf("}\n");
  }
}

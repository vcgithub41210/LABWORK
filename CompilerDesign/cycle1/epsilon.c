#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char states[20][20], result[20][20];
void add_state(char state[], int index) { strcpy(result[index], state); }
void display(int i, char state[]) {
  printf("Epsilon closure of %s: { ", state);
  for (int j = 0; j < i; j++) {
    printf("%s ", result[j]);
  }
  printf("}\n");
}
int main() {
  char current[3], start[3], symbol[3], end[3], copy[3];
  int n, eof;
  FILE *fptr = fopen("input.txt", "r");
  if (fptr == NULL) {
    printf("Eror opening file\n");
    return 0;
  }
  printf("Enter the number of states: ");
  scanf("%d", &n);
  printf("Enter the states: \n");
  for (int i = 0; i < n; i++) {
    scanf("%s", states[i]);
  }
  for (int k = 0; k < n; k++) {
    int i = 0;
    strcpy(current, states[k]);
    strcpy(copy, current);
    add_state(current, i++);
    while (1) {
      eof = fscanf(fptr, "%s %s %s", start, symbol, end);
      if (eof == EOF) {
        break;
      }
      if (strcmp(current, start) == 0 && strcmp(symbol, "e") == 0) {
        add_state(end, i++);
        strcpy(current, end);
      }
    }
    display(i, copy);
    rewind(fptr);
  }
  fclose(fptr);
  return 0;
}

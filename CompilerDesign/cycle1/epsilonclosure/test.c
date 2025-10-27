#include <stdio.h>
#include <string.h>

#define MAX 10

int states, symbols;
char alpha[MAX];          // alphabet symbols (including 'e')
int trans[MAX][MAX][MAX]; // transitions[from][symbol][to]
int closure[MAX][MAX];    // epsilon closures

// Recursive epsilon closure finder

void epsilonClosure(int s, int closureRow[]) {
  if (closureRow[s])
    return;          // already visited
  closureRow[s] = 1; // mark current
  for (int t = 0; t < states; t++)
    if (trans[s][symbols][t]) // last symbol is 'e'
      epsilonClosure(t, closureRow);
}

// Function to print set of states
void printSet(int set[]) {
  printf("{ ");
  for (int i = 0; i < states; i++)
    if (set[i])
      printf("q%d ", i);
  printf("}");
}

int main() {
  printf("Enter number of states: ");
  scanf("%d", &states);

  printf("Enter number of input symbols (excluding epsilon): ");
  scanf("%d", &symbols);

  alpha[symbols] = 'e'; // Add epsilon symbol as last
  printf("Enter symbols: ");
  for (int i = 0; i < symbols; i++)
    scanf(" %c", &alpha[i]);

  printf("Enter transitions (1 for present, 0 for absent):\n");
  for (int i = 0; i < states; i++) {
    for (int j = 0; j < symbols + 1; j++) {
      for (int k = 0; k < states; k++) {
        printf("Transition from q%d on '%c' to q%d? (1/0): ", i,
               j == symbols ? 'e' : alpha[j], k);
        scanf("%d", &trans[i][j][k]);
      }
    }
  }

  // Compute epsilon closures for each state
  for (int i = 0; i < states; i++) {
    for (int j = 0; j < states; j++)
      closure[i][j] = 0;
    epsilonClosure(i, closure[i]);
  }

  printf("\n--- Epsilon Closures ---\n");
  for (int i = 0; i < states; i++) {
    printf("E-closure(q%d) = ", i);
    printSet(closure[i]);
    printf("\n");
  }

  printf("\n--- NFA Transitions (after removing epsilon) ---\n");
  for (int i = 0; i < states; i++) {
    for (int j = 0; j < symbols; j++) { // skip epsilon
      int result[MAX] = {0};

      // For each state in ε-closure(qi)
      for (int s = 0; s < states; s++)
        if (closure[i][s])
          for (int t = 0; t < states; t++)
            if (trans[s][j][t])
              // add ε-closure of target
              for (int k = 0; k < states; k++)
                if (closure[t][k])
                  result[k] = 1;

      printf("From E-closure(q%d) on '%c' -> ", i, alpha[j]);
      printSet(result);
      printf("\n");
    }
  }

  return 0;
}

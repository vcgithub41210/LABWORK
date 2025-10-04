#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

// Transition table: state x symbol -> next state
int transition[MAX][MAX];
int states, symbols;
int final_states[MAX], final_count;
int start_state;

// Partition array
int partition[MAX], new_partition[MAX];

// Check if two states are distinguishable
int different(int s1, int s2) {
  if (final_states[s1] != final_states[s2])
    return 1;
  return 0;
}

int main() {
  printf("Enter number of states: ");
  scanf("%d", &states);

  printf("Enter number of symbols: ");
  scanf("%d", &symbols);

  printf("Enter transition table (%d x %d):\n", states, symbols);
  for (int i = 0; i < states; i++) {
    for (int j = 0; j < symbols; j++) {
      scanf("%d", &transition[i][j]);
    }
  }

  printf("Enter start state: ");
  scanf("%d", &start_state);

  printf("Enter number of final states: ");
  scanf("%d", &final_count);

  memset(final_states, 0, sizeof(final_states));
  printf("Enter final states: ");
  for (int i = 0; i < final_count; i++) {
    int f;
    scanf("%d", &f);
    final_states[f] = 1;
  }

  // Initial partition
  for (int i = 0; i < states; i++) {
    partition[i] = final_states[i] ? 1 : 0;
  }

  int changed;
  do {
    changed = 0;
    int new_class = 0;
    for (int i = 0; i < states; i++)
      new_partition[i] = -1;

    for (int i = 0; i < states; i++) {
      if (new_partition[i] == -1) {
        new_partition[i] = new_class;
        for (int j = i + 1; j < states; j++) {
          if (partition[i] == partition[j]) {
            int same = 1;
            for (int s = 0; s < symbols; s++) {
              if (partition[transition[i][s]] != partition[transition[j][s]]) {
                same = 0;
                break;
              }
            }
            if (same)
              new_partition[j] = new_class;
          }
        }
        new_class++;
      }
    }

    // Check if changed
    for (int i = 0; i < states; i++) {
      if (partition[i] != new_partition[i]) {
        changed = 1;
        break;
      }
    }
    for (int i = 0; i < states; i++)
      partition[i] = new_partition[i];

  } while (changed);

  // Print minimized DFA
  int classes = 0;
  for (int i = 0; i < states; i++) {
    if (partition[i] > classes)
      classes = partition[i];
  }
  classes++;

  printf("\nMinimized DFA has %d states:\n", classes);
  for (int i = 0; i < classes; i++) {
    printf("State %d: {", i);
    for (int j = 0; j < states; j++)
      if (partition[j] == i)
        printf(" %d ", j);
    printf("}\n");
  }

  printf("\nTransitions in Minimized DFA:\n");
  for (int i = 0; i < classes; i++) {
    for (int s = 0; s < symbols; s++) {
      int old_rep = -1;
      for (int j = 0; j < states; j++) {
        if (partition[j] == i) {
          old_rep = j;
          break;
        }
      }
      if (old_rep != -1) {
        printf("%d --%d--> %d\n", i, s, partition[transition[old_rep][s]]);
      }
    }
  }

  printf("\nStart state: %d\n", partition[start_state]);
  printf("Final states: ");
  for (int i = 0; i < states; i++) {
    if (final_states[i] && partition[i] != -1) {
      printf("%d ", partition[i]);
    }
  }
  printf("\n");

  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct States {
  char state[5];
  int closure_count;
  char closures[50][5];
} recorded_states[50];

struct Transition {
  char from[5];
  char symbol[5];
  char to[5];
} trans[100];

struct Transition eps_trans[100]; // Only epsilon
int count = 0, t_count = 0, eps_count = 0;

int search_state(char *initial) {
  for (int i = 0; i < count; i++) {
    if (strcmp(recorded_states[i].state, initial) == 0)
      return i;
  }
  return -1;
}

int add_state(char *initial) {
  strcpy(recorded_states[count].state, initial);
  recorded_states[count].closure_count = 1;
  strcpy(recorded_states[count].closures[0], initial);
  return count++;
}

int in_closure(int i, char *state) {
  for (int x = 0; x < recorded_states[i].closure_count; x++) {
    if (strcmp(recorded_states[i].closures[x], state) == 0)
      return 1;
  }
  return 0;
}

void add_closure(char *initial, char *final) {
  int i = search_state(initial);
  if (i == -1) {
    i = add_state(initial);
  }
  if (strcmp(final, "") != 0 && !in_closure(i, final)) {
    strcpy(recorded_states[i].closures[recorded_states[i].closure_count++],
           final);
  }
}

void expand_closures() {
  int changed = 1;
  while (changed) {
    changed = 0;
    for (int i = 0; i < count; i++) {
      for (int x = 0; x < recorded_states[i].closure_count; x++) {
        char *state = recorded_states[i].closures[x];
        for (int t = 0; t < eps_count; t++) {
          if (strcmp(eps_trans[t].from, state) == 0) {
            if (!in_closure(i, eps_trans[t].to)) {
              strcpy(recorded_states[i]
                         .closures[recorded_states[i].closure_count++],
                     eps_trans[t].to);
              changed = 1;
            }
          }
        }
      }
    }
  }
}

void find_closures(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    printf("No such file exists\n");
    exit(0);
  }
  char initial[5], final[5], symbol[5];

  //```
  while (fscanf(fp, "%s %s %s", initial, symbol, final) == 3) {
    if (strcmp(symbol, "e") == 0) { // epsilon
      add_closure(initial, final);
      strcpy(eps_trans[eps_count].from, initial);
      strcpy(eps_trans[eps_count].to, final);
      eps_count++;
    } else {
      // store normal transition
      strcpy(trans[t_count].from, initial);
      strcpy(trans[t_count].symbol, symbol);
      strcpy(trans[t_count].to, final);
      t_count++;
    }
    add_closure(initial, "");
    add_closure(final, "");
  }
  fclose(fp);
  expand_closures();
  //```
}

// Function to check if state is already added
int in_set(char result[50][5], int *rcount, char *s) {
  for (int i = 0; i < *rcount; i++) {
    if (strcmp(result[i], s) == 0)
      return 1;
  }
  return 0;
}

// Move function: from closure(S) using symbol a
void move_closure(int state_index, char *symbol, char result[50][5],
                  int *rcount) {
  *rcount = 0;
  for (int i = 0; i < recorded_states[state_index].closure_count; i++) {
    char *s = recorded_states[state_index].closures[i];
    for (int t = 0; t < t_count; t++) {
      if (strcmp(trans[t].from, s) == 0 &&
          strcmp(trans[t].symbol, symbol) == 0) {
        int idx = search_state(trans[t].to);
        if (idx != -1) {
          for (int c = 0; c < recorded_states[idx].closure_count; c++) {
            if (!in_set(result, rcount, recorded_states[idx].closures[c])) {
              strcpy(result[(*rcount)++], recorded_states[idx].closures[c]);
            }
          }
        }
      }
    }
  }
}

int main() {
  char filename[50];
  printf("Enter the name of the file with state transitions: ");
  scanf("%s", filename);
  find_closures(filename);

  printf("\nEpsilon Closures of all states:\n");
  for (int i = 0; i < count; i++) {
    printf("%s: {", recorded_states[i].state);
    for (int x = 0; x < recorded_states[i].closure_count; x++) {
      printf("%s%s", recorded_states[i].closures[x],
             x + 1 == recorded_states[i].closure_count ? "" : ", ");
    }
    printf("}\n");
  }

  printf("\nEquivalent NFA Transitions (without epsilon):\n");
  for (int i = 0; i < count; i++) {
    char symbols[20][5];
    int s_count = 0;

    // collect all input symbols (non-epsilon)
    for (int t = 0; t < t_count; t++) {
      if (search_state(trans[t].from) == i) {
        int found = 0;
        for (int k = 0; k < s_count; k++) {
          if (strcmp(symbols[k], trans[t].symbol) == 0) {
            found = 1;
            break;
          }
        }
        if (!found)
          strcpy(symbols[s_count++], trans[t].symbol);
      }
    }

    // compute transitions
    for (int k = 0; k < s_count; k++) {
      char result[50][5];
      int rcount;
      move_closure(i, symbols[k], result, &rcount);

      printf("%s --%s--> {", recorded_states[i].state, symbols[k]);
      for (int r = 0; r < rcount; r++) {
        printf("%s%s", result[r], r + 1 == rcount ? "" : ", ");
      }
      printf("}\n");
    }
  }
}

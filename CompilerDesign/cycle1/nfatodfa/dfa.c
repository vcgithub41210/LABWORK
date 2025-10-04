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
  char to[5];
} eps_trans[100];

int count = 0;
int eps_count = 0;

int search_state(char *initial) {
  for (int i = 0; i < count; i++) {
    if (strcmp(recorded_states[i].state, initial) == 0) {
      return i;
    }
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
        // Check epsilon transitions from this state
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

  while (fscanf(fp, "%s %s %s", initial, symbol, final) == 3) {
    if (strcmp(symbol, "e") == 0) {
      add_closure(initial, final);
      strcpy(eps_trans[eps_count].from, initial);
      strcpy(eps_trans[eps_count].to, final);
      eps_count++;
    }
    add_closure(initial, "");
    add_closure(final, "");
  }
  fclose(fp);
  expand_closures();
}

int main() {
  char filename[50];
  printf("Enter the name of the file with state transitions: ");
  scanf("%s", filename);
  find_closures(filename);

  printf("Epsilon Closures of all states:\n");
  for (int i = 0; i < count; i++) {
    printf("%s: {", recorded_states[i].state);
    for (int x = 0; x < recorded_states[i].closure_count; x++) {
      printf("%s%s", recorded_states[i].closures[x],
             x + 1 == recorded_states[i].closure_count ? "" : ", ");
    }
    printf("}\n");
  }
}

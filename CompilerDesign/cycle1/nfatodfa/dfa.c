#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATES 100
// global variables
char sym[5], nfastate[5], symbols[100][5];
int curr_state_count, res_count, symbol_count = 0;

struct NFATransition {
  char from[5], symbol[5], to[5];
} nfa_trans[100];
int nfa_count = 0;

struct DFATransition {
  char from[MAX_STATES][5], symbol[5], to[MAX_STATES][5];
  int from_count, to_count;
} dfa_trans[100];
int dfa_count = 0;

struct DFAState {
  char state[MAX_STATES][5];
  int state_count;
} dfa_states[100];
int dfa_state_count = 0;

int search_state(char *state) {
  for (int i = 0; i < nfa_count; i++) {
    if (strcmp(nfa_trans[i].from, state) == 0)
      return i;
  }
  return -1;
}

void add_to_res(char *state, char *symbol, char res[][5], int *res_count) {
  for (int i = 0; i < nfa_count; i++) {
    if (strcmp(nfa_trans[i].from, state) == 0 &&
        strcmp(nfa_trans[i].symbol, symbol) == 0) {
      strcpy(res[(*res_count)++], nfa_trans[i].to);
    }
  }
}

void find_nfa_transitions(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    printf("No such file exists\n");
    exit(0);
  }
  char initial[5], final[5], symbol[5];
  while (fscanf(fp, "%s %s %s", initial, symbol, final) == 3) {
    strcpy(nfa_trans[nfa_count].from, initial);
    strcpy(nfa_trans[nfa_count].symbol, symbol);
    strcpy(nfa_trans[nfa_count++].to, final);
    if (symbol_count == 0)
      strcpy(symbols[symbol_count++], symbol);
    else {
      int found = 0;
      for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i], symbol) == 0) {
          found = 1;
          break;
        }
      }
      if (!found)
        strcpy(symbols[symbol_count++], symbol);
    }
  }
  fclose(fp);
}

int state_exists(char res[][5], int res_count) {
  for (int i = 0; i < dfa_state_count; i++) {
    if (dfa_states[i].state_count == res_count) {
      int match = 1;
      for (int j = 0; j < res_count; j++) {
        int found = 0;
        for (int k = 0; k < res_count; k++) {
          if (strcmp(res[j], dfa_states[i].state[k]) == 0) {
            found = 1;
            break;
          }
        }
        if (!found) {
          match = 0;
          break;
        }
      }
      if (match)
        return i;
    }
  }
  return -1;
}

void add_dfa_transition(char from[][5], int from_count, char *symbol,
                        char to[][5], int to_count) {
  for (int i = 0; i < from_count; i++)
    strcpy(dfa_trans[dfa_count].from[i], from[i]);
  for (int i = 0; i < to_count; i++)
    strcpy(dfa_trans[dfa_count].to[i], to[i]);
  strcpy(dfa_trans[dfa_count].symbol, symbol);
  dfa_trans[dfa_count].from_count = from_count;
  dfa_trans[dfa_count++].to_count = to_count;
}

void print_transitions() {
  printf("\nDFA Transitions:\n");
  for (int i = 0; i < dfa_count; i++) {
    printf("{ ");
    for (int j = 0; j < dfa_trans[i].from_count; j++)
      printf("%s ", dfa_trans[i].from[j]);
    printf("} --%s--> { ", dfa_trans[i].symbol);
    for (int j = 0; j < dfa_trans[i].to_count; j++)
      printf("%s ", dfa_trans[i].to[j]);
    printf("}\n");
  }
}

void find_dfa_transistions(char *start_state) {
  strcpy(dfa_states[dfa_state_count++].state[0], start_state);
  dfa_states[0].state_count = 1;
  for (int curr_state = 0; curr_state < dfa_state_count; curr_state++) {
    for (int s = 0; s < symbol_count; s++) {
      char res[MAX_STATES][5];
      res_count = 0;
      strcpy(sym, symbols[s]);
      curr_state_count = dfa_states[curr_state].state_count;
      for (int st = 0; st < curr_state_count; st++) {
        strcpy(nfastate, dfa_states[curr_state].state[st]);
        add_to_res(nfastate, sym, res, &res_count);
      }
      if (res_count == 0)
        continue;
      if (state_exists(res, res_count) == -1) {
        for (int r = 0; r < res_count; r++)
          strcpy(dfa_states[dfa_state_count].state[r], res[r]);
        dfa_states[dfa_state_count].state_count = res_count;
        dfa_state_count++;
      }
      add_dfa_transition(dfa_states[curr_state].state,
                         dfa_states[curr_state].state_count, sym, res,
                         res_count);
    }
  }
  print_transitions();
}

int main() {
  char filename[50];
  printf("Enter the name of the file with state transitions: ");
  scanf("%s", filename);
  find_nfa_transitions(filename);
  printf("Enter the start state of the NFA: ");
  char start_state[5];
  scanf("%s", start_state);
  // check if start state is valid
  if (search_state(start_state) == -1) {
    printf("Invalid start state\n");
    exit(0);
  }
  find_dfa_transistions(start_state);
}

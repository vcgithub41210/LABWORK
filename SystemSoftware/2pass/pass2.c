#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int length, address, opcode, starting_address;
FILE *input, *optab, *symtab, *objcode, *temptr;
char label[20], operator[20], operand[20], mnemonic[20], symbol[20], lbl[50];

void CLOSE() {
  if (input) fclose(input);
  if (objcode) fclose(objcode);
  if (optab) fclose(optab);
  if (symtab) fclose(symtab);
}

int findOpTAB() {
  rewind(optab);
  while (!feof(optab)) {
    fscanf(optab, "%s %x", mnemonic, &opcode);
    if (strcmp(operator, mnemonic) == 0) return opcode;
  }
  return -1;
}

int findSymTab() {
  rewind(symtab);
  while (!feof(symtab)) {
    fscanf(symtab, "%s %x", symbol, &address);
    if (strcmp(operand, symbol) == 0) return address;
  }
  return -1;
}
int main() {
  input = fopen("intermediate.txt", "r");
  optab = fopen("optab.txt", "r");
  symtab = fopen("symtab.txt", "r");
  temptr = fopen("length.txt", "r");
  fscanf(temptr, "%x", &length);
  fclose(temptr);
  objcode = fopen("objcode.txt", "w");
  fscanf(input, "%x %s %s %s", &address, label, operator, operand);
  if (strcmp(operator, "START") == 0) {
    starting_address = (int)strtol(operand, NULL, 16);
  } else {
    starting_address = 0;
  }
  fprintf(objcode, "H^%s^%06X^%06X\n", label, starting_address, length);
  fscanf(input, "%x %s %s %s", &address, label, operator, operand);
  while (strcmp(operator, "END") != 0) {
    opcode = findOpTAB();
    if (opcode == -1) { // If opcode not found
      if (strcmp(operator, "WORD") == 0) {
        sprintf(lbl, "%06X", (int)strtol(operand, NULL, 16));
      } else if (strcmp(operator, "BYTE") == 0) { // Start after 'C' or 'X'
        sprintf(lbl,"");
        for(int i = 2; operand[i] != '\'';i++) sprintf(lbl+strlen(lbl),"%02X",operand[i]);
      } else if (strcmp(operator, "RESW") == 0 || strcmp(operator, "RESB") == 0) address = -1;
      else {
        printf("Error: Invalid opcode %s\n", operator);
        CLOSE();
        return 1;
      }
    } else { // If opcode found
      if (strcmp(operand, "**") != 0) {
        address = findSymTab();
        if (address == -1) {
          printf("Error: Invalid symbol %s\n", operand);
          CLOSE();
          return 1;
        }
      } else address = -1;
    }
    if (opcode != -1) {
      if (address != -1) fprintf(objcode, "T^03^%02X%04X\n", opcode, address);
      else fprintf(objcode, "T^03^%02X0000\n", opcode);
    } else if (address != -1) fprintf(objcode, "T^03^%06s\n", lbl);
    fscanf(input, "%x %s %s %s", &address, label, operator, operand);
  }
  fprintf(objcode, "E^%06X", starting_address);
  CLOSE();
  return 0;
}


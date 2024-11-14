#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char label[20], operator[20], operand[20];
int loctr, start_add, temp;
char uumbi[20];
int searchOptab(char operator[]){
  FILE *optab = fopen("optab.txt","r");
  char test[20];
  int address;
  while (!feof(optab)){
    fscanf(optab,"%s\t%x",test,&address);
    if(strcmp(test,operator)== 0){
      return 1;
    }
  }
  return 0;
}
int main() {
  FILE *input, *symtab,*intermediate;
  input = fopen("input.txt", "r");
  intermediate = fopen("intermediate.txt","w");
  symtab = fopen("symtab.txt", "a+"); // Use append+read mode
  fscanf(input, "%s %s %s", label, operator, operand);
  if (strcmp(operator, "START") == 0) {
    loctr = (int)strtol(operand, NULL, 16);
    fprintf(intermediate,"%04X\t%s\t%s\t%s\n", loctr,label, operator, operand);
  } else {
    loctr = 0;
  }
  start_add = loctr;
  fscanf(input, "%s %s %s", label, operator, operand);
  while (strcmp(operator, "END") != 0 && !feof(input)) {
    fprintf(intermediate,"%04X\t%s\t%s\t%s\n", loctr, label, operator, operand);
    if (strcmp(label, "**") != 0) {
      int duplicate = 0;
      rewind(symtab); // Reset cursor for reading
      do{
        fscanf(symtab,"%s\t%x",uumbi,&temp);
        if (strcmp(uumbi, label) == 0) {
          printf("Error: Duplicate symbol '%s'.\n", label);
          fclose(symtab);
          fclose(input);
          fclose(intermediate);
            exit(0);
        }
      }
      while (!feof(symtab));
      fprintf(symtab, "%s\t%X\n", label, loctr); // Write to symtab
    }
    if (strcmp(operator, "BYTE") == 0) {
      loctr += strlen(operand) - 3;
    } else if (strcmp(operator, "WORD") == 0) {
      loctr += 3;
    } else if (strcmp(operator, "RESW") == 0) {
      loctr += 3 * atoi(operand);
    } else if (strcmp(operator, "RESB") == 0) {
      loctr += atoi(operand);
    } else if (searchOptab(operator)) {
      loctr += 3;
    } else {
      printf("error: invalid opcode\n");
      fclose(symtab);
      fclose(input);
      fclose(intermediate);
      exit(0);
    }
    fscanf(input, "%s %s %s", label, operator, operand);
  }
  fprintf(intermediate,"%04X\t%s\t%s\t%s\n", loctr, label, operator, operand);
  fclose(input);
  fclose(symtab);
  fclose(intermediate);
  return 0;
}



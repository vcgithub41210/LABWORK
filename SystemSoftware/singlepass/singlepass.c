#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *input,*object,*symtab,*optab,*reference;
int len_offset,loctr,start,opcode,address,text_offset,temp,count,sym_offset;
char label[20],operator[20],operand[20],search_add[20],mnemonic[20],symbol[20],lbl[20];

void close(){
  fclose(input);
  fclose(object);
  fclose(symtab);
  fclose(optab);
}
void TRavailable(){
  if(strcmp(operator,"RESW") == 0 || strcmp(operator,"RESB") == 0 || (strcmp(operator,"BYTE") == 0 || strcmp(operator,"WORD") == 0 ) && count < strlen(lbl) || (opcode != -1 && count < 3)){
    fprintf(object,"\n");
    temp = ftell(object);
    fseek(object,text_offset,SEEK_SET);
    fprintf(object,"%02X",30-count);
    fseek(object,temp,SEEK_SET);
    count = 30;
  }
  if(count == 30){
    if(strcmp(operator,"RESW") != 0 && strcmp(operator,"RESB") != 0){
      fprintf(object,"T^%06X^",loctr);
      text_offset = ftell(object);
      fprintf(object,"__");
    }
  }
}
void searchOpTab(){
  rewind(optab);
  while (!feof(optab)) {
    fscanf(optab, "%s %x", mnemonic, &opcode);
    if (strcmp(operator, mnemonic) == 0) return;
  }
  opcode = -1;
}
void searchSymTab(){
  address = -1;
  rewind(symtab);
  while(fscanf(symtab,"%s %s",symbol,search_add) != EOF){
    if(strcmp(symbol,operand) == 0){
      if(search_add[0] != '_') address = (int)strtol(search_add,NULL,16);
      else fprintf(reference,"%s\t%x\n",operand,ftell(object));
      return;
    }
  }
  fprintf(symtab,"%s\t______\n",operand);
  fprintf(reference,"%s\t%x\n",operand,ftell(object));
}
void forward_resolve(){
  temp = ftell(object);
  rewind(reference);
  while(fscanf(reference,"%s %x",symbol,&sym_offset) != EOF){
    if(strcmp(symbol,label) == 0){
      fseek(object,sym_offset,SEEK_SET);
      fprintf(object,"%04X",loctr);
    }
  }
  fseek(object,temp,SEEK_SET);
}
void insertSymTab(){
  rewind(symtab);
  while(fscanf(symtab,"%s",symbol) != EOF){
    if(strcmp(symbol,label) == 0){
      fprintf(symtab,"\t%06X\n",loctr);
      forward_resolve();
    }
    fscanf(symtab,"%*[^\n]\n",symbol);
  }
  fprintf(symtab,"%s\t%06X\n",label,loctr);
}

int main(){
  input = fopen("input.txt","r");
  object = fopen("object.txt","w+");
  optab = fopen("optab.txt","r");
  symtab = fopen("symtab.txt","w+");
  reference = fopen("reference.txt","w+");
  count = 30;
  fscanf(input,"%s %s %s",label,operator,operand);
  if(strcmp(operator,"START") == 0) start = (int)strtol(operand,NULL,16);
  else start = 0;
  loctr = start;
  fprintf(object,"H^% 6s^%06X^",label,loctr);
  len_offset = ftell(object);
  fprintf(object,"______\n");
  while(fscanf(input,"%s %s %s",label,operator,operand) != EOF){
    if(strcmp(operator,"END") == 0){ //end record
      fprintf(object,"\nE^%06X",start);
      fseek(object,len_offset,SEEK_SET);
      fprintf(object,"%06X",loctr-start);
      fseek(object,text_offset,SEEK_SET);
      fprintf(object,"%02X",30-count);
      close();
      return 0;
    }
    if(strcmp(label,"**") != 0) insertSymTab();
    searchOpTab();
    if(opcode == -1){
      sprintf(lbl,"");
      TRavailable();
      if(strcmp(operator,"RESW")==0) loctr += 3*atoi(operand);
      else if(strcmp(operator,"RESB")==0) loctr += atoi(operand);
      else if(strcmp(operator,"WORD") ==0) sprintf(lbl,"%06X",(int)strtol(operand,NULL,16));
      else if(strcmp(operator,"BYTE") == 0){
        for(int i = 2;operand[i] != '\'';i++) sprintf(lbl+strlen(lbl),"%02X",operand[i]);
        TRavailable();
      }
      else {
        close();
        return 0;
      }
      if(strcmp(lbl,"") != 0){
        fprintf(object,"^%s",lbl);
        loctr += strlen(lbl)/2;
        count -= strlen(lbl)/2;
      }
    }
    else{
      TRavailable();
      fprintf(object,"^%02X",opcode);
      if(strcmp(operand,"**") != 0){
        searchSymTab();
       if(address == -1) fprintf(object,"____");
       else fprintf(object,"%04X",address); 
      }
      else fprintf(object,"0000");
      count -=3;
      loctr += 3;
    }
  }
}


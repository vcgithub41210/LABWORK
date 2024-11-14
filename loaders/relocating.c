#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *input,*output;
char line[20],ipname[20],pgname[20],address[20],bitcode[3];
int i,j,starting_address,loctr,length,bitmask,actual_address;
int mask = 4095;
void close(){
  fclose(input);
  fclose(output);
}
int main(){
  input = fopen("relocatinginput.txt","r");
  output = fopen("output.txt","w");
  printf("Enter the name of the prgram: ");
  scanf("%s",ipname);
  fscanf(input,"%s",line);
  for(i = 2,j=0;line[i] != '^';i++,j++) pgname[j] = line[i]; 
  if(strcmp(ipname,pgname) != 0){
    printf("error; invalid prgram name:");
    close();
    return 0;
  }
  printf("Enter the starting address of the program: ");
  scanf("%x",&starting_address);
  while(fscanf(input,"%s",line) != EOF){
    if(line[0] == 'T'){
      for(i = 2,j = 0; line[i] != '^';i++,j++) address[j] = line[i];
      i += 4;
      for(i,j=0;line[i] != '^';i++,j++)bitcode[j] = line[i];
      bitmask = (int)strtol(bitcode,NULL,16);
      i++;
      loctr = (int)strtol(address,NULL,16);
      while(line[i] != '\0'){
        if(line[i] == '^')i++;
        else {
          if ((bitmask & mask) != 0){
      printf("%d\n\n",bitmask);
           actual_address = starting_address + loctr;
            bitmask <<= 1; 
          }
          else actual_address = loctr;
          fprintf(output,"%x\t%c%c\n",actual_address,line[i],line[i+1]);
          loctr += 1;
          i += 2;
        }
      }
    }
  }
  close();
  return 0;
}

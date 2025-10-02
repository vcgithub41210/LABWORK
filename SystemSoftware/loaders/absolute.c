#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *input,*output;
char line[20],ipname[20],pgname[20],address[20];
int i,j,starting_address,loctr,length;
void close(){
  fclose(input);
  fclose(output);
}
int main(){
  input = fopen("input.txt","r");
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
  while(fscanf(input,"%s",line) != EOF){
    if(line[0] == 'T'){
      for(i = 2,j = 0; line[i] != '^';i++,j++) address[j] = line[i];
      i += 4;
      loctr = (int)strtol(address,NULL,16);
      while(line[i] != '\0'){
        if(line[i] == '^')i++;
        else {
          fprintf(output,"%x\t%c%c\n",loctr,line[i],line[i+1]);
          loctr += 1;
          i += 2;
        }
      }
    }
  }
  close();
  return 0;
}

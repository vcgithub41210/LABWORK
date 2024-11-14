#include<stdio.h>

struct frame{
  int page;
  int freq;
  int fifo;
}frames[3];

int pages[10], pageFaults=0;

void print(struct frame frames[]){
  for(int i=0;i<3;i++){
    printf("%d\t",frames[i].page);
  }
  printf("\n");
}

void lfu(int n){
  for(int i=0;i<n;i++){
    int found=0;
    for(int j=0;j<3;j++){
      if(frames[j].page==pages[i]){
        printf("%d already exists!\n",pages[i]);
        found=1;
        frames[j].freq++;
        for(int p=0;p<3;p++){
          frames[p].fifo++;
        }
        print(frames);
        break;
      }
    }
    if(found==0){
      int min=0;
      for(int k=0;k<3;k++){
        if(frames[k].page==-1){
          min=k;
          break;
        }
        else if(frames[k].freq<frames[min].freq ||frames[k].freq==frames[min].freq && frames[k].fifo>frames[min].fifo){
          min=k;
        }
      }
      for(int p=0;p<3;p++){
        frames[p].fifo++;
      }
      frames[min].page=pages[i];
      frames[min].freq=1;
      frames[min].fifo=1;
      print(frames);
      pageFaults++;
    }
  }
}

void main(){
  int pg;
  printf("\nenter number of pages: ");
  scanf("%d",&pg);
  printf("\nenter page sequence: ");
  for(int i=0;i<pg;i++){
    scanf("%d",&pages[i]);
  }
  for(int i=0;i<3;i++){
    frames[i].page=-1;
    frames[i].freq=0;
    frames[i].fifo=0;
  }
  lfu(pg);
  printf("Page Faults=%d", pageFaults);
}

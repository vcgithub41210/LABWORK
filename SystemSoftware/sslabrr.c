#include <stdio.h>
#include <stdlib.h>
int pointer;
int process;
typedef struct node {
  int id;
  int at;
  int bt;
  int ct;
  int tempbt;
  int completed;
  int priority;
  int wt;
  int tat;
  struct node *next;
}nd;

nd *head = NULL;

void Display(){
  printf("\x1b[2J\x1b[HID\tAT\tBT\tPTY\tCT\tWT\tTAT\n");
  nd *temp = head;
  while(temp != NULL){
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",temp->id,temp->at,temp->bt,temp->priority,temp->ct,temp->wt,temp->tat);
    temp = temp->next;
  }
}

void Enter(){
  nd *temp = head;
  for(int i= 0; i < process; i++){
    nd *newnd = (nd*)malloc(sizeof(nd));
    printf("process %d arrival time: \n",i+1);
    scanf("%d",&newnd->at);
    printf("process burst time\n");
    scanf("%d",&newnd->bt);
    newnd->tempbt = newnd->bt;
    printf("process priority");
    scanf("%d",&newnd->priority);
    newnd->id = i;
    newnd->completed = 0;
    newnd->wt = 0;
    newnd->tat = 0;
    newnd->ct = 0;
    if(temp == NULL)head = temp = newnd;
    else{
      temp->next = newnd;
      temp = newnd;
    }
  }
}
void SoID(){
  nd *temp = head;
  while(temp != NULL){
    nd *small = temp;
    nd *temp2 = temp->next;
    while(temp2 != NULL){
      if(temp2->id < small->id)small = temp2;
      temp2 = temp2->next;
    }
    if(temp != small){
      int temp_ct = temp->ct;
      temp->ct = small->ct;
      small->ct = temp_ct;
      int temp_tat = temp->tat;
      temp->tat = small->tat;
      small->tat = temp_tat;
      int temp_wt = temp->wt;
      temp->wt = small->wt;
      small->wt = temp_wt;

      int temp_id = temp->id;
      temp->id = small->id;
      small->id = temp_id;
      int temp_at = temp->at;
      temp->at = small->at;
      small->at = temp_at;
      int temp_bt = temp->bt;
      temp->bt = small->bt;
      small->bt = temp_bt;
      int temp_tempbt = temp->tempbt;
      temp->tempbt = small->tempbt;
      small->tempbt = temp_tempbt;
      int temp_priority = temp->priority;
      temp->priority = small->priority;
      small->priority = temp_priority;
    }
    temp = temp->next;
  }
}
void Sort(){
  nd *temp = head;
  while(temp != NULL){
    nd *small = temp;
    nd *temp2 = temp->next;
    while(temp2 != NULL){
      if(temp2->at < small->at)small = temp2;
      temp2 = temp2->next;
    }
    if(temp != small){
      int temp_id = temp->id;
      temp->id = small->id;
      small->id = temp_id;
      int temp_at = temp->at;
      temp->at = small->at;
      small->at = temp_at;
      int temp_bt = temp->bt;
      temp->bt = small->bt;
      small->bt = temp_bt;
      int temp_tempbt = temp->tempbt;
      temp->tempbt = small->tempbt;
      small->tempbt = temp_tempbt;
      int temp_priority = temp->priority;
      temp->priority = small->priority;
      small->priority = temp_priority;
    }
    temp = temp->next;
  }
}
void CheckArrival(nd *array[],int ct){
  nd *temp = head;
  while(temp != NULL){
    if(temp->at <= ct && temp->completed == 0){
      pointer++;
      array[pointer] = temp;
    }
  temp = temp->next;}
}
int AComplete(){
  nd *temp = head;

  while(temp != NULL){
    if(temp->completed == 0)return 0;
    temp = temp->next;
  }
  return 1;
}
nd *FindExecutable(nd *array[]){
  nd *max = array[0];
  for(int i = 1; i < pointer; i++){
    if(array[i]->priority < max->priority)max = array[i];
  }
  return max;
}
void Execute(){
  int cT = head->at;
  while(!AComplete()){
    nd *array[process];
    pointer = -1;
    CheckArrival(array,cT);
    if(pointer != -1){
      nd *exc = FindExecutable(array);
      cT++;
      exc->tempbt -= 1;
      if(exc->tempbt == 0){
        exc->ct = cT;
        exc->tat = cT - exc->at;
        exc->wt = exc->tat - exc->bt;
        exc->completed = 1;
      }
    }
    else cT++;
  }
}
int main(){
  printf("enter process: ");
  scanf("%d",&process);
  Enter();
  Sort();
  Execute();
  SoID();
  Display();
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int display(int *need,int *allocated,int *available){
	printf("\x1b[2J\x1b[Hprocess\t\tneed\t\tallocated\t\tavailable\n");
	for(int i = 0; i < sizeof(need)/sizeof(need[0]);i++){
		printf("%d\t\t",i+1);
		for(int j = 0; j< 3; j++){
			printf("%d  ",*(need+i+j));
		}
		printf("\t\t");
		for(int j = 0; j < 3;j++){
			printf("%d  ",*(allocated+i+j));
		}
		printf("\t\t");
		if(i == 0){
			for(int j = 0; j < 3; j++){
				printf("%d  ",*(available+j));
			}
		}
		printf("\n");
	}
}

void calculate_available(int pr,int *allocated,int *max,int *available){
	for(int i = 0; i < 3; i++){
		*(available+i) = *(max+i);
	}
	for(int i = 0; i < pr;i++){
		for(int j = 0;j<3;j++){
			*(available+j) -= *(allocated+i+j);
		}
	}
}

int main(){
	int pr;
	printf("Enter the number of processes: ");
	scanf("%d",&pr);
	int *need = (int*)malloc(3*pr*sizeof(int));
	int *allocated = (int*)malloc(3*pr*sizeof(int));
	int *available = (int*)malloc(3*sizeof(int));
	int *max = (int*)malloc(3*sizeof(int));
	for(int i = 0; i < pr ; i++){
		for(int j= 0; j < 3;j++){
			printf("Enter the need for resource %d for process %d",j+1,i+1);
			scanf("%d",need+i+j);
		}
	}
	for(int i = 0; i < pr; i++){
		for(int j = 0; j < 3;j++){
			 printf("Enter the allocation of resource %d for process %d",j+1,i+1);
			 scanf("%d",allocated+i+j);
		}
	}
	for(int i = 0; i < 3;i++){
		printf("Enter the maximum instance for resource %d",i+1);
		scanf("%d",max+i);
	}

	calculate_available(pr,allocated,max,available);
	display(need,allocated,available);

}

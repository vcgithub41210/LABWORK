#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_turn_around_time(int turn_around_time[],int waiting_time[],int burst_time[],int number_of_processes){
	for(int i = 0; i < number_of_processes;i++){
		turn_around_time[i] = waiting_time[i] + burst_time[i];
	}
}
int total_completion_time(int burst_time[],int number_of_processes){
	int ct = 0;
	for(int i = 0; i < number_of_processes;i++){
		ct += burst_time[i];
	}
	return ct;
}
int find_latest_process(int time,int arrival_time[],int latest_p_index,int number_of_processes){
	for(int i = latest_p_index+1; i < number_of_processes;i++){
		if(arrival_time[i] <= time){
			latest_p_index = i;
		}
		else{
			break;
		}
	}
	return latest_p_index;
}
int waiting_time_incrementer(int current_index,int remaining_bt[],int waiting_time[],int number_of_processes,int seconds){
	for(int i = 0; i < number_of_processes;i++){
		if(remaining_bt[i] != 0 && i != current_index){
			waiting_time[i] += seconds;
		}
	}
}
int calculate(int process_id[],int burst_time[],int arrival_time[],int time_quantum,int number_of_processes){
	int *waiting_time = malloc(number_of_processes * sizeof(int));
	int *turn_around_time = malloc(number_of_processes * sizeof(int));
	for(int i = 0 ; i < number_of_processes;i++){
		waiting_time[i] = 0 - arrival_time[i];
	}
	int *remaining_bt = malloc(number_of_processes * sizeof(int));
	memcpy(remaining_bt,burst_time,number_of_processes * sizeof(int));
	int total_ct = total_completion_time(burst_time,number_of_processes);
	int time = 0;
	int current_index = 0;
	int latest_p_index = find_latest_process(time,arrival_time,-1,number_of_processes);
	while(time != total_ct){
		if(remaining_bt[current_index] < time_quantum){
			time += remaining_bt[current_index];
			waiting_time_incrementer(current_index,remaining_bt,waiting_time,number_of_processes,remaining_bt[current_index]);
			remaining_bt[current_index] = 0;
		}
		else{
			time += time_quantum;
			waiting_time_incrementer(current_index,remaining_bt,waiting_time,number_of_processes,time_quantum);
			remaining_bt[current_index] -= time_quantum;
		}
		if(latest_p_index != number_of_processes-1)
			latest_p_index = find_latest_process(time,arrival_time,latest_p_index,number_of_processes);
		do{
			current_index = (current_index + 1) % (latest_p_index+1);
		}while(remaining_bt[current_index] == 0 && time != total_ct);
	}
	find_turn_around_time(turn_around_time,waiting_time,burst_time,number_of_processes);
	printf("Process\tArrivalTime\tBurstTime\tWaitingTime\tTurnAroundTime\n");
	for(int i= 0;i<number_of_processes;i++){
		printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n",process_id[i],arrival_time[i],burst_time[i],waiting_time[i],turn_around_time[i]);
	}
}
int main(){
	int time_quantum = 2;
	int process_id[] = {1,2,3,4,5};
	int burst_time[] = {8,2,3,2,6};
	int arrival_time[] = {0,1,1,3,4};
	int number_of_processes = sizeof(process_id)/sizeof(process_id[0]);
	calculate(process_id,burst_time,arrival_time,time_quantum,number_of_processes);
}

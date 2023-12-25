#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	int a;
	for(int i=0; i<5; i++){
		if(fork() == 0){
			char* const* b = NULL;
			printf("KIND\n");
			execl("kind","a",NULL);
			break;
		}else{
			printf("VATER\n");
		}
		
		
	}
	
	for(int i=0; i<5; i++){
		int b = wait(NULL);
		printf("Kindprozess tot PID: %d \n", b);
	}
}

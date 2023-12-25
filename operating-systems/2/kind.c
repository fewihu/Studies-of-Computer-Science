#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	
	FILE* f;
	int num = 7;
	for(int i=0; i<20000; i++){
		f = fopen("zahl.dat", "r");
		fscanf (f, "%d", &num);

			//printf("%d\n",num);
		num++;
			//printf("%d\n",num);
		fclose(f);
		f = fopen("zahl.dat", "w");
		if(fprintf (f, "%d\n",num)<0){
			printf("Schreibefehler\n");
			exit(-1);
		}
		fclose(f);

	}
	exit(0);
}

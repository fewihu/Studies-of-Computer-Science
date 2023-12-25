#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	union semun {
        int              val;
		struct semid_ds *buf;
		unsigned short  *array;
		struct seminfo  *__buf;
	}arg;

	
	int semid, key, perms;
    struct sembuf sops;

	semid=atoi(argv[0]); //Argument durch Vaterprozess erhalten
	key = IPC_PRIVATE;
    perms = 0600; //Besitzer darf lesen und schreiben

	printf("KIND: ich habe von meinem Vater die ID %d für den Semaphor erhalten.\n", semid);
	

	printf("KIND: Semaphor erzeugt\n");
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1){
		printf("Fehler bei Initialisieren des Semaphors\n");
		exit(-1);
	}else{
		printf("KIND: Semaphor initialisiert\n");
		sops.sem_num = 0;
		sops.sem_op =  1;
		sops.sem_flg = 0;
			
		printf("KIND: warte kurz\n");
		sleep(5);
			
		if(semop(semid, &sops, 1) == -1){
			printf("Fehler bei P-Operation\n");
			exit(-1);
		}else{
			printf("KIND: Habe meinem Vater die Freigabe erteilt und beende mich\n");
			exit(0);
		}	
	}

}

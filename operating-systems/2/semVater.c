#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h> 

int main(){
	
	union semun {
        int              val;
		struct semid_ds *buf;
		unsigned short  *array;
		struct seminfo  *__buf;
	}arg;

	int semid, key, perms;
    struct sembuf sops;
    key = IPC_PRIVATE;
    perms = 0600; //Besitzer kann lesen und schreiben
	
	//Semaphor erzeugen
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | perms);

    if (semid != -1) {
		printf("VATER: Semaphor mit ID %d erzeugt\n",semid);
        arg.val = 0;
        if (semctl(semid, 0, SETVAL, arg) == -1)
            exit(-1);
        printf("VATER: Semaphor initialisiert\n");

		//ID in char* umwandeln
		char* str = malloc(sizeof(char*));
		sprintf(str, "%d", semid);
	
		//Kindprozess erzeugen
		int ret = fork();
		if(ret == 0){
			//Kind
			execl("semKind", str, "a", NULL);
		}else if(ret == -1){
			printf("Fehler bei fork()\n");
			exit(-1);
		}else{
			//Vater führt V-Operation aus
			sops.sem_num = 0;
			sops.sem_op = -1;
			sops.sem_flg = 0;
			if(semop(semid, &sops, 1) == -1){
				printf("Fehler bei V-Operation");
				exit(-1);
			}else{
				printf("VATER: habe Freigabe erhalten, schließe Semaphor und beende mich\n");
				int pid = wait(NULL);
				printf("VATER: Mein Sohn (%d) ist tot\n",pid);
				semctl(semid, 0, IPC_RMID);
				exit(0);
			}	
		}
	}else{
		printf("VATER: Fehler beim Initialisieren des Semaphors");
		exit(-1);
	}
}

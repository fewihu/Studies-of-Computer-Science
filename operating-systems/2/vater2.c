#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <semaphore.h>
 
#include <fcntl.h>

#include <string.h>



#define SHM_SIZE 4
#define OPERATIONS 100
#define RESSFILE "zahl.dat"

/*SHMS-pointer*/
char *adrshm;
/*SHMS-key*/
int shmid;
/*reader count*/
int rc;

/*semaphor keys*/
int mutex, w, start;


union semun {
  int val;                    /* value for SETVAL */
  struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array for GETALL, SETALL */
  struct seminfo *__buf;      /* buffer for IPC_INFO */
};

void write_file(void)
{
  FILE *fin;
  int ret;
  unsigned long x;

  fin = fopen(RESSFILE, "r+");
  if (fin == NULL) {
    perror("fopen for write");
	exit(EXIT_FAILURE);
  }

  ret = fscanf(fin, "%lu", &x); /* returns # of converted items */
  if (ret != 1) {
    perror("fscanf during write access");
	exit(EXIT_FAILURE);
  }

  x+=1;
  rewind(fin);
  ret = fprintf(fin, "%ld\n", x);
  if (ret == -1){
    perror("fprintf");
	exit(EXIT_FAILURE);
  }
  
  ret = fclose(fin);
  if (ret == EOF) {
    perror("fclose");
	exit(EXIT_FAILURE);
  }
}

void Init(int semid, int index, int value){
	int ret;
	union semun arg;

	arg.val = 1;
	if ((ret = semctl(semid, index, SETVAL, value)) == -1) {
		perror("sem Init failed");
		exit(EXIT_FAILURE);
	}
}

void P(int semid, int index){
	int ret;
	struct sembuf op;

	op.sem_num = index;
	op.sem_op = -1;
	op.sem_flg = 0;
	if ((ret = semop(semid, &op, 1)) == -1) {
		perror("sem P() failed");
		exit(EXIT_FAILURE);
	}
}

void V(int semid, int index){
	int ret;
	struct sembuf op;

	op.sem_num = index;
	op.sem_op = +1;
	op.sem_flg = 0;
	if ((ret = semop(semid, &op, 1)) == -1) {
		perror("sem V() failed");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char * argv[]){
	
	key_t key = ftok("/vater.c", 0);
	int ret;
	int numberR = atoi(argv[1]);
	int numberW = atoi(argv[2]);
	int forkcount = 0;
	FILE* fin;
	
	/*prepare file*/
	fin = fopen(RESSFILE, "w");
	if (fin == NULL) {
		perror("main, fopen for write");
		exit(EXIT_FAILURE);
	}
	fprintf(fin, "%d\n", 0);
	fclose(fin);
	
	/*get shared memory segment*/
	shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666);
	if(shmid == -1){
		perror("shmget father");
		exit(EXIT_FAILURE);
	}
	
	if((adrshm = (char*) shmat(shmid, NULL, 0)) == (void*) -1){
		perror("shmat father");
		exit(EXIT_FAILURE);
	}
	
	/*initialize mutex semaphore*/
	if ((mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1) {
		perror("semget mutex");
		exit(EXIT_FAILURE);
	} 
	/* init the semaphore (set) as open */
	Init(mutex, 0, 1);
	
	/*initialize mutex semaphore*/
	if ((w = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1) {
		perror("semget w");
		exit(EXIT_FAILURE);
	} 
	/* init the semaphore (set) as open */
	Init(w, 0, 1);
	
	/*initialize start semaphore*/
	if ((start = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1) {
		perror("semget start");
		exit(EXIT_FAILURE);
	} 
	/* init the semaphore (set) and close it */
	Init(mutex, 0, 1);
	printf("VATER: vor Start\n");
	V(start, 0);
	semctl(start, 0, SETVAL, 1);
	printf("VATER: nach Start\n");
	
	for(int i = 0; i < numberR; i++){
		/*fork processes*/
		ret = fork();
		if(ret == -1){
			/*error*/
			perror("fork");
			exit(EXIT_FAILURE);
		}else if(ret == 0){
			/*son*/
			printf("Ich bin ein Leser\n");
			int semvalue = 0;
			
			/*wait until father signals via semaphore to start*/
			while(1){
				semvalue = semctl(start, 0, GETVAL, 0);
				if(semvalue != 1)
					break;
			}
			
			int opcount = 0;
			char* out = malloc(sizeof(char*));
			char c;
			while(opcount < OPERATIONS){
					
				opcount++;
				P(mutex, 0);
				strcpy(out,adrshm);
				rc = atoi(out);
				rc++;
				c = ((char)rc) + 48;
				strcpy(out,&c);
				out[1]='\0';
				strcpy(adrshm,out);
				printf("rc: %s\n",adrshm);
					
				if(rc == 1){
					P(w, 0);
					printf("Ich bin der erste Leser und schließe die Tür ab\n");
				}
					
				V(mutex, 0);
				printf("Leser %d liest zum %d-ten mal.\n",(forkcount+1),opcount);
				//sleep(1);
				P(mutex, 0);
				rc--;
				c = ((char)rc) + 48;
				//printf("char c %c\n",c);
				strcpy(out,&c);
				out[1]='\0';
				//printf("char* out %s\n",out);
			
				if(rc == 0){
					V(w, 0);
					printf("Ich war der letzte Leser und mache das Licht aus\n");
				}
				strcpy(adrshm,out);
				//printf("schreibe %s in SHM\n",adrshm);
				V(mutex, 0);
			}
			free(out);
			exit(EXIT_SUCCESS);
		}else{
			/*father*/
			printf("VATER: Leser erzeugt\n");
			forkcount ++;
		}
	}	
	
	for(int i = 0; i < numberW; i++){
		/*fork processes*/
		ret = fork();
		if(ret == -1){
			/*error*/
			perror("fork");
			exit(EXIT_FAILURE);
		}else if(ret == 0){
			/*son*/
			printf("Ich bin ein Schreiber\n");
			int semvalue = 0;
			
			/*wait until father signals via semaphore to start*/
			while(1){
				semvalue = semctl(start, 0, GETVAL, 0);
				if(semvalue != 1)
					break;
			}
			
			int opcount = 0;
			while(opcount < OPERATIONS){
				P(w,0);
				/*writing*/
				opcount ++;
				printf("Schreiber %d schreibt zum %d-ten mal\n",(forkcount-numberR+1),opcount);
				write_file();
				//sleep(1);
				V(w,0);
			}
				
			/*kill son*/
			exit(EXIT_SUCCESS);

		}else{
			/*father*/
			printf("VATER: Schreiber erzeugt\n");
			forkcount ++;
		}
		
	}
	
	/*start reader and writers*/
	//printf("VATER: ich gebe in 2 Sekunden das Startsignal\n");
	//sleep(2);
	printf("VATER: Start!\n");
	semctl(start, 0, SETVAL, 0);	

	/*rest of father*/
	for(int i = 0; i <(numberR+numberW); i++){
				
		int ret = wait(NULL);
				
		if(ret != -1){
			printf("VATER: Sohn %d hat sich beendet\n", ret);
		}
	}
			
	/*detach and destroy shared memory segment*/
	if((ret = shmdt(adrshm)) == -1){
		perror("shmdt()");
		exit(EXIT_FAILURE);
	}	
			
	/*destroy mutex semaphore*/
	if ((ret = semctl(mutex, 0, IPC_RMID)) == -1) {
		perror("semctl(), IPC_RMID");
	}
 
	/*destroy w semaphore*/
	if ((ret = semctl(w, 0, IPC_RMID)) == -1) {
		perror("semctl(), IPC_RMID");
	}
	
	/*destroy* start semaphore*/
	if ((ret = semctl(start, 0, IPC_RMID)) == -1) {
		perror("semctl(), IPC_RMID");
	}
	return 0;
}

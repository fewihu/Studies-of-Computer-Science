#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#define PROCESSES 5
#define RESSFILE "zahl.dat"
#define ITERATIONS 20000
#define PROJECT_ID 42

union semun {
	int val; 				/* Value for SETVAL */
	struct semid_ds *buf; 	/* Buffer for IPC_STAT, IPC_SET */
	unsigned short *array; 	/* Array for GETALL, SETALL */
	struct seminfo *__buf; 	/* Buffer for IPC_INFO (Linux-specific) */
};

void access_file(void){
	FILE *fin;
	int ret;
	unsigned long x;
	
	fin = fopen(RESSFILE, "r+");
	if (fin == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	ret = fscanf(fin, "%ld", &x); // returns # of converted items
	if (ret != 1) {
		perror("fscanf");
		exit(EXIT_FAILURE);
	}
	
	x++;
	
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
	
	
	if(x==1){
		abort();
	}
}

int main(int argc, char * argv[]){
	pid_t son[PROCESSES], tmppid;
	int c, c2, ret, semid, aborts;
	key_t semkey;
	union semun mysem;
	struct sembuf mysemop;
	FILE *fin;

	/* create file with a single ’0’ in it */
	fin = fopen(RESSFILE, "w");
	if (fin == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	fprintf(fin, "0");
	fclose(fin);

	/* generate a key to identify a semaphore set */
	semkey = ftok("/etc/passwd", PROJECT_ID);
	if (semkey == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	printf("semaphore key = %d\n", semkey);

	/* create the semaphore ’set’ */
	semid = semget(semkey, 1, IPC_CREAT|0600);
	if (semid == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	/* init the semaphore as "open" */
	mysem.val = 1;
	ret = semctl(semid, 0, SETVAL, mysem);
	if (ret == -1) {
		perror("semctl/IPC_SET");
		semctl(semid, 0, IPC_RMID);
		exit(EXIT_FAILURE);
	}
	
	for (c=0; c<PROCESSES; c++) {
		tmppid = fork();
		if (tmppid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
		}
		
		if (tmppid != 0) {
			/* Father */
			son[c] = tmppid;
		}else{
			
			/* Son */

			for (c2=0; c2<ITERATIONS; c2++) {
				mysemop.sem_num = 0;
				mysemop.sem_op = -1;
			
				/* P() */
				mysemop.sem_flg = SEM_UNDO; // Semaphor Adjustment Value to avoid deadlock if process aborts
				ret = semop(semid, &mysemop, 1);
				if (ret == -1){
					perror("semop/-1");
					exit(EXIT_FAILURE);
				}


				/* kritischer Abschnitt */
				access_file();

				/* V() */
				mysemop.sem_num = 0;
				mysemop.sem_op = 1;
				/* V() */
				mysemop.sem_flg = 0;
				ret = semop(semid, &mysemop, 1);
				if (ret == -1){
					perror("semop/+1");
					exit(EXIT_FAILURE);
				}
			}
		exit(EXIT_SUCCESS);
		}
	}
	
	
	/* only father reaches this */
	for (c=0; c<PROCESSES; c++) {
	tmppid = wait(&ret);
		printf("Son with PID %ld returned %d.\n", tmppid, ret);
	}
	ret = semctl(semid, 0, IPC_RMID);
	if (ret == -1) {
		perror("semctl/IPC_RMID");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

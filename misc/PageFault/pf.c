/*
zu uebersetzen mit gcc *.c -D _GNU_SOURCE -D LARGE_SEG=X -D FINE_SEG=Y 
_GNU_SOURCE damit Semaphorenoperationen mit Timeout funktionieren
X ... # Segmente (in GiB) die eher zügig belegt werden
Y ... # Segmente (in MiB) die sehr vorsichtig belegt werden
*/

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <sys/sem.h>

#include "ccl.h"

/*Synchronisation Sem*/
#define LOCK       -1
#define UNLOCK      1
#define PERM 0666     


int remSem(int semid) {
	
	if(semctl(semid, 0, IPC_RMID, 0) == -1) {
      	perror("semctl failed (remove)");
      	return 0;
	} 
	
   	printf("semaphor %d destroyed\n",semid);
   	return 1;
}

int operateSem(int operation, int semid) {
	
	struct sembuf semaphore;
	semaphore.sem_num = 0;
    semaphore.sem_op = operation;
   	semaphore.sem_flg = SEM_UNDO;
   	
   	if(semop(semid, &semaphore, 1) == -1) {
    	perror("semop");
    	return 0;
   	}
   	
   	return 1;
}

int operateSem_TO(int operation, int semid, int timeout) {

	struct sembuf semaphore;
	semaphore.sem_num = 0;
    semaphore.sem_op = operation;
   	semaphore.sem_flg = SEM_UNDO;
   	
   	struct timespec timeoutSpec;
   	timeoutSpec.tv_sec = timeout;
   	timeoutSpec.tv_nsec = 0;
   	
   	if(semtimedop(semid, &semaphore, 1, &timeoutSpec) == -1) {
    	printf("Semop timeout occured: OOM probably killed the child\n");
   	}
   	
   	return 1;
}

int createSem(long KEY) {
   
		int semid = semget (KEY, 1, IPC_CREAT | IPC_EXCL | PERM);
      	
      	if(semid < 0) {
			perror("semget failed");
      		exit(-1);
      	}
      
      	printf ("semaphor created: %d\n", semid);

      	if(semctl(semid, 0, SETVAL, (int) 1) == -1) {
      		perror("semctl failed (init)");
       		exit(-1);
		}
		
		return semid;
}

int main() {

	/*Taktzyklen messen*/
	uint64_t start, stop;
  	unsigned long long duration;
	
	/*Prozesse identifizieren*/
	int pid;
	
	/*Semaphore*/
	int semid1 = createSem(ftok(".", 1));
	int semid2 = createSem(ftok(".", 2));
	int semid3 = createSem(ftok(".", 3));
	
	/*Semaphore sind initial geschlossen, damit werden Prozesse blockiert und zu gegebener Zeit freigegeben*/
	if(!operateSem(LOCK, semid1) || !operateSem(LOCK, semid2) || !operateSem(LOCK, semid3))
		exit(-1);
			
			
	if((pid = fork()) < 0) {
    	perror("fork failed");
    	exit(-1);
	} else if(pid == 0) {
    	
    	/*Kind*/
    	
    	/*versuche Semaphor 1 zu schließen -> blockiert bis Vater sein GiB Speicher erhalten hat*/
   		if(!operateSem(LOCK, semid1))
   			exit(-1);
    	
    	/*LARGE_SEG GB Speicher holen und referenzieren (es sollten ca. 3 GiB HS überbleiben)*/
    	void* p[LARGE_SEG];
    	
    	for(int i = 0; i < LARGE_SEG; i++) {
        	       	
        	p[i] = malloc(1024 * 1024 * 1024);
        	
        	if(p == NULL) {
        		perror("malloc failed");
        		exit(-1);
        	}
        	
        	for (int j = 0; j < 1024 * 1024 ; j++)
            	(*(char*)(p[i] + (1024*j))) = 1;
        	        	
        	printf("greedy child consumes the %2d. GiB of main memory\n",i+1);
        	
        	sleep(1); /* ist notwendig, da der OOM sonst zuschlägt */
    	}
    	
    	/* allozieren des letzten bisschen Speichers in Schritten von 1 MiB 
    	(Menge durch Define FINE_SEG bestimmen) so dass das Kind den OOM überlebt*/
    	
    	printf("greedy child consumes the least little bit (%d MiB) of main memory, that can take up to %.0f seconds\n", FINE_SEG, 0.1*FINE_SEG);
    	
    	void* p2[FINE_SEG];
    	for(int j = 0; j < FINE_SEG; j++) {
    		
    		usleep(100000); /*Das ist wieder notwendig um den OOM zu beruhigen*/
    		
    		p2[j] = malloc(1024 * 1024);
    		for(int i = 0; i < 1024; i++) 
    			*(char*)(p2[j] + (1024*i)) = 1;
    	}
    	     	    	
    	/* Semaphor 2 öffnen -> Vater kann Seiten referenzieren 
    	und Versuch Semaphor 3 zu schließen -> blockiert bis Vater Speicherzugriffe vermessen hat */
    	if(!operateSem(UNLOCK, semid2) || !operateSem(LOCK, semid3))
   			exit(-1);
   		    	    
    	    
    	/*Speicher regulär freigeben*/
    	for(int i = 0; i < LARGE_SEG; i++)
    		free(p[i]);
    	
    	for(int i = 0; i < FINE_SEG; i++)
    		free(p2[i]);
    	
    	printf("child terminates regularly\n");  	
    	exit(0);
    	/*ENDE Kind*/
    	
	} else {
    	
    	/*Vater*/
					
		/*1 GiB Speicher reservieren und referenzieren -> entsprechende Seiten sind eingelagert*/
		char* mem = malloc(1024 * 1024 * 1024);
	
		for(int i = 0; i < 1024 * 1024; i++) {
			mem[i * 1024] = 1; 
		}
								
		printf("father got his 1 GiB \nchild (pid:%d) now tries to force page fault\n", pid);

		/*Semaphor 1 öffnen -> Kind kann seinerseits Speicher belegen*/
		/*Semop Timeout ergibt sich aus definierten Speichergrößen*/
		if(!operateSem(UNLOCK, semid1) || !operateSem_TO(LOCK, semid2, LARGE_SEG+FINE_SEG/10))
			exit(-1);

		
 		/*HS aller 32MiB referenzieren und Zugriffszeit messen*/ 		
 		for(int i = 0; i < 32 ; i++) {
						
			start = rdtsc();
			mem[i * 1024 * 1024 * 32] = 1; 
			stop = rdtsc();
			
			duration=stop-start;
  			printf("Length in Cycles: %7llu for %2d. 32MiB segment\n", duration, i+1);
		}
 	 	
		/*Semaphor 3 öffnen -> Kindprozess kann terminieren*/
		if(!operateSem(UNLOCK, semid3))
   			exit(-1);
		
		if(wait(NULL) == -1) 
			perror("wait failed");
		else
			printf("child: %d terminated\n", pid);
		
		/*Semaphore aus dem Speicher entfernen und Speicher freigeben*/
		remSem(semid1);
		remSem(semid2);
		remSem(semid3);
		free(mem);
		exit(0);
	}
}

/*
	Tool für den Kasiski Test
	findet gleiche Folgen von Chiffratzeichen und bestimmt ihren Abstand
	--> Primfaktoren der Abstände sind Hinweis auf Schlüssellänge Vigenere-verschlüsselter Texte
	
	Übersetzen mit: gcc -o kasiski3 kasiski3.c -Wall -lm -pthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>

#define MINLENGTH 3
#define MAXLENGTH 100

/*globale Variable*/

char* cipherbuf;
long flength;

int prims_values[24] 	= {2,3,5,7,11,13,17,19,23,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97}; 
int prims_contained[24];
int prims_notContained[24];

pthread_mutex_t thread_mutex;// = PTHREAD_MUTEX_INITIALIZER;

void initialize_prims(){
	int i;
	for(i = 0; i < 24; i++){
		prims_contained[i] 	  = 0;
		prims_notContained[i] = 0;
	}
}

void primfact(int d){

	int i, contained;
	
	for(i = 0; i < 24; i++){	
		contained = 0;
		
		/*Abstand durch i-te Primzahl teilbar*/
		while(d % prims_values[i] == 0){
			prims_contained[i]++;
			contained++;
			d = d / prims_values[i];
		}
		
		if(contained == 0){
			prims_notContained[i]++;
		}
	}
}

void freq_prims(){
	
	int i;
	for(i = 0; i < 24; i++){
		prims_contained[i] = prims_contained[i] - prims_notContained[i];
	}
}
	
void read_cipher(char* fname){
	
	FILE* fin;
	int c, pos;
	
	/*Datei eröffnen*/
	fin = fopen(fname, "r");
	if(!fin){
		perror("fopen\n");
		exit(EXIT_FAILURE);
	}
	
	/*Länge der Datei bestimmen -> ausreichend Speicher holen*/
	fseek(fin, 0, SEEK_END);
	flength = ftell(fin);
	cipherbuf = malloc(flength + 1);
	if(!cipherbuf){
		perror("malloc\n");
		exit(EXIT_FAILURE);
	}
	rewind(fin);
	
	/*Datei in Speicher kopieren (nur Buchstaben)*/
	pos = 0;
	while(!feof(fin)){
		c = fgetc(fin);
		if(isalpha(c)){
			cipherbuf[pos++] = c;
		}
	}
	fclose(fin);
	cipherbuf[pos] = 0;
}

void print_buf(char* buf){
	
	int pos = 0;
	
	/*formatierte Ausgabe*/
	while(cipherbuf[pos]){
		printf("%c", cipherbuf[pos]);
		pos++;
		if(pos % 125 == 0){
			printf("\n\n");
		}else{
			if(pos % 25 == 0){
				printf("\n");
			}else{
				if(pos % 5 == 0){
					printf(" ");
				}
			}
		}
	}
	printf("\n\n");
}

void search_ngrams(void* args){
	
	int startpos, dist, num;
	int ngram_lngth = *((int*) args);
	char ngram_buf[ngram_lngth];
	char* ret;
	
	num 	 = 0;	
	startpos = 0;
	while(cipherbuf[startpos] != 0){
		
		/*Folge festlegen, nach der gesucht wird*/				
		strncpy(ngram_buf, &cipherbuf[startpos], ngram_lngth);
		ngram_buf[ngram_lngth] = 0;
					
		/*Suche im Rest des Chiffrats nach Folge*/
		ret = strstr(&cipherbuf[startpos] + ngram_lngth, ngram_buf);

		while(ret){
			dist = ret - &cipherbuf[startpos];
			num++;
			
			/*Primfaktoren ermitteln und in Array ablegen*/
			pthread_mutex_lock(&thread_mutex);
			primfact(dist);
			pthread_mutex_unlock(&thread_mutex);
			
			/*Rest neu bestimmen*/
			ret = strstr(ret + 1, ngram_buf);
		}
		startpos ++;
	}
		
	if(num > 0){
		printf("%d Folgen der Länge %d gefunden\n", num, ngram_lngth);
	}
	
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	int search_depth, i, ret;
	pthread_t thread_id[100];
	int depth[100];
	
	/*Array für die Folgenlänge*/
	for(i=0; i<100; i++){
		depth[i] = i; 
	}
	
	/*argv[1] Dateiname*/
	read_cipher(argv[1]);
	print_buf(cipherbuf);
	
	/*Schätzung bis zu welcher Folgenlänge die Suche sinnvoll ist*/	
	search_depth = 0.75 * sqrt((double)strlen(cipherbuf)) ;
	
	/*maximale Folgenlänge 100*/
	if(search_depth > MAXLENGTH){
		printf("Suche bis Schlüssellänge: %d mehr wäre möglichweise sinnvoll\n", MAXLENGTH);
		search_depth = MAXLENGTH;
	}else{
		printf("Suche bis Schlüssellänge: %d\n\n", search_depth);
	}
	
	/*Mutex zum Sperren des Schreibzugriffs auf Primfaktoren*/
	ret = pthread_mutex_init(&thread_mutex, NULL);
	if(ret != 0){
		perror("pthread_mutex_init\n");
		exit(EXIT_FAILURE);
	}

	/*Primzahlen laden*/
	initialize_prims();

	/*eigentliche Suche*/
	for(i= search_depth; i > MINLENGTH; i--){
		ret = pthread_create(&(thread_id[i]), NULL, (void*) &search_ngrams, &depth[i]);
		if(ret != 0){
			perror("pthread_create\n");
			exit(EXIT_FAILURE);
		}
	}
	
	for(i= search_depth; i > MINLENGTH; i--){
		pthread_join(thread_id[i], NULL);
	}

	/*Mutex zerstören*/
	ret = pthread_mutex_destroy(&thread_mutex);
	if(ret != 0){
		perror("pthread_mutex_destroy\n");
		exit(EXIT_FAILURE);
	}

	/*Häufigkeit der Primfaktoren der Folgenabstände ermitteln*/
	freq_prims();
	for(int i = 0; i < 24; i++){
		if(prims_contained[i] > 0){
			printf("\nPrimfaktor der Schlüssellänge: %d Vorkommen: %d\n",prims_values[i], prims_contained[i]);
		}
	}
	
	free(cipherbuf);
	exit(EXIT_SUCCESS);
}

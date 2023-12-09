#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LINESIZE 512

const char* fileExtension	= ".csv";
const char* file			= "Data/RKI_COVID19_2020-";
const char* file2			= "Data/RKI_COVID19_2021-";

const char* month[]={
	"05-", "06-", "07-", "08-", "09-", "10-", "11-",  "12-"
};

const int month_Lenght[]={
	31, 30, 31, 31, 30, 31, 30, 31
};

int main(int argc, char* argv[]){

	char* name = malloc(sizeof(char) * 64);

	FILE* pDatei;
	
	char* line = malloc(sizeof(char) * LINESIZE);
	
	char * dayPart = malloc(sizeof(char*) * 3);
	int i;
	int j;
	for(i = 0; i < 8; i++){ //Monat
		for(j = 1; j <= month_Lenght[i]; j++){ //Tag
			sprintf(dayPart, "%02d", j);
			
			if(j == 6 && i == 5) continue; // 06.10 gibts nicht :(
			//if(j == 4 && i == 0) continue; //anderes Format
			
			strcpy(name, file);
			name = strcat(name, month[i]);
			name = strcat(name, dayPart);
			name = strcat(name, fileExtension);
			FILE* pDatei = fopen(name, "r");
			
			if(pDatei == NULL){printf("Dateifehler"); exit(1);}
			
			fgets(line, LINESIZE, pDatei);
			int sum = 0;
			int sumTod = 0;
			int sumHealed = 0;
			while(fgets(line, LINESIZE, pDatei)){
				
				char* part = malloc(sizeof(char*) * 16);
				
				part = strtok(line, ","); part = strtok(NULL, ","); part = strtok(NULL, ",");
				part = strtok(NULL, ","); part = strtok(NULL, ","); part = strtok(NULL, ",");
				
				//AnzahlFall
				part = strtok(NULL, ","); int anzahlFall = atoi(part);

				//AnzahlTodesfall
				part = strtok(NULL, ","); int anzahlTodesfall = atoi(part);
				
				part = strtok(NULL, ","); 
				part = strtok(NULL, ","); 
				part = strtok(NULL, ",");
				
				//neuer Fall
				part = strtok(NULL, ","); if(strcmp(part,"1") == 0) sum += anzahlFall;
				
				//neuer Todesfall
				part = strtok(NULL, ","); if(strcmp(part,"1") == 0) sumTod += anzahlTodesfall;
				
				part = strtok(NULL, ",");
				
				//Genesungen
				part = strtok(NULL, ",");
				if(strcmp(part,"1") == 0){
					part = strtok(NULL, ",");
					sumHealed += atoi(part);
				} 
			}
			
			printf("%s%s: neue: %d, tot: %d, genesen: %d, delta I: %d\n", month[i], dayPart, sum, sumTod, sumHealed, sum-(sumTod + sumHealed));
	
			fclose(pDatei);
			
		}
	}
	
	
	//Januar 2021
	for(j=1; j<=25; j++){
		sprintf(dayPart, "%02d", j);
		
		strcpy(name, file2);
		name = strcat(name, "01-");
		name = strcat(name, dayPart);
		name = strcat(name, fileExtension);
				
		FILE* pDatei = fopen(name, "r");
			
		if(pDatei == NULL){printf("Dateifehler2"); exit(1);}
			
			fgets(line, LINESIZE, pDatei);
			int sum = 0;
			int sumTod = 0;
			int sumHealed = 0;
			while(fgets(line, LINESIZE, pDatei)){
				
				char* part = malloc(sizeof(char*) * 16);
				
				part = strtok(line, ","); part = strtok(NULL, ","); part = strtok(NULL, ",");
				part = strtok(NULL, ","); part = strtok(NULL, ","); part = strtok(NULL, ",");
				
				//AnzahlFall
				part = strtok(NULL, ","); int anzahlFall = atoi(part);

				//AnzahlTodesfall
				part = strtok(NULL, ","); int anzahlTodesfall = atoi(part);
				
				part = strtok(NULL, ","); 
				part = strtok(NULL, ","); 
				part = strtok(NULL, ",");
				
				//neuer Fall
				part = strtok(NULL, ","); if(strcmp(part,"1") == 0) sum += anzahlFall;
				
				//neuer Todesfall
				part = strtok(NULL, ","); if(strcmp(part,"1") == 0) sumTod += anzahlTodesfall;
				
				part = strtok(NULL, ",");
				
				//Genesungen
				part = strtok(NULL, ",");
				if(strcmp(part,"1") == 0){
					part = strtok(NULL, ",");
					sumHealed += atoi(part);
				} 
			}
			
			printf("%s%s: neue: %d, tot: %d, genesen: %d, delta I: %d\n", "01-", dayPart, sum, sumTod, sumHealed, sum-(sumTod + sumHealed));
	
			fclose(pDatei);
	}
	
}

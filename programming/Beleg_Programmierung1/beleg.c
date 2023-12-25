//Funktionssammlung zur Loesung der Belegaufgabe Programmierung I

#include "liste.h"
#include "telvz.h"

#define BUFFERSIZE 256
#define ARRAYSIZE  21

Knoten* Suche(Liste* PointerListe, char* Suche)
{
	Knoten* PointerKnoten;
			
	for(PointerKnoten=ErsterKnoten(PointerListe); PointerKnoten->PointerItem; PointerKnoten=NxtKnoten(PointerListe))
	{
		if(strcmp(PointerListe->PointerAkt->PointerItem->TlNr,Suche)==0)
		{
			return PointerListe->PointerAkt;
		}			
	}
    return NULL;
}

int NummerTest(char* Nummer, char* Name, char* VorName)
{
    int n = strlen(Nummer);
    int m = strlen(Name);
    int v = strlen(VorName);
    
    int j;
    
	for(j=0; j<n; j++)
	{
		if(iscntrl(Nummer[j]) || isalpha(Nummer[j]))
		{
            return -1;
		}
	}
    for(j=0; j<m; j++)
	{
		if(iscntrl(Name[j]) || isdigit(Name[j]))
		{
            return -2;
		}
	}
	for(j=0; j<v; j++)
	{
		if(iscntrl(VorName[j]) || isdigit(VorName[j]))
		{
            return -3;
		}
	} 
	
	return 0;
}

void neu(Knoten * PointerKnoten)
{
    char* NameNeu = (char*)fl_get_input(fd_Telvz->Name);
    char* VorNameNeu = (char*)fl_get_input(fd_Telvz->VorName);
    char* TelNeu = (char*)fl_get_input(fd_Telvz->Tel);
    if(*NameNeu != 0)
        strcpy(PointerKnoten->PointerItem->Name, NameNeu);
    if(*VorNameNeu != 0)
        strcpy(PointerKnoten->PointerItem->VorName, VorNameNeu);
    if(TelNeu != 0)
        strcpy(PointerKnoten->PointerItem->TlNr, TelNeu);      
}

Datensatz* DateiEinlesen(FILE* PointerDatei, Liste* PointerListe)
{
	char buf [BUFFERSIZE];
	char* Pointer;
	Datensatz* PDatensatz = NULL;
	char trenner[1] = "|";
	count = 0;

	
	if(fgets(buf,BUFFERSIZE,PointerDatei))
	{
		PDatensatz = malloc(sizeof(Datensatz));
		if(PDatensatz)
		{		
			Pointer = strtok(buf, "|");
			PDatensatz->Name = malloc(sizeof(char*));
			
			if(PDatensatz->Name)
			{
				strcpy(PDatensatz->Name,Pointer);
				count++;
			}

			PDatensatz->VorName = malloc(sizeof(char*));
			Pointer = strtok(NULL, "|");

			if(PDatensatz->VorName)
			{
				strcpy(PDatensatz->VorName,Pointer);
				count++;
			}

			PDatensatz->TlNr = malloc(sizeof(char*));
			Pointer = strtok(NULL, "|");
			Pointer[strlen(Pointer)-1] = 0;

			if(PDatensatz->TlNr)
			{
				strcpy(PDatensatz->TlNr,Pointer);
				count++;
			}	
		}
	}
	if(count == 3) 
    {
        return PDatensatz;
    }
	else return NULL;	
}

int DateiSchreiben(char* Dateiname, Liste* PointerListe)
{
	count=0;
	PointerDatei = fopen(Dateiname,"wt");
	Knoten* PointerKnoten;
	
	if(PointerDatei)
	{
		PointerKnoten=ErsterKnoten(PointerListe);
		while(PointerKnoten->PointerItem)			
		{ 
			fputs(PointerKnoten->PointerItem->Name, PointerDatei);
			fputs("|", PointerDatei);
			fputs(PointerKnoten->PointerItem->VorName, PointerDatei);
			fputs("|", PointerDatei);
			fputs(PointerKnoten->PointerItem->TlNr, PointerDatei);
			fputs("\n", PointerDatei);
			count++;
			PointerKnoten=NxtKnoten(PointerListe);
		}
	return count;
	}else 
	{
		return -1;
	}

	
}

//Zuletzt geaendert: 28.02.2019

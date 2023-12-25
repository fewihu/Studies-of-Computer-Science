//Headerfile zur Loesung der Belegaufgabe Programmierung I

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define BUFFERSIZE 256
#define ARRAYSIZE  21

//Typedef fuer einen Eintrag im Telefonbuch

typedef struct Datensatz
{
    char* Name;		//Nachname	als Pointer auf Character
    char* VorName;	//Vorname		-"-
    char* TlNr;		//Telfonnummer		-"-
}Datensatz;

//--------------------------------------------------------------------------------------------------------------------------

//Typedef fuer ein Element der doppelt verketteten Liste

typedef struct Knoten
{
    struct Knoten* PointerNxt;	//Pointer auf naechtes Listenelement
    struct Knoten* PointerPrv;	//Pointer auf vorheriges 
    Datensatz* PointerItem;	//Pointer auf Datensatz des Listenelements
}Knoten;

//-------------------------------------------------------------------------------------------------------------------------

//Typedef fuer den Kopf der doppelt verketteten Liste

typedef struct Liste
{
	Knoten Kopf;
	Knoten*PointerAkt;
}Liste;

//--------------------------------------------------------------------------------------------------------------------------

//Globale Variablen

Liste*		PointerListe;		//Pointer der Liste
Datensatz*	PointerDatensatz;	//Pointer des aktuellen Datensatzes (Eintrags)
Knoten*		PointerKnoten;		//Pointer des aktuellen Listenelements
FILE* PointerDatei;			//FILE-Pointervariable
char*Dateiname;				//mit argv[1] uebergebener Name einer Datei zum Speichern
int flag;				//Flag: nicht 0 = bereits Datei eingelesen bzw. in Datei gespeichert, 0 = noch keine Datei eingelesen

//--------------------------------------------------------------------------------------------------------------------------

//Funktionsprototypen

void neu(Knoten* PointerKnoten);
//Funktion zum aendern eines ausgewaehlten Datensatzes

Liste* ListeErzeugen();
//Erzeugt eine leere Liste

void* ErsterKnoten(Liste* PointerListe);
//setzt aktuelles Listenelement im Listenkopf auf das erste Listenelement

void* LetzterKnoten(Liste* PointerListe);
//setzt aktuelles Listenelement im Listenkopf auf das letzte Listenelement

void* NxtKnoten(Liste* PointerListe);
//setzt aktuelles Listenelemnet im Listenkopf auf das naechste Listenelement

int Einfugen(Knoten* KnotenDavor, Datensatz* PointerDatensatz);
//fuegt einen Datensatz(Eintrag) hinter ein Listenelement "KnotenDavor" ein, Bestandteil der Funktionen EinfugenVorn und EinfugenHinten

int xEinfugen(Knoten* KnotenDahinter, Datensatz* PointerDatensatz);
//fuegt einen Datensatz(Eintrag) vor ein Listenelement "KnotenDahinter" ein

int EinfugenVorn(Liste* PointerListe, Datensatz*PointerDatensatz);
//fuegt einen Datensatz(Eintrag) am Anfang der Liste ein

int EinfugenHinten(Liste* PointerListe, Datensatz*PointerDatensatz);
//fuegt einen Datnesatz(Eintrag) am Ende der Liste ein

void KnotenEntfernen(Liste* PointerListe, Knoten* Knoten);
//Entfernt ein Listenelement aus der Liste und gibt den allozierten Speicherplatz frei

Knoten* Suche(Liste* PointerListe, char* Suche);
//Suche in den Listenelementen nach einer als Character-Pointer uebergebenen Telefonnummer

int NummerTest(char* Nummer, char*Name, char*VorName);
//prueft ob uebergebener String eine Telefonnummer (bestehend aus Ziffern und Sonderzeichen ist)

Datensatz* DateiEinlesen(FILE* PointerDatei, Liste* PointerListe);
//Liest aus einer mit argv[1] uebergebenen Textdatei x Datensätze(Einträge) in Listenelemente der Liste ein

int DateiSchreiben(char* Dateiname, Liste* PointerListe);
//Speichert alle Datensaetze der Liste in einer mit argv[1] uebergebenen Textdatei

//Zuletzt geaendert: 28.02.2019

//Headerfile fuer die Form (von fdesign generiert) Bestandteil der Loesung der Belegaufgabe Programmierung I

#ifndef FD_Telvz_h_
#define FD_Telvz_h_

#include <forms.h>

//Funktionsprototypen fuer Callbackfunktionen

void VorName    (FL_OBJECT *,long);
void Name       (FL_OBJECT *,long);
void Tel        (FL_OBJECT *,long);
void dread      (FL_OBJECT *,long);
void insert     (FL_OBJECT *,long);
void search     (FL_OBJECT *,long);
void erase      (FL_OBJECT *,long);
void dwrite     (FL_OBJECT *,long);
void info       (FL_OBJECT *,long);
void output     (FL_OBJECT *,long);
void reset      (FL_OBJECT *,long);
void nderung    (FL_OBJECT *,long);
void ende       (FL_OBJECT *,long);
int  endclose   (FL_FORM *, void *);

//Globale Variablen

const char* returninfo;			//Globale Variablen fuer Zustandsinformation des Programms
char* SucheBuf;					//Globale Variable fuer zu suchende Telfonnummer
int totalcount;					//Zaehlvariable fuer die gesammte Anzahl Einträge
int count;						//Zaehlvariable fuer ausgegebene bzw gespeicherte Eintraege
int outputinfo;					//Zaehlvariable zum Zuruecksetzen des Ausgabebrowsers
int outputout;                  //Zaehlvariable zum Zuruecksetzen des Infobrowsers
	
//Typedef fuer die Form mit Objekten

typedef struct {
    FL_FORM   * Telvz;
    void      * vdata;
    char      * cdata;
    long        ldata;
    FL_OBJECT * telvz;
	FL_OBJECT * info;
    FL_OBJECT * out;
    FL_OBJECT * VorName;
    FL_OBJECT * Name;
    FL_OBJECT * Tel;
    FL_OBJECT * read;
    FL_OBJECT * insert;
    FL_OBJECT * search;
    FL_OBJECT * erase;
    FL_OBJECT * write;
	FL_OBJECT * output;
	FL_OBJECT * reset;
    FL_OBJECT * new;
    FL_OBJECT * ende;
}FD_Telvz;

//Funktionsprototyp zum Erzeugen der Form

extern FD_Telvz * create_form_Telvz( void );
extern FD_Telvz *fd_Telvz;

#endif /* FD_Telvz_h_ */

//Zuletzt geaendert: 28.02.2018

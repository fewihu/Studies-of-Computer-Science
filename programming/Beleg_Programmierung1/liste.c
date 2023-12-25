//Funktionssammlung zur Loesung der Belegaufgabe Programmierung I

#include "liste.h"
#include "telvz.h"

#define BUFFERSIZE 256
#define ARRAYSIZE  21

Liste* ListeErzeugen()
{
	Liste* PointerListe;
	PointerListe=malloc(sizeof(Liste));
	if(PointerListe)
	{
	PointerListe->PointerAkt=NULL;
	PointerListe->Kopf.PointerNxt=(Knoten*)PointerListe;
	PointerListe->Kopf.PointerPrv=(Knoten*)PointerListe;
	PointerListe->Kopf.PointerItem=NULL;
	}
}

void* LetzterKnoten(Liste* PointerListe)
{
	PointerListe->PointerAkt=PointerListe->Kopf.PointerPrv;
	return PointerListe->PointerAkt;
}

void* ErsterKnoten(Liste* PointerListe)
{
	PointerListe->PointerAkt=PointerListe->Kopf.PointerNxt;
	return PointerListe->PointerAkt;	
}

void* NxtKnoten(Liste* PointerListe)
{
	PointerListe->PointerAkt=PointerListe->PointerAkt->PointerNxt;
	return PointerListe->PointerAkt;
}

int Einfugen(Knoten* KnotenDavor, Datensatz* PointerDatensatz)
{
	Knoten* Speicher;
	Speicher=malloc(sizeof(Knoten));
	if(Speicher)
	{
		Speicher->PointerItem=PointerDatensatz;
		Speicher->PointerNxt=KnotenDavor->PointerNxt;
		Speicher->PointerPrv=KnotenDavor->PointerNxt->PointerPrv;
		Speicher->PointerNxt->PointerPrv=Speicher;
		KnotenDavor->PointerNxt=Speicher;	
	}
	return Speicher?1:0;
}

int xEinfugen(Knoten* KnotenDahinter, Datensatz* PointerDatensatz)
{
	Knoten* Speicher;
	Speicher=malloc(sizeof(Knoten));
	if(Speicher)
	{
		Speicher->PointerItem=PointerDatensatz;
		Speicher->PointerNxt=KnotenDahinter;
		Speicher->PointerPrv=KnotenDahinter->PointerPrv;
		KnotenDahinter->PointerPrv->PointerNxt=Speicher;
		KnotenDahinter->PointerPrv=Speicher;
	}
	return Speicher?1:0;	
}

int EinfugenVorn(Liste* PointerListe, Datensatz*PointerDatensatz)
{
	return Einfugen(&(PointerListe->Kopf),PointerDatensatz);
}

int EinfugenHinten(Liste* PointerListe, Datensatz*PointerDatensatz)
{
	return Einfugen((PointerListe->Kopf.PointerPrv),PointerDatensatz);
}

void KnotenEntfernen(Liste* PointerListe, Knoten* Knoten)
{
	PointerListe->PointerAkt = Knoten;
	PointerListe->PointerAkt->PointerNxt->PointerPrv = PointerListe->PointerAkt->PointerPrv;
	PointerListe->PointerAkt->PointerPrv->PointerNxt = PointerListe->PointerAkt->PointerNxt;
	PointerListe->PointerAkt = Knoten->PointerPrv;
	free(Knoten->PointerItem);
	free(Knoten);
}

//Zuletzt geaendert: 28.02.2019

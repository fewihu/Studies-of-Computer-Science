//C-File fuer die Callbackfunktionen zur Loesung der Belegaufgabe Programmierung I

#include "liste.h"
#include "telvz.h"

void VorName(FL_OBJECT * ob, long data)
{
	returninfo = fl_get_input(ob);
    fl_add_browser_line(fd_Telvz->info,returninfo);
    outputinfo++;
}

void Name(FL_OBJECT * ob, long data)
{
	returninfo = fl_get_input(ob);
    fl_add_browser_line(fd_Telvz->info,returninfo);
    outputinfo++;
}

void Tel(FL_OBJECT * ob, long data)
{
	returninfo = fl_get_input(ob);
    fl_add_browser_line(fd_Telvz->info,returninfo);
    outputinfo++;
}

void ende(FL_OBJECT * ob, long data)
{
    if(totalcount > 0 && Dateiname) DateiSchreiben(Dateiname,PointerListe);
    exit(3);
}

int endclose(FL_FORM * f, void *d)
{
    if(totalcount > 0)
	{
		DateiSchreiben(Dateiname,PointerListe);
    }
    return FL_OK;
}

void dread( FL_OBJECT * ob, long data )
{
	Datensatz* PDatensatz;
	char c[12];
    char* meldung = " Datensaetze eingelesen";
    char* kompmeldung;
	PointerDatei = fopen(Dateiname,"rt");
	if(flag < 1)
	{
		if(PointerDatei)
		{
			while(1)
			{
				PDatensatz = DateiEinlesen(PointerDatei,PointerListe);

				if(PDatensatz)
				{
					EinfugenHinten(PointerListe, PDatensatz);
					totalcount++;
                    flag++;
				}else break;
			}
            snprintf(c, 12, "%d", totalcount);
            strcat(c, meldung);
            fl_add_browser_line(fd_Telvz->info,(const char*)c);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
		}else
		{
            
			returninfo = "Einlesen fehlgeschlagen";
            fl_add_browser_line(fd_Telvz->info,returninfo);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
		}
	}else
	{
		returninfo = "Bereits aus Datei eingelesen";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
	}
	//fclose(PointerDatei);
}

void insert(FL_OBJECT * ob, long data)
{
	char* bufVorName=(char*)fl_get_input(fd_Telvz->VorName);
	char* bufName	=(char*)fl_get_input(fd_Telvz->Name);		 
	char* bufTel	=(char*)fl_get_input(fd_Telvz->Tel);
	int x = 0;

	PointerDatensatz=malloc(sizeof(Datensatz));
	if(PointerDatensatz)
	{
        flag++;	
        if(totalcount == 0)
		{
			PointerDatensatz->VorName	= malloc(sizeof(char*));
			if(PointerDatensatz->VorName)	
				strcpy(PointerDatensatz->VorName, bufVorName);

			PointerDatensatz->Name		= malloc(sizeof(char*));
			if(PointerDatensatz->Name)
				strcpy(PointerDatensatz->Name, bufName);
	
			PointerDatensatz->TlNr		= malloc(sizeof(char*));
			if(PointerDatensatz->TlNr)
                strcpy(PointerDatensatz->TlNr, bufTel);
			{
				if(NummerTest(bufTel, bufName, bufVorName)==0)
				{
					EinfugenVorn(PointerListe, PointerDatensatz);
					totalcount++;
                    returninfo="eingefuegt";
                    fl_add_browser_line(fd_Telvz->info,returninfo);
                    fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                    outputinfo += 2;
				}else
				{
					returninfo = "Telefonnummern koennen keine Buchstaben und Namen keine Zahlen enthalten";
                    fl_add_browser_line(fd_Telvz->info,returninfo);
                    fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                    outputinfo += 2;
					free(PointerDatensatz->VorName);
					free(PointerDatensatz->Name);
					free(PointerDatensatz);
				}
			}
		}else
		{
			for(PointerKnoten=ErsterKnoten(PointerListe);PointerKnoten->PointerItem;PointerKnoten=NxtKnoten(PointerListe))
			{
				if(strcmp(PointerKnoten->PointerItem->Name,bufName)>0)
				{
					PointerDatensatz->VorName	= malloc(sizeof(char*));
					if(PointerDatensatz->VorName)	
						strcpy(PointerDatensatz->VorName, bufVorName);
	
					PointerDatensatz->Name		= malloc(sizeof(char*));
					if(PointerDatensatz->Name)
						strcpy(PointerDatensatz->Name, bufName);
		
					PointerDatensatz->TlNr		= malloc(sizeof(char*));
					if(PointerDatensatz->TlNr)
                        strcpy(PointerDatensatz->TlNr, bufTel);
					{
						if(NummerTest(bufTel, bufName, bufVorName)==0)
						{
							xEinfugen(PointerKnoten, PointerDatensatz);
							totalcount++;
							x++;
                            returninfo = "eingefuegt";
                            fl_add_browser_line(fd_Telvz->info,returninfo);
                            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                            outputinfo += 2;
						}else
						{
							returninfo = "Telefonnummern koennen keine Buchstaben und Namen keine Zahlen enthalten";
                            fl_add_browser_line(fd_Telvz->info,returninfo);
                            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                            outputinfo += 2;
							free(PointerDatensatz->VorName);
							free(PointerDatensatz->Name);
							free(PointerDatensatz);
						}
					}
				}
			}
			if(x==0)
			{
				PointerDatensatz->VorName	= malloc(sizeof(char*));
				if(PointerDatensatz->VorName)	
					strcpy(PointerDatensatz->VorName, bufVorName);
	
				PointerDatensatz->Name		= malloc(sizeof(char*));
				if(PointerDatensatz->Name)
					strcpy(PointerDatensatz->Name, bufName);
	
				PointerDatensatz->TlNr		= malloc(sizeof(char*));
				if(PointerDatensatz->TlNr)
                    strcpy(PointerDatensatz->TlNr, bufTel);
				{
					if(NummerTest(bufTel, bufName, bufVorName)==0)
					{
						EinfugenHinten(PointerListe, PointerDatensatz);
						totalcount++;
                        returninfo = "eingefuegt";
                        fl_add_browser_line(fd_Telvz->info,returninfo);
                        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                        outputinfo += 2;
					}else
					{
						returninfo = "Telefonnummern koennen keine Buchstaben und Name keine Zahlen enthalten";
                        fl_add_browser_line(fd_Telvz->info,returninfo);
                        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
                        outputinfo += 2;
						free(PointerDatensatz->VorName);
						free(PointerDatensatz->Name);
						free(PointerDatensatz);
					}
				}
				
					
			}	
		}
	}
	
}

void reset(FL_OBJECT* ob, long data)
{
	fl_set_input(fd_Telvz->Name, "");
	fl_set_input(fd_Telvz->VorName, "");
	fl_set_input(fd_Telvz->Tel, "");

	while(outputinfo > 0)      //Entfernt Zeilen aus dem Info-Browser
	{	
		fl_delete_browser_line(fd_Telvz->info,1);
		outputinfo--;
	}
    while(outputout > 0)        //Entfernt Zeilen aus dem Ausgabe-Brower
    {
        fl_delete_browser_line(fd_Telvz->out,1);
        outputout--;
    }
	if(outputinfo == 0)
	{
		returninfo = "Ein- und Ausgabe gelehrt";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
	}
}

void search(FL_OBJECT * ob, long data)
{
	SucheBuf = (char*)fl_get_input(fd_Telvz->Tel);
	Knoten* PointerErgebnis = Suche(PointerListe,SucheBuf);
    if(PointerErgebnis !=NULL)
	{
        returninfo = "Passenden Eintrag ausgegeben";
        fl_add_browser_line(fd_Telvz->info,returninfo);
		fl_add_browser_line(fd_Telvz->info,PointerErgebnis->PointerItem->VorName);
		fl_add_browser_line(fd_Telvz->info,PointerErgebnis->PointerItem->Name);
		fl_add_browser_line(fd_Telvz->info,PointerErgebnis->PointerItem->TlNr);
		fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
		outputinfo += 5;
        PointerKnoten = PointerErgebnis;
	}else
	{
		returninfo = "Keinen passenden Eintrag gefunden";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
	}
	SucheBuf = NULL;
}

void erase(FL_OBJECT * ob, long data)
{
	if(totalcount>0)
	{
		if(PointerListe->PointerAkt)
		{
			KnotenEntfernen(PointerListe,PointerListe->PointerAkt);
			returninfo = "Eintrag geloescht";
            fl_add_browser_line(fd_Telvz->info,returninfo);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
			totalcount--;
		}else
		{
			returninfo = "Keinen Eintrag zum loeschen ausgewaehlt, Suche benutzen";
            fl_add_browser_line(fd_Telvz->info,returninfo);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
		}	
	}else
	{
		returninfo = "Liste enthaelt keine Eintraege";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
	}
}

void dwrite(FL_OBJECT * ob, long data)
{
	char c[12];
	int x;
	if(totalcount > 0)
	{
		x = DateiSchreiben(Dateiname,PointerListe);
		if(x < 0)
		{
			returninfo = "Speichern fehlgeschlagen";
            fl_add_browser_line(fd_Telvz->info,returninfo);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
		}else
		{
			//snprintf(c, 3, "%d", x);
            snprintf(c, 12, "%d", x);
			returninfo = (const char*) c;
            fl_add_browser_line(fd_Telvz->info,returninfo);
            fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
            outputinfo += 2;
		}
	}else
	{
		returninfo = "Liste enthaelt keine Eintraege";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
	}

}

void output(FL_OBJECT * ob, long data)
{
    int count=0;
    char c[12];
	if(totalcount>0)
	{

		for(PointerKnoten=ErsterKnoten(PointerListe); PointerKnoten->PointerItem; PointerKnoten=NxtKnoten(PointerListe))
		{
			fl_add_browser_line(fd_Telvz->out,PointerKnoten->PointerItem->VorName);
			fl_add_browser_line(fd_Telvz->out,PointerKnoten->PointerItem->Name);
			fl_add_browser_line(fd_Telvz->out,PointerKnoten->PointerItem->TlNr);
			fl_add_browser_line(fd_Telvz->out,"----------------------------------------------------------");
			count++;
			outputout = 4*count;
		}	       
	}
    snprintf(c, 12, "%d", count);
    returninfo = (const char*)c;
    fl_add_browser_line(fd_Telvz->info,returninfo);
    fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
    outputinfo += 2;  
}

void nderung(FL_OBJECT * ob, long data)
{
    if(PointerKnoten->PointerItem)
    {
        neu(PointerKnoten);
        returninfo = "Erfolgeich geaendert";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
    }else
    {
        returninfo = "Kein Eintrag ausgewaehlt, Suche benutzen!";
        fl_add_browser_line(fd_Telvz->info,returninfo);
        fl_add_browser_line(fd_Telvz->info,"----------------------------------------------------------");
        outputinfo += 2;
    }
}

//Zuletzt geaendert: 28.02.2019

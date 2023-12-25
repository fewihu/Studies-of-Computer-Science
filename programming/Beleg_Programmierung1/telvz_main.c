//Hauptprogramm zum Anzeigen der Form (generiert von fdesign) Bestandteil der Leosung der Belegaufgabe Programmierung I

#include "liste.h"
#include "telvz.h"

FD_Telvz *fd_Telvz;

int main(int argc, char * argv[])
{
	Dateiname=argv[1];				//Uebergabe des mit argv[1] angegebenen Dateinames zum Speichern
	PointerListe=ListeErzeugen();	//Erzeugt leere Liste
	if(PointerListe == NULL)
    {
        printf("Liste konnte nicht erzeugt werden\n");
        exit;
    }
    flag = 0;


    fl_initialize( &argc, argv, 0, 0, 0 );
    fd_Telvz = create_form_Telvz( );

    fl_show_form( fd_Telvz->Telvz, FL_PLACE_CENTERFREE, FL_FULLBORDER, "Telvz" );

    fl_do_forms( );

    if ( fl_form_is_visible( fd_Telvz->Telvz ) )
        fl_hide_form( fd_Telvz->Telvz );
    fl_free( fd_Telvz );
    fl_finish( );

    return 0;
}

//Zuletzt geaendert: 28.02.2018

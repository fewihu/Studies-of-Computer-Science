//File (von fdesign generiert) Bestandteil der Loesung der Belegaufgabe Programmierung I

#include "telvz.h"

FD_Telvz *
create_form_Telvz( void )
{
    FL_OBJECT *obj;
    FD_Telvz *fdui = ( FD_Telvz * ) fl_malloc( sizeof *fdui );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->Telvz = fl_bgn_form( FL_NO_BOX, 1400, 800 );

    fdui->telvz = obj = fl_add_box( FL_FLAT_BOX, 0, 0, 1400, 800, "" );
    fl_set_object_color( obj, FL_LEFT_BCOL, FL_COL1 );
    fl_set_object_lsize( obj, FL_LARGE_SIZE );

    fdui->out = obj = fl_add_browser( FL_NORMAL_BROWSER, 960, 20, 350, 740, "Ausgaben" );
    fl_set_object_lsize( obj, FL_LARGE_SIZE );
    
    fdui->info = obj = fl_add_browser( FL_NORMAL_BROWSER, 590, 20, 350, 740, "Zustandsinfo");
    fl_set_object_lsize( obj, FL_LARGE_SIZE );
    
    fdui->VorName = obj = fl_add_input( FL_NORMAL_INPUT, 160, 40, 230, 40, "Vorname" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, VorName, 0 );

    fdui->Name = obj = fl_add_input( FL_NORMAL_INPUT, 160, 90, 230, 40, "Name" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, Name, 0 );

    fdui->Tel = obj = fl_add_input( FL_NORMAL_INPUT, 160, 140, 230, 40, "Telefonnumer" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, Tel, 0 );

    fdui->read = obj = fl_add_button( FL_NORMAL_BUTTON, 420, 40, 150, 50, "Datei Einlesen" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, dread, 0 );

    fdui->insert = obj = fl_add_button( FL_NORMAL_BUTTON, 420, 100, 150, 50, "Einfuegen" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, insert, 0 );

    fdui->search = obj = fl_add_button( FL_NORMAL_BUTTON, 420, 160, 150, 50, "Suchen" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, search, 0 );

    fdui->erase = obj = fl_add_button( FL_NORMAL_BUTTON, 420, 220, 150, 50, "Loeschen" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, erase, 0 );

    fdui->write = obj = fl_add_button( FL_NORMAL_BUTTON, 420, 280, 150, 50, "Datei Speichern" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_callback( obj, dwrite, 0 );

	fdui->output = obj = fl_add_button(FL_NORMAL_BUTTON, 420, 340, 150, 50, "Ausgabe");
	fl_set_object_lsize(obj, FL_MEDIUM_SIZE);
	fl_set_object_callback(obj, output, 0);

	fdui->reset = obj = fl_add_button(FL_NORMAL_BUTTON, 420, 400, 150, 50, "Reset");
	fl_set_object_lsize(obj, FL_MEDIUM_SIZE);
	fl_set_object_callback(obj, reset, 0);
    
    fdui->new = obj = fl_add_button(FL_NORMAL_BUTTON, 420, 460, 150, 50, "Aenderung");
    fl_set_object_lsize(obj, FL_MEDIUM_SIZE);
	fl_set_object_callback(obj, nderung, 0);

    fdui->ende = obj = fl_add_button(FL_NORMAL_BUTTON, 420, 520, 150, 50, "Beenden");
    fl_set_object_lsize(obj, FL_MEDIUM_SIZE);
    fl_set_object_callback(obj, ende, 0);
    
    
    void* fmclose = &endclose;
    fl_set_atclose(fmclose, NULL);
    
        
    fl_end_form( );

    fdui->Telvz->fdui = fdui;

    return fdui;
}

//Zuletzt geaendert: 28.02.2019

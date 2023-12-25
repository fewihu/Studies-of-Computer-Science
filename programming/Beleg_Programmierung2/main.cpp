//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61

#include "bibo.h"
#include <QApplication>

int main( int argc, char* argv[]){
	QApplication a(argc, argv);
	bibo b;
	b.show();
	return a.exec();
}

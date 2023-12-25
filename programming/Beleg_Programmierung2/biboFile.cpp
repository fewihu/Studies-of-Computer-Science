//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61
//Implementation des Ladens und Speicherns

#include "bibo.h"
#include "user.h"
#include "medium.h"
#include "lends.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <QFileDialog>

using namespace std;

//fct to get int from char
int cint(char* c){
	int res = 0;
	unsigned int i = 0;
	int pot = 0;
	int val = 0;
	for(i = 0; i < strlen(c); i++){
		pot = pow(10,i);
		val = (* (c + strlen(c) - (i + 1) ) ) - 48;
		res += pot*val;
	}
	return res;
}

void bibo :: Load(){
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Datei öffnen"),  "/home/felix/bibo", tr("Bibodoc (*.bib)"));

	bool us = false;
	bool med = false;

	ifstream ifs(fileName.toStdString());
	char in[256];
	ifs.getline(in, 256);
	//there are users to read in
	while(strcmp(in, "-") != 0){
		us = true;
		user u;
		//vname
		QString vn(in);
		u.SetVName(vn);

		//name
		ifs.getline(in, 256);
		QString nn(in);
		u.SetName(nn);

		//id
		ifs.getline(in, 256);
		QString id(in);
		u.SetID(id);

		ListUser.append(u);
		ifs.getline(in, 256);
	}

	ifs.getline(in, 256);
	//there are books to read in
	while(strcmp(in, "-") != 0){
		med = true;
		book b;

		//title
		QString t(in);	
		b.SetTitle(t);

		//autor
		ifs.getline(in, 256);
		QString a(in);
		b.SetAutor(a);

		//verlag
		ifs.getline(in, 256);
		QString v(in);
		b.SetVerlag(v);

		//id
		ifs.getline(in, 256);
		QString id(in);
		b.SetID(id);

		//pages
		ifs.getline(in, 256);
		int pages = cint(in);
		b.SetPages(pages);	

		//total amount
		ifs.getline(in, 256);
		int TAmount = cint(in);
		b.setTotal(TAmount);

		//free amount
		ifs.getline(in, 256);
		int FAmount = cint(in);	
		b.setFree(FAmount);
		
		ListBook.append(b);
		ifs.getline(in, 256);
	}

	ifs.getline(in, 256);
	//there are journals to read in
	while(strcmp(in, "-") != 0){
		med = true;
		journal j;

		//title
		QString t(in);	
		j.SetTitle(t);

		//autor
		ifs.getline(in, 256);
		QString a(in);
		j.SetAutor(a);

		//verlag
		ifs.getline(in, 256);
		QString v(in);
		j.SetVerlag(v);

		//id
		ifs.getline(in, 256);
		QString id(in);
		j.SetID(id);

		//pages
		ifs.getline(in, 256);
		int pages = cint(in);	
		j.SetPages(pages);	

		//total amount
		ifs.getline(in, 256);
		int TAmount = cint(in);	
		j.setTotal(TAmount);

		//free amount
		ifs.getline(in, 256);
		int FAmount = cint(in);	
		j.setFree(FAmount);
		
		ListJournal.append(j);
		ifs.getline(in, 256);
	}

	ifs.getline(in, 256);
	//there are discs to read in
	while(strcmp(in, "-") != 0){
		med = true;
		disc d;

		//title
		QString t(in);	
		d.SetTitle(t);

		//autor
		ifs.getline(in, 256);
		QString a(in);
		d.SetAutor(a);

		//verlag
		ifs.getline(in, 256);
		QString v(in);
		d.SetVerlag(v);

		//id
		ifs.getline(in, 256);
		QString id(in);
		d.SetID(id);

		//pages
		ifs.getline(in, 256);
		int dur = cint(in);
		d.SetDur(dur);	

		//total amount
		ifs.getline(in, 256);
		int TAmount = cint(in);	
		d.setTotal(TAmount);

		//free amount
		ifs.getline(in, 256);
		int FAmount = cint(in);
		d.setFree(FAmount);
		
		ListDisc.append(d);
		ifs.getline(in, 256);
	}

	ifs.getline(in, 256);
	//there are lends to read in and user/mediums to build lends
	while(strcmp(in, "-") != 0 && us == true && med == true){

		lends l;

		//MID
		QString m(in);
		l.setMid(m);

		//UID
		ifs.getline(in, 256);
		QString u(in);
		l.setUid(u);
	
		//day
		ifs.getline(in, 256);
		int day = cint(in);

		//month
		ifs.getline(in, 256);
		int mon = cint(in);

		//year
		ifs.getline(in, 256);
		int y = cint(in);

		l.createDate(day, mon, y);

		ListLend.append(l);
		ifs.getline(in, 256);
	}

	

}

void bibo :: Safe(){
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Datei auswählen"),  "/home/felix/bibo", tr("Bibodoc (*.bib)"));

	ofstream of(fileName.toStdString());
	int i = ListUser.size() -1;
	for(; i >= 0; i--){
		of << ListUser.at(i);
	}
	of << "-" << endl;
	i = ListBook.size() -1;
	for(; i >= 0; i--){
		of << ListBook.at(i);
	}	
	of << "-" << endl;
	i = ListJournal.size() -1;
	for(; i >= 0; i--){
		of << ListJournal.at(i);
	}
	of << "-" << endl;
	i = ListDisc.size() -1;
	for(; i >= 0; i--){
		of << ListDisc.at(i);
	}
	of << "-" << endl;
	i = ListLend.size() -1;
	for(; i >= 0; i--){
		of << ListLend.at(i);
	}
	of << "-"; 
	of.close();
}

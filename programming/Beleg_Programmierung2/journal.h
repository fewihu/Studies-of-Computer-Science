//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef JOURNAL_H
#define JOURNAL_H

#include "medium.h"

class journal : public medium{
private:
	int pages;
public:

	//fill ostream with title, autor, verlag, id , pages and amount for output
	inline friend ostream & operator << (ostream &os, const journal &other){
		os << other.GetTitle().toStdString() << endl << other.GetAutor().toStdString() << endl << other.GetVerlag().toStdString() << endl << other.GetID().toStdString() << endl <<  other.pages << endl << other.getTAmount() << endl <<  other.getFAmount() << endl;
		return os;
	}

	inline void SetPages(int x){
		pages = x;
	}

	inline int GetPages() const{
		return pages;
	}

	inline QString display() const {
		QString out = "Titel: " + this->GetTitle() + "\n" + "Autor: " + this->GetAutor() + "\n" + "Verlag: " + this->GetVerlag() + "\n" + "ID: " + this->GetID() + "\n" + "Seitenzahl: " + QString :: number(this->pages) + "\n" + "Exemplare (gesammt / frei): " + QString :: number(this->getTAmount()) + " / " + QString :: number(this->getFAmount()) + "\n";
		return out;
	}
};

#endif

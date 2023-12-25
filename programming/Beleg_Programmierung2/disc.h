//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef DISC_H
#define DISC_H

#include "medium.h"

class disc : public medium{
private:
	int duration;
public:

	//fill ostream with title, autor, verlag, id , pages and amount for output
	inline friend ostream & operator << (ostream &os, const disc &other){
		os << other.GetTitle().toStdString() << endl << other.GetAutor().toStdString() << endl << other.GetVerlag().toStdString() << endl << other.GetID().toStdString() << endl <<  other.duration << endl << other.getTAmount() << endl <<  other.getFAmount() << endl;
		return os;
	}

	inline void SetDur(int x){
		duration = x;
	}

	inline int GetDur() const{
		return duration;
	}

	inline QString display() const {
		QString out = "Titel: " + this->GetTitle() + "\n" + "Autor: " + this->GetAutor() + "\n" + "Verlag: " + this->GetVerlag() + "\n" + "ID: " + this->GetID() + "\n" + "Spieldauer: " + QString :: number(this->duration) + "\n" + "Exemplare (gesammt / frei): " + QString :: number(this->getTAmount()) + " / " + QString :: number(this->getFAmount()) + "\n";
		return out;
	}
};

#endif

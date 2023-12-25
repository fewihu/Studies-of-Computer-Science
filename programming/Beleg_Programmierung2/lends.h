//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef LENDS_H
#define LENDS_H

#include "medium.h"
#include "user.h"
#include <iostream>
#include <iomanip>
#include <QString>
#include <QDate>

using namespace std;

class lends{

private:
	QString mid;
	QString uid;
	QDate date;
public:

	lends(){
	
	}

	~lends(){

	}

	void setDate(int dur){
		this->date = QDate::currentDate().addDays(dur);
	}

	void createDate(int d, int m, int y){
		this->date.setDate(y, m, d);
	}

	void setUid(const QString& uid){
		this->uid = uid;
	}

	void setMid(const QString& mid){
		this->mid = mid;
	}

	int daysLeft() const {
		QDate act = QDate::currentDate();
		return act.daysTo(this->date);
	}	

	bool due() const{
		QDate act = QDate::currentDate();
		if(act.daysTo(this->date) < 1)	
			return true;
		else 
			return false;
	}

	inline friend ostream & operator << (ostream &os, const lends &other){
		os << other.mid.toStdString() << endl << other.uid.toStdString() << endl << other.date.day() << endl << other.date.month() << endl << other.date.year() << endl;
		return os;
	}

	bool cmpUID(const user& other) const {
		if(this->uid.compare(other.GetID()) == 0)
			return true;
		else
			return false;
	}

	QString getMID() const{
		return this->mid;
	}

	QString getUID() const{
		return this->uid;
	}

};

#endif

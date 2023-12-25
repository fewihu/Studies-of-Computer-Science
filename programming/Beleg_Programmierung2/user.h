//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <iostream>
#include <iomanip>

using namespace std;

class user{
	private:
		QString	Name;
		QString	VName;
		QString	ID;

	public:

		//default-constructor
		user(){

		}

		//constructor QString parameters
		user(QString VName, QString Name, QString ID){
			this->VName	= VName;
			this->Name	= Name;
			this->ID		= ID;
		}

		//destructor delete
		~user(){

		}	

		inline void SetName(QString const &other){
			Name = other;
		}
		
		inline void SetVName(QString const &other){
			VName = other;
		}

		inline void SetID(QString const &other){
			ID = other;
		}
		
		inline const QString& GetName() const{
			return Name;
		}

		inline const QString& GetVName() const{
			return VName;
		}

		inline const QString& GetID() const{
			return ID;
		}

		inline friend ostream & operator << (ostream &os, const user &other){
   			os << other.VName.toStdString() << endl << other.Name.toStdString() << endl << other.ID.toStdString() << endl;
			return os;
		}

		inline bool cmpID(QString BufID) const{
			if(this->ID.compare(BufID) == 0){
				return true;
			}else{
				return false;
			}
		}

		inline int cmpName(QString BufName) const{
			return this->Name.compare(BufName);
		}

		inline int cmpVName(QString BufVName) const{
			return this->VName.compare(BufVName);
		}

		inline bool cmpID(const user& other) const{
			if(this->ID.compare(other.ID) == 0){
				return true;
			}else{
				return false;
			}
		}

		QString display() const {
			QString out = "Name: " + this->Name + "\n" + "Vorname: " + this->VName + "\n" + "ID: " + this->ID;
			return out;
		}
};

#endif

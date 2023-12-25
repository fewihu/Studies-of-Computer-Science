//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef MEDIUM_H
#define MEDIUM_H

#include <QString>
#include <QList>
#include <iostream>
#include <iomanip>

class medium{

private:
	
	QString Title;
	QString Autor;
	QString Verlag;
	QString ID;

	int totalAmount;
	int freeAmount;

public:

	medium(){
		
	}

	~medium(){

	}

	inline void SetTitle(const QString& other){
		Title = other;
	}

	inline void SetAutor(const QString& other){
		Autor = other;
	}

	inline void SetVerlag(const QString& other){
		Verlag = other;
	}

	inline void SetID(const QString& other){
		ID = other;
	}

	inline const QString& GetTitle() const{
		return Title;
	}

	inline const QString& GetAutor() const{
		return Autor;
	}

	inline const QString& GetVerlag() const{
		return Verlag;
	}

	inline const QString& GetID() const{
		return ID;
	}

	inline bool cmpID(const QString& ID) const{
		if(this->ID.compare(ID) == 0){
			return true;
		}else{
			return false;
		}
	}

	inline bool cmpID(const medium& other) const{
		if(this->ID.compare(other.ID) == 0){
			return true;
		}else{
			return false;
		}
	}

	inline bool cmpTitle(const QString& other) const{
		if(this->Title.compare(other) > 0){
			return true;
		}else{
			return false;
		}
	}

	inline bool cmpOther(const QString& tit, const QString& aut, const QString& ver) const{
		if(this->Title.compare(tit)		== 0||
		   this->Autor.compare(aut)	== 0||
		   this->Verlag.compare(ver)	== 0){
			return true;
		}else{
			return false;
		}
	}

	void setTotal(int n){
		totalAmount = n;
		freeAmount = n;
	}

	void setFree(int n){
		freeAmount = n;		
	}
	
	bool decAmount(){
		if(freeAmount > 0){
			freeAmount --;
			return true;
		}else{
			return false;
		}
	}
	
	bool incAmount(){
		if(freeAmount < totalAmount){
			freeAmount++;
			return true;
		}else{
			return false;
		}
	}

	int getFAmount() const {
		return freeAmount;
	}
	
	int getTAmount() const{
		return totalAmount;
	}
};

#endif

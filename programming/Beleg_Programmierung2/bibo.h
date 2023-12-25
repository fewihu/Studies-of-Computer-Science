//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61


#ifndef BIBO_H
#define BIBO_H
#include "ui_bibo.h"
#include "user.h" 
#include "medium.h"
#include "lends.h"
#include "book.h"
#include "disc.h"
#include "journal.h"

class bibo : public QMainWindow, public Ui::MainWindow{
	
	private:
		//create user list	
		QList<user> ListUser;

		//create user search hit list
		QList<user> ListUserHit; 

		//create book list
		QList<book> ListBook;

		//create book search hit list
		QList<book> ListBookHit;

		//create disc list
		QList<disc> ListDisc;

		//create disc search hit list
		QList<disc> ListDiscHit;

		//create journal list
		QList<journal> ListJournal;

		//create journal search hit list
		QList<journal> ListJournalHit;

		//create lend list
		QList<lends> ListLend; 	
	
		Q_OBJECT
	public:
		bibo (QMainWindow *parent = 0);
	private slots:
	
		//connected functions
		void CreateUser();
		void SearchUser();
		void EraseUser();
		void ChangeUser();

		void CreateMedium();
		void SearchMedium();
		void EraseMedium();
		void ChangeMedium();

		void Lend();
		void Restore();
		void DisplayLends();
		void DueLends();

		void Load();
		void Safe();

		//functions to set lineEdits to red
		void RedName();
		void RedVName();
		void RedUID();
		void RedTitle();
		void RedAutor();
		void RedVerlag();
		void RedMID();

		//function to resett all lineEdits to white
		void WhiteUM();
		
};

#endif

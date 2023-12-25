//Autor: Felix Mueller, Matrikelnummer: 46132, Studiengruppe: 18/041/61

#include "bibo.h"
#include <iostream>
#include <iomanip>
#include <QInputDialog>
#include <QMessageBox>

//TODO


//Medium löschen löscht alle Ausleihen



using namespace std;

//constructor for window bibo
bibo:: bibo(QMainWindow *parent) : QMainWindow(parent){
	
	//setup interface	
	setupUi(this);

	//connect QObjects to slots (user)
	connect(Bt_CreateUser, SIGNAL(clicked()), this, SLOT(CreateUser()));
	connect(Bt_SearchUser, SIGNAL(clicked()), this, SLOT(SearchUser()));
	connect(Bt_EraseUser, SIGNAL(clicked()), this, SLOT(EraseUser()));
	connect(Bt_ChangeUser, SIGNAL(clicked()), this, SLOT(ChangeUser()));

	//connect QObject to slots (medium)
	connect(Bt_CreateM, SIGNAL(clicked()), this, SLOT(CreateMedium()));
	connect(Bt_SearchM, SIGNAL(clicked()), this, SLOT(SearchMedium()));
	connect(Bt_EraseM, SIGNAL(clicked()), this, SLOT(EraseMedium()));
	connect(Bt_ChangeM, SIGNAL(clicked()), this, SLOT(ChangeMedium()));

	//connect QObject to slots (lends)
	connect(Bt_Lend, SIGNAL(clicked()), this, SLOT(Lend()));
	connect(Bt_Restore, SIGNAL(clicked()), this, SLOT(Restore()));
	connect(Bt_ListLends, SIGNAL(clicked()), this, SLOT(DisplayLends()));	
	connect(Bt_DueLends, SIGNAL(clicked()), this, SLOT(DueLends()));

	//connect actions to slots (load/store)
	connect(actionLaden, SIGNAL(triggered()), this, SLOT(Load()));
	connect(actionSpeichern, SIGNAL(triggered()), this, SLOT(Safe()));
}

//functions to set lineEdits to red
void bibo :: RedName(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_Name->setPalette(p);
}

void bibo :: RedVName(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_VName->setPalette(p);
}

void bibo :: RedUID(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_Uid->setPalette(p);
}

void bibo :: RedTitle(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_Titel->setPalette(p);
}

void bibo :: RedAutor(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_Autor->setPalette(p);
}

void bibo :: RedVerlag(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_Verlag->setPalette(p);
}

void bibo :: RedMID(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: red);
	this->In_MID->setPalette(p);
}

//fct to reset lineEdit color
void bibo :: WhiteUM(){
	QPalette p;
	p.setColor(QPalette :: Base, Qt :: white);
	this->In_Name->setPalette(p);
	this->In_VName->setPalette(p);
	this->In_Uid->setPalette(p);
	this->In_Titel->setPalette(p);
	this->In_Autor->setPalette(p);	
	this->In_Verlag->setPalette(p);
	this->In_MID->setPalette(p);
}

//user
//create user by inputs in lineEdits
void bibo :: CreateUser(){
	
	//set lineEdits to white
	WhiteUM();

	//clear user search hit list
	ListUserHit.clear();

	//clear display and Mid lineEdit
 	Display_User->clear();
	Lends_U->clear();
	this->In_MID->clear();

	//buffer QStrings
	QString BufName 	= this->In_Name->text();
	QString BufVName	= this->In_VName->text();
	QString BufID		= this->In_Uid->text();

	//QString user inserted message
	QString  UserInserted(QString::fromUtf8("Benutzer hinzugefügt!"));
		
	//buffer user
	user u;	

	//bool new ID already given and QString error message check if new ID is already given --> set bool given
	QString ErrGiven = "Diese ID ist bereits vergeben";
	bool given = false;
	if(!ListUser.isEmpty()){
		int l = ListUser.size();
		for(int j = 0; j < l; j++){

			//hit + mark ID lineEdit, set given and display error message
			if(ListUser.at(j).cmpID(BufID)){
				given = true;
				RedUID();
				Display_User->append(ErrGiven);	
				break;
			}
		}
	}

	//min 1 lineEdit is empty
	if(BufName.isEmpty()){
		//marking empty lineEdit Name
		RedName();
	}
	if(BufVName.isEmpty()){
		//marking empty lineEdit VName
		RedVName();
	}
	if(BufID.isEmpty()){
		//marking empty lineEdit Uid
		RedUID();
	}

	//all lineEdits filled
	if(!BufName.isEmpty() && !BufVName.isEmpty() && !BufID.isEmpty()){
			
	//setting strings of buffer user
	u.SetName(BufName);
	u.SetVName(BufVName);
	u.SetID(BufID);
	
	//check if bool given is false (new ID is not given)	
	if(!given){	
		//adding buffer user to ListUser
		//ListUser already filled
		if(!ListUser.isEmpty()){		
			bool inserted = false;
			int n = ListUser.size();	
			
			//searching lex position of names	
			for(int i = 0; i < n; i++){
		
				//mid position of names (set bool + display message + clear lineEdits)
				if(ListUser.at(i).cmpName(BufName) > 0){
					ListUser.insert(i, u);
					ListUserHit.append(u);
					Lends_U->append(u.display());
					In_Name->clear();
					In_VName->clear();
					In_Uid->clear();
					inserted = true;
					Display_User->append(UserInserted);
					WhiteUM();
					break;
						
				//identical name --> search for prenames
				}else if(ListUser.at(i).cmpName(BufName) == 0){
					bool inserted2 = false;
					int x = 0;
	
					//while names are identical search for prenames
					while(ListUser.at(i).cmpName(BufName) == 0 && i+x < n){
									
						//mid position of prenames (set bools + display message + clear lineEdits)
						if(ListUser.at(i+x).cmpVName(BufVName) > 0){
							ListUser.insert(i+x, u);
							ListUserHit.append(u);
							Lends_U->append(u.display());
							inserted2 = true;
							inserted = true;
							Display_User->append(UserInserted);
							In_Name->clear();
							In_VName->clear();
							In_Uid->clear();
							WhiteUM();
							break;
						}
						x++;	
					} // end of prename search

				//end positon of prenames (set bool + display message + clear lineEdits)
				if(!inserted2){
					ListUser.insert((i+x), u);
					ListUserHit.append(u);
					Lends_U->append(u.display());
					inserted = true;
					Display_User->append(UserInserted);
					In_Name->clear();
					In_VName->clear();
					In_Uid->clear();
					WhiteUM();
				}
				i = i + x;					
				}else{} // name is lex-larger then actual indexed name in name search --> do nothing + inc index
			} // end of name search
				
			//end position of names (dislay message + clear lineEdits)
			if(!inserted){
				ListUser.insert(n, u);
				ListUserHit.append(u);
				Lends_U->append(u.display());
				Display_User->append(UserInserted);
				In_Name->clear();
				In_VName->clear();
				In_Uid->clear();
				WhiteUM();
			}
	
		//first user to add (display message + clear lineEdits)
		}else{
			ListUser.append(u);
			ListUserHit.append(u);
			Lends_U->append(u.display());
			Display_User->append(UserInserted);
			In_Name->clear();
			In_VName->clear();
			In_Uid->clear();
			WhiteUM();
		}
	}
}
}

//searches users by ID, Name or VName
void bibo :: SearchUser(){

	//count hits
	int hitcount = 0;

	//clear old user search hit list
	ListUserHit.clear();

	//clear display and Mid lineEdit
	Lends_U->clear();
	this->In_MID->clear();
	Display_User->clear();

	//reset lineEdits to White
	WhiteUM();

	//bool search hit
	bool found = false;

	//buffer QString search
	QString BufName 	= this->In_Name->text();
	QString BufVName	= this->In_VName->text();
	QString BufID		= this->In_Uid->text();	

	//QString information messages
	QString HitID (QString::fromUtf8("Die ID lieferte eine Übereinstimmung:"));
	QString HitName(QString::fromUtf8("Der Name lieferte eine Uebereinstimmung:")); 
	QString ErrNoUser = "Keine Benutzer angelegt!";
	QString ErrNoHit = "Kein Treffer!";

	//ID should be unique --> if lineEdit Uid is filled search for ID
	if(!BufID.isEmpty()){

		//go through ListUser
		if(!ListUser.isEmpty()){
			int n = ListUser.size();
			for(int i = 0; i < n; i++){
				if(ListUser.at(i).cmpID(BufID)){
					//ID found --> display user
					Display_User->append(HitID);
					Display_User->append(ListUser.at(i).display());

					//add to search hit list and set found
					ListUserHit.append(ListUser.at(i));
					found = true;		
					hitcount ++;			

					//clear lineEdits
					In_Name->clear();
					In_VName->clear();
					In_Uid->clear();
				}
			}
		}else{
			Display_User->append(ErrNoUser);
		}

	//if lineEdit ID not filled search for name
	}else if(!BufName.isEmpty()){
	
		//lineEdit VName not filled
		if(BufVName.isEmpty()){		
	
			//go through ListUser
			if(!ListUser.isEmpty()){
				int n = ListUser.size();
				for(int i = 0; i < n; i++){
					if(ListUser.at(i).cmpName(BufName) == 0){
						//Name found --> display user
						Display_User->append(HitName);
						Display_User->append(ListUser.at(i).display());

						//add to search hit list ans set found
						ListUserHit.append(ListUser.at(i));
						found = true;
						hitcount ++;

						//clear lineEdits
						In_Name->clear();
						In_VName->clear();
						In_Uid->clear();
					}
				}
			}else{
			Display_User->append(ErrNoUser);
			}

		//lineEdit VName filled
		}else{

			//go through ListUser
			if(!ListUser.isEmpty()){
				int n = ListUser.size();
				for(int i = 0; i < n; i++){
					if(	ListUser.at(i).cmpName(BufName) == 0 &&
						 ListUser.at(i).cmpVName(BufVName) == 0){

						//Name and VName found --> display user
						Display_User->append(HitName);
						Display_User->append(ListUser.at(i).display());


						//add to search hit list and set found
						ListUserHit.append(ListUser.at(i));
						found = true;
						hitcount ++;
	
						//clear lineEdits
						In_Name->clear();
						In_VName->clear();
						In_Uid->clear();
					}
				}
			}else{
			Display_User->append(ErrNoUser);
			}
		}	

	//lineEdit ID and Name not filled search for VName
	}else if(!BufVName.isEmpty()){

		//go through ListUser
		if(!ListUser.isEmpty()){
			int n = ListUser.size();
			for(int i = 0; i < n; i++){
				if(ListUser.at(i).cmpVName(BufVName) == 0){

					//VName found --> display user
					Display_User->append(HitName);
					Display_User->append(ListUser.at(i).display());

					//add to search hit list and set found
					ListUserHit.append(ListUser.at(i));
					found = true;
					hitcount ++;
	
					//clear lineEdits
					In_Name->clear();
					In_VName->clear();
					In_Uid->clear();
				}
			}
		}else{
			Display_User->append(ErrNoUser);
		}

	//no input --> mark lineEdits
	}else{
		RedName();
		RedVName();
		RedUID();
	}

	if(!found){
		Display_User->append(ErrNoHit);
	}	

	if(hitcount == 1){
		Lends_U->append(ListUserHit.at(0).display());
	}
}

//earses all user(s) found by search and saved in user seach hit list
void bibo :: EraseUser(){
    
    //reset lineEdits to white
	WhiteUM();

	//QString information messages
	QString NumErase(QString::fromUtf8("Benutzer gelöscht"));
	QString ErrNoUser(QString::fromUtf8("Kein(e) Benutzer ausgewählt"));
	
	//counter erased users
	int EraseCount = 0;

	//clear display and Uid/Mid lineEdits
	Display_User->clear();
	Lends_U->clear();
	Lends_M->clear();	
	this->In_Uid->clear();
	this->In_MID->clear();

    int n = ListUserHit.size();
    
    for(int j = 0; j < n; j++){
        QMessageBox msgBox;
        msgBox.setText(QString::fromUtf8("Benutzer löschen?"));
        msgBox.setInformativeText(QString::fromUtf8("Damit werden alle Ausleihen gelöscht."));
        msgBox.addButton("JA", QMessageBox::AcceptRole);
        msgBox.addButton("NEIN", QMessageBox::RejectRole);
        int ret = msgBox.exec();
        if(ret == 0){
            int m = ListUser.size();
            for(int i = 0; i < m; i++){
                if(ListUser.at(i).cmpID(ListUserHit.at(j))){
                    QString BufUID = ListUser.at(i).GetID();
                    int k = ListLend.size();
                    for(int y = 0; y < k; y++){
                        if(ListLend.at(y).getUID().compare(BufUID) == 0){
                            QString BufMID = ListLend.at(y).getMID();
                            int a = ListBook.size();
                            for(int e = 0; e < a; e++){
                                if(ListBook.at(e).cmpID(BufMID)){
                                    book bbuf = ListBook.takeAt(e);
                                    bbuf.incAmount();
                                    ListBook.insert(e, bbuf);
                                    break;
                                }
                            }
                            a = ListDisc.size();
                            for(int e = 0; e < a; e++){
                                if(ListDisc.at(e).cmpID(BufMID)){
                                    disc dbuf = ListDisc.takeAt(e);
                                    dbuf.incAmount();
                                    ListDisc.insert(e, dbuf);
                                    break;
                                }
                            }
                            a = ListJournal.size();
                            for(int e = 0; e < a; e++){
                                if(ListJournal.at(e).cmpID(BufMID)){
                                    journal jbuf = ListJournal.takeAt(e);
                                    jbuf.incAmount();
                                    ListJournal.insert(e, jbuf);
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

}

//changes data of one user found by search (only filled lineEdits)
void bibo:: ChangeUser(){

	//buffer user
	user u;

	//clear display and Mid lineEdit + reset lineEdits to white
	Display_User->clear();
	Lends_U->clear();
	Lends_M->clear();	
	this->In_MID->clear();
	WhiteUM();

	//QString buffer for lineEdits
	QString BufName 	= this->In_Name->text();
	QString BufVName	= this->In_VName->text();

	//bool at least one lineEdit filled
	bool filled = false;

	//QString information messages
	QString ErrNoInput = "Keine Eingaben!";
	QString SuccesN(QString::fromUtf8("Name erfolgreich geändert!"));
	QString SuccesV(QString::fromUtf8("Vorname erfolgreich geändert!"));
	QString ErrNoUser(QString::fromUtf8("Kein Benutzer ausgewählt!"));
	QString ErrMoreUser(QString::fromUtf8("Mehr als ein Benutzer ausgewählt! Benutzerdaten können nur einzeln geändert werden!"));

	//check if exact one user is in user search hit list
	if(ListUserHit.size() == 1){
	
		//get user in user search hit list and go through user list to same user
		int n = ListUser.size() - 1;
		for(; n > 0; n--){
			if(ListUserHit.at(0).cmpID(ListUser.at(n))){
				break;
			}
		}

		//check which lineEdit is filled
		//change Name, display Succes, set filled and reset lineEdit color, clear user search hit list, clear lineEdits
		if(!BufName.isEmpty()){
			u.SetName(BufName);
			u.SetVName(ListUser.at(n).GetVName());
			u.SetID(ListUser.at(n).GetID());
			ListUser.replace(n, u);			
			Display_User->append(SuccesN);
			filled = true;
			ListUserHit.clear();
			In_Name->clear();
			In_VName->clear();
		}

		//change VName, display Succes, set filled and reset lineEdit color, clear user search hit list, clear lineEdits
		if(!BufVName.isEmpty()){
			u.SetVName(BufVName);
			u.SetName(ListUser.at(n).GetName());
			u.SetID(ListUser.at(n).GetID());
			ListUser.replace(n, u);
			Display_User->append(SuccesV);
			filled = true;
			ListUserHit.clear();
			In_Name->clear();
			In_VName->clear();
		}

		//display error message no input and mark empty lineEdits
		if(!filled){
			QString buf = this->In_Uid->text();
			if(buf.isEmpty()){
				Display_User->append(ErrNoInput);
				RedName();
				RedVName();
			}else{
				this->In_Uid->setText(QString::fromUtf8("nicht änderbar"));
				RedUID();
			}
		}

	//display error message no user in search user hit list
	}else if(ListUserHit.isEmpty()){
	Display_User->append(ErrNoUser);		

	//display error message more than one user in search user hit list
	}else{
		Display_User->append(ErrMoreUser);
	} 

	//clear user search hit list
	ListUserHit.clear();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//medium
//create new medium from lineEdit inputs
void bibo :: CreateMedium(){

	//clear book/disc/journal search hit list
	ListBookHit.clear();
	ListDiscHit.clear();
	ListJournalHit.clear();

	//clear Display and Uid lineEdit + reset lineEdits to white
	Display_M->clear();
	Lends_M->clear();	
	this->In_Uid->clear();
	WhiteUM();

	//buffer QStrings + clear lineEdits
	QString BufTitle 	= this->In_Titel->text();
	QString BufAutor	= this->In_Autor->text();
	QString BufVerlag	= this->In_Verlag->text();
	QString BufID		= this->In_MID->text();

	this->In_Titel->clear();
	this->In_Autor->clear();
	this->In_Verlag->clear();
	this->In_MID->clear();

	//bool new ID already given and QString error message
	bool given = false;
	//bool box false entry and QString error message
	bool box = false;
	//bool medium inserted
	bool inserted = false;

	//QString information messages
	QString  MediumInserted(QString::fromUtf8("Medium hinzugefügt!"));
	QString ErrBox(QString::fromUtf8("Buch, Zeitschrift oder CD/DVD auswählen!"));
	QString ErrGiven = "Diese ID ist bereits vergeben!";
		
	//check if new ID is already given --> set bool given (book list)
	if(!ListBook.isEmpty()){
		int l = ListBook.size();
		for(int j = 0; j < l; j++){

			//hit + mark ID lineEdit, set given and display error message
			if(ListBook.at(j).cmpID(BufID)){
				given = true;
				RedMID();
				Display_M->append(ErrGiven);	
				break;
			}
		}
	}

	//check if new ID is already given --> set bool given (disc list)
	if(!ListDisc.isEmpty()){
		int l = ListDisc.size();
		for(int j = 0; j < l; j++){

			//hit + mark ID lineEdit, set given and display error message
			if(ListDisc.at(j).cmpID(BufID)){
				given = true;
				RedMID();
				Display_M->append(ErrGiven);
				break;
			}
		}
	}

	// check if new ID is already given --> set bool given (journal list)
	if(!ListJournal.isEmpty()){
		int l = ListJournal.size();
		for(int j = 0; j < l; j++){

			//hit + mark ID lineEdit, set given and display error mesaage
			if(ListJournal.at(j).cmpID(BufID)){
				given = true;
				RedMID();
				Display_M->append(ErrGiven);
				break;
			}	
		}
	}

	//marking empty lineEdits
	if(BufTitle.isEmpty()){
		RedTitle();
	}
	if(BufAutor.isEmpty()){
		RedAutor();
	}
	if(BufVerlag.isEmpty()){
		RedVerlag();
	}
	if(BufID.isEmpty()){
		RedMID();
	}
	
	if(!BufTitle.isEmpty() && !BufAutor.isEmpty() && !BufVerlag.isEmpty() && !BufID.isEmpty() && !given){

		//buffer medium objects;	
		book b;
		disc d;
		journal j;

		//check if book is selected as medium type --> add to book list
		if(this->Box_MArt->currentText().compare("Buch") == 0){

			box = true;

			//set buffer QStrings as object members
			b.SetTitle(BufTitle);
			b.SetAutor(BufAutor);
			b.SetVerlag(BufVerlag);
			b.SetID(BufID);

			//get page number from dialogue
			bool ok;
   			int pages = QInputDialog :: getInt(this, "Seitenzahl (max 5000)", "Seitenzahl : ", 0, 1, 5000, 1, &ok);
   			int amount = QInputDialog :: getInt(this, "Exemplare (max. 20) ", "Exemplare: ", 0, 1, 20, 1, &ok);
			if (ok && pages > 0) {
				b.SetPages(pages);
				b.setTotal(amount);
				//add buffer book to ListBook
				int n = ListBook.size();
				if(n > 0){
					//search for lex-position of title
					for(int i = 0; i < n; i++){
						if(ListBook.at(i).cmpTitle(BufTitle)){
							ListBook.insert(i, b);
							ListBookHit.append(b);
							Lends_M->append(b.display());	
							inserted = true;						
							break;
						}
					}
	
				//end position
				if(!inserted){
					ListBook.append(b);	
					ListBookHit.append(b);
					Lends_M->append(b.display());		
				}

				}else{
					//first book to add
					ListBook.append(b);
					ListBookHit.append(b);
					Lends_M->append(b.display());
				}
			}
		}

		//check if disc is selected as medium type --> add to disc list
		if(this->Box_MArt->currentText().compare("CD/DVD") == 0){ 
			
			box = true;

			//set buffer QStrings as object members
			d.SetTitle(BufTitle);
			d.SetAutor(BufAutor);
			d.SetVerlag(BufVerlag);
			d.SetID(BufID);

			//get duration from dialogue
			bool ok;
   			int dur = QInputDialog :: getInt(this, "Dauer (max 180 min)", "Dauer (min) : ", 0, 1, 180, 1, &ok);
   			int amount = QInputDialog :: getInt(this, "Exemplare (max. 20) ", "Exemplare: ", 0, 1, 20, 1, &ok);
			if (ok && dur > 0) {
				d.SetDur(dur);
				d.setTotal(amount);
				//add buffer disc to ListDisc				
				int n = ListDisc.size();
				if(n > 0){
					//search for lex-position of title
					for(int i = 0; i < n; i++){
						if(ListDisc.at(i).cmpTitle(BufTitle)){
							ListDisc.insert(i, d);
							ListDiscHit.append(d);
							Lends_M->append(d.display());	
							inserted = true;						
							break;
						}
					}
	
				//end position
				if(!inserted){
					ListDisc.append(d);	
					ListDiscHit.append(d);
					Lends_M->append(d.display());
				}

				}else{
					//first disc to add
					ListDisc.append(d);
				 	ListDiscHit.append(d);
					Lends_M->append(d.display());
				}
			}
		}

		//check if journal is selelcted as medium type ->> add to journal list
		if(this->Box_MArt->currentText().compare("Zeitschrift") == 0){ 
	
			box = true;

			//set buffer QStrings as object members
			j.SetTitle(BufTitle);
			j.SetAutor(BufAutor);
			j.SetVerlag(BufVerlag);
			j.SetID(BufID);

			//get page number from dialogue
			bool ok;
   			int pages = QInputDialog :: getInt(this, "Seitenzahl (max 200)", "Seitenzahl : ", 0, 1, 250, 1, &ok);
   			int amount = QInputDialog :: getInt(this, "Exemplare (max. 20) ", "Exemplare: ", 0, 1, 20, 1, &ok);
			if (ok && pages > 0) {
				j.SetPages(pages);
				j.setTotal(amount);
				//add buffer journal to ListJournal
				int n = ListJournal.size();
				if(n > 0){
					//search for lex-position of title
					for(int i = 0; i < n; i++){
						if(ListJournal.at(i).cmpTitle(BufTitle)){
							ListJournal.insert(i, j);	
							ListJournalHit.append(j);
							Lends_M->append(j.display());
							inserted = true;						
							break;
						}
					}
	
					//end position
					if(!inserted){
						ListJournal.append(j);	
						ListJournalHit.append(j);
						Lends_M->append(j.display());		
					}

				}else{
					//first journal to add
					ListJournal.append(j);
					ListJournalHit.append(j);
					Lends_M->append(j.display());
				}	
			}
		}

		//error message if none or all is selected in box medium type
		if(!box){
			Display_M->append(ErrBox);
		}
	}
}

void bibo :: SearchMedium(){
	
	//count hits
	int hitcount = 0;

	//clear old hit lists
	ListJournalHit.clear();
	ListBookHit.clear();
	ListDiscHit.clear();

	//clear display and Uid lineEdit + reset lineEdits to white
	Lends_M->clear();	
	Display_M->clear();
	this->In_Uid->clear();
	WhiteUM();

	//bool search hit
	bool found = false;

	//buffer QString search
	QString BufTitle 	= this->In_Titel->text();
	QString BufAutor	= this->In_Autor->text();
	QString BufVerlag	= this->In_Verlag->text();
	QString BufID		= this->In_MID->text();

	//QString information messages
	QString HitID(QString::fromUtf8("Die ID lieferte eine Übereinstimmung:"));
	QString HitTitle (QString::fromUtf8("Der Titel/Autor lieferte eine Übereinstimmung :")); 
	QString ErrNoMedium = "Kein Medium angelegt!";
	QString ErrNoHit = "Kein Treffer!";

	//id as parameter 
	if(!BufID.isEmpty()){
		//book
		if(this->Box_MArt->currentText().compare("Buch") == 0){
			int n = ListBook.size();
			for(int i = 0; i < n; i++){
				if(ListBook.at(i).cmpID(BufID)){
					ListBookHit.append(ListBook.at(i));
					this->Display_M->append("Treffer mit dieser ID:");
					this->Display_M->append(ListBook.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
		//disc
		}else if(this->Box_MArt->currentText().compare("CD/DVD") == 0){
			int n = ListDisc.size();
			for(int i = 0; i < n; i++){
				if(ListDisc.at(i).cmpID(BufID)){
					ListDiscHit.append(ListDisc.at(i));
					this->Display_M->append("Treffer mit dieser ID:");
					this->Display_M->append(ListDisc.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
		//journal
		}else if(this->Box_MArt->currentText().compare("Zeitschrift") == 0){
			int n = ListJournal.size();
			for(int i = 0; i < n; i++){
				if(ListJournal.at(i).cmpID(BufID)){
					ListJournalHit.append(ListJournal.at(i));
					this->Display_M->append("Treffer mit dieser ID:");
					this->Display_M->append(ListJournal.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
		//all types
		}else{
			int n = ListBook.size();
			for(int i = 0; i < n; i++){
				if(ListBook.at(i).cmpID(BufID)){
					ListBookHit.append(ListBook.at(i));
					this->Display_M->append("Buch mit dieser ID: ");
					this->Display_M->append(ListBook.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
			n = ListDisc.size();
			for(int i = 0; i < n; i++){
				if(ListDisc.at(i).cmpID(BufID)){
					ListDiscHit.append(ListDisc.at(i));
					this->Display_M->append("CD/DVD mit dieser ID: ");
					this->Display_M->append(ListDisc.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
			n = ListJournal.size();
			for(int i = 0; i < n; i++){
				if(ListJournal.at(i).cmpID(BufID)){
					ListJournalHit.append(ListJournal.at(i));
					this->Display_M->append("Zeitschrift mit dieser ID: ");
					this->Display_M->append(ListJournal.at(i).display());
					hitcount ++;
					found = true;
					break;
				}
			}
		}

		if(!found){
			//search for other parameters
			if(!BufTitle.isEmpty() || !BufAutor.isEmpty() || BufVerlag.isEmpty()){
				//books
				if(this->Box_MArt->currentText().compare("Zeitschrift") == 0){
					int n = ListJournal.size();
					for(int i = 0; i < n; i++){
						if(ListJournal.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListJournalHit.append(ListJournal.at(i));
							this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
							this->Display_M->append(ListJournal.at(i).display());
							hitcount ++;
							found = true;
						}
					}
				//disc
				}else if(this->Box_MArt->currentText().compare("CD/DVD") == 0){
					int n = ListDisc.size();
					for(int i = 0; i < n; i++){
						if(ListDisc.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListDiscHit.append(ListDisc.at(i));
							this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
							this->Display_M->append(ListDisc.at(i).display());
							hitcount ++;
							found = true;
						}
					}
				//journal
				}else if(this->Box_MArt->currentText().compare("Zeitschrift") == 0){
					int n = ListJournal.size();
					for(int i = 0; i < n; i++){
						if(ListJournal.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListJournalHit.append(ListJournal.at(i));
							this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
							this->Display_M->append(ListJournal.at(i).display());
							hitcount ++;
							found = true;
						}
					}
				// allt types
				}else{
					int n = ListBook.size();
					for(int i = 0; i < n; i++){
						if(ListBook.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListBookHit.append(ListBook.at(i));
							this->Display_M->append("Buch mit diesem Titel/Autor/Verlag: ");
							this->Display_M->append(ListBook.at(i).display());
							hitcount ++;
							found = true;
						}
					}
					n = ListDisc.size();
					for(int i = 0; i < n; i++){
						if(ListDisc.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListDiscHit.append(ListDisc.at(i));
							this->Display_M->append("CD/DVD mit diesem Titel/Autor/Verlag: ");
							this->Display_M->append(ListDisc.at(i).display());
							hitcount ++;
							found = true;
						}
					}
					n = ListJournal.size();
					for(int i = 0; i < n; i++){
						if(ListJournal.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
							ListJournalHit.append(ListJournal.at(i));
							this->Display_M->append("Zeitschrift mit diesem Titel/Autor/Verlag: ");
							this->Display_M->append(ListJournal.at(i).display());
							hitcount ++;
							found = true;
						}
					}
				}
			}
		}

	//title, autor, verlag as parameter
	}else if(!BufTitle.isEmpty() || !BufAutor.isEmpty() || !BufVerlag.isEmpty()){

		//search for other parameters
		if(!BufTitle.isEmpty() || !BufAutor.isEmpty() || BufVerlag.isEmpty()){
			//books
			if(this->Box_MArt->currentText().compare("Buch") == 0){
				int n = ListBook.size();
				for(int i = 0; i < n; i++){
					if(ListBook.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListBookHit.append(ListBook.at(i));
						this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
						this->Display_M->append(ListBook.at(i).display());
						hitcount ++;
						found = true;
					}
				}
			//disc
			}else if(this->Box_MArt->currentText().compare("CD/DVD") == 0){
				int n = ListDisc.size();
				for(int i = 0; i < n; i++){
					if(ListDisc.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListDiscHit.append(ListDisc.at(i));
						this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
						this->Display_M->append(ListDisc.at(i).display());
						hitcount ++;
						found = true;
					}
				}
			//journal
			}else if(this->Box_MArt->currentText().compare("Zeitschrift") == 0){
				int n = ListJournal.size();
				for(int i = 0; i < n; i++){
					if(ListJournal.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListJournalHit.append(ListJournal.at(i));
						this->Display_M->append("Treffer mit diesem Titel/Autor/Verlag:");
						this->Display_M->append(ListJournal.at(i).display());
						hitcount ++;
						found = true;
					}
				}
			// allt types
			}else{
				int n = ListBook.size();
				for(int i = 0; i < n; i++){
					if(ListBook.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListBookHit.append(ListBook.at(i));
						this->Display_M->append("Buch mit diesem Titel/Autor/Verlag: ");
						this->Display_M->append(ListBook.at(i).display());
						hitcount ++;
						found = true;
					}
				}
				n = ListDisc.size();
				for(int i = 0; i < n; i++){
					if(ListDisc.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListDiscHit.append(ListDisc.at(i));
						this->Display_M->append("CD/DVD mit diesem Titel/Autor/Verlag: ");
						this->Display_M->append(ListDisc.at(i).display());
						hitcount ++;
						found = true;
					}
				}
				n = ListJournal.size();
				for(int i = 0; i < n; i++){
					if(ListJournal.at(i).cmpOther(BufTitle, BufAutor, BufVerlag)){
						ListJournalHit.append(ListJournal.at(i));
						this->Display_M->append("Zeitschrift mit diesem Titel/Autor/Verlag: ");
						this->Display_M->append(ListJournal.at(i).display());
						hitcount ++;
						found = true;
					}
				}
			}
		}

	//everything empty --> mark line edits
	}else{
		RedTitle();
		RedAutor();
		RedVerlag();
		RedMID();
	}

	//error message no hit
	if(!found){
		this->Display_M->append("Kein Treffer!");
	}

	//if 1 medium found display in medium display of lends	
	if(hitcount == 1){
		if(!ListBookHit.isEmpty())
			this->Lends_M->append(ListBookHit.at(0).display());	
		else if(!ListDiscHit.isEmpty())
			this->Lends_M->append(ListDiscHit.at(0).display());
		else
			this->Lends_M->append(ListJournalHit.at(0).display());		
	}
}

void bibo :: EraseMedium(){
    
    QMessageBox msgBox;
    msgBox.setText(QString::fromUtf8("Medium löschen?"));
    msgBox.setInformativeText(QString::fromUtf8("Damit werden alle Ausleihen gelöscht."));
    msgBox.addButton("JA", QMessageBox::AcceptRole);
    msgBox.addButton("NEIN", QMessageBox::RejectRole);
    int ret = msgBox.exec();

	//QString information messages 
	QString NumErase(QString::fromUtf8("Medien geloescht"));
	QString ErrNoMediums(QString::fromUtf8("Keine Medien ausgewählt! Benutzen Sie dazu die Suche."));

	//clear display and Uid/Mid lineEdits + reset lineEdits to white
	Lends_U->clear();
	Lends_M->clear();	
	Display_M->clear();
	this->In_Uid->clear();
	this->In_MID->clear();
	WhiteUM();
    
    if(ret == 0){
        int n = ListBookHit.size();
        for(int i = 0; i < n; i++){
            int m = ListBook.size();
            for(int j = 0; j < m; j++){
                if(ListBookHit.at(i).cmpID(ListBook.at(j))){
                    QString MID = ListBook.at(j).GetID();
                    ListBook.removeAt(j);
                    ListBookHit.removeAt(i);
                    int l = ListLend.size();
                    for(int y = 0; y < l; y++){
                        if(ListLend.at(y).getMID().compare(MID) == 0){
                            ListLend.removeAt(y);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        n  = ListDiscHit.size();
        for(int i = 0; i < n; i++){
            int m = ListDisc.size();
            for(int j = 0; j < m; j++){
                if(ListDiscHit.at(i).cmpID(ListDisc.at(j))){
                    QString MID = ListDisc.at(j).GetID();
                    ListDisc.removeAt(j);
                    ListDiscHit.removeAt(i);
                    int l = ListLend.size();
                    for(int y = 0; y < l; y++){
                        if(ListLend.at(y).getMID().compare(MID) == 0){
                            ListLend.removeAt(y);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        n = ListJournalHit.size();
        for(int i = 0; i < n; i++){
            int m = ListJournal.size();
            for(int j = 0; j < m; j++){
                if(ListJournalHit.at(i).cmpID(ListJournal.at(j))){
                    QString MID = ListJournal.at(j).GetID();
                    ListJournal.removeAt(j);
                    ListJournalHit.removeAt(i);
                    int l = ListLend.size();
                    for(int y = 0; y < l; y++){
                        if(ListLend.at(y).getMID().compare(MID) == 0){
                            ListLend.removeAt(y);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void bibo :: ChangeMedium(){

	//check if exactly one medium is chosen
	int hitCount = ListBookHit.size() + ListJournalHit.size() + ListDiscHit.size();

	//QString information messages
	QString ErrNoMedium(QString::fromUtf8("Kein Medium ausgewählt!"));
	QString ErrManyMediums(QString::fromUtf8("Mehr als ein Medium ausgewählt!"));
	QString Succes(QString::fromUtf8("Erfolgreich geändert!"));
	
	//reset lineEdits to white + clear display and Uid lineEdits
	WhiteUM();
	Display_M->clear();
	Lends_U->clear();
	Lends_M->clear();	
	this->In_Uid->clear();

	//id should not be changed --> if id lineEdit filled throw error
	if(!this->In_MID->text().isEmpty()){
		this->In_MID->setText(QString::fromUtf8("nicht änderbar"));
		RedMID();
	}
	
	if(hitCount == 1){
		if(!ListBookHit.isEmpty()){
		//book chosen
			int n = ListBook.size();
			for(int i = 0; i < n; i++){
				if(ListBook.at(i).cmpID(ListBookHit.at(0))){
					book b;
					b.SetTitle(ListBook.at(i).GetTitle());
					b.SetAutor(ListBook.at(i).GetAutor());
					b.SetVerlag(ListBook.at(i).GetVerlag());
					b.SetID(ListBook.at(i).GetID());
					b.SetPages(ListBook.at(i).GetPages());
					QString BufTitle = this->In_Titel->text();
					QString BufAutor = this->In_Autor->text();
					QString BufVerlag = this->In_Verlag->text();
					if(!BufTitle.isEmpty())
						b.SetTitle(BufTitle);
					if(!BufAutor.isEmpty())
						b.SetAutor(BufAutor);
					if(!BufVerlag.isEmpty())
						b.SetVerlag(BufVerlag);
					bool ok = false;
   					int pages = QInputDialog :: getInt(this, "Seitenzahl (max 5000)", "Seitenzahl : ", ListBook.at(i).GetPages(), 1, 5000, 1, &ok);
					if(ok){
						b.SetPages(pages);
						ListBook.replace(i ,b);
						ListBookHit.clear();
						Display_M->append(Succes);
					}
					break;
				}
			}
		}else if(!ListJournalHit.isEmpty()){
		//journal chosen
			int n = ListJournal.size();
			for(int i = 0; i < n; i++){
				if(ListJournal.at(i).cmpID(ListJournalHit.at(0))){
					journal j;
					j.SetTitle(ListJournal.at(i).GetTitle());
					j.SetAutor(ListJournal.at(i).GetAutor());
					j.SetVerlag(ListJournal.at(i).GetVerlag());
					j.SetID(ListJournal.at(i).GetID());
					j.SetPages(ListJournal.at(i).GetPages());
					QString BufTitle = this->In_Titel->text();
					QString BufAutor = this->In_Autor->text();
					QString BufVerlag = this->In_Verlag->text();
					if(!BufTitle.isEmpty())
						j.SetTitle(BufTitle);
					if(!BufAutor.isEmpty())
						j.SetAutor(BufAutor);
					if(!BufVerlag.isEmpty())
						j.SetVerlag(BufVerlag);
					bool ok = false;
   					int pages = QInputDialog :: getInt(this, "Seitenzahl (max 200)", "Seitenzahl : ", ListJournal.at(i).GetPages(), 1, 200, 1, &ok);
					if(ok){
						j.SetPages(pages);
						ListJournal.replace(i ,j);
						ListJournalHit.clear();
						Display_M->append(Succes);
					}
					break;
				}
			}
		}else{
		//disc chosen
			int n = ListDisc.size();
			for(int i = 0; i < n; i++){
				if(ListDisc.at(i).cmpID(ListDiscHit.at(0))){
					disc d;
					d.SetTitle(ListDisc.at(i).GetTitle());
					d.SetAutor(ListDisc.at(i).GetAutor());
					d.SetVerlag(ListDisc.at(i).GetVerlag());
					d.SetID(ListDisc.at(i).GetID());
					d.SetDur(ListDisc.at(i).GetDur());
					QString BufTitle = this->In_Titel->text();
					QString BufAutor = this->In_Autor->text();
					QString BufVerlag = this->In_Verlag->text();
					if(!BufTitle.isEmpty())
						d.SetTitle(BufTitle);
					if(!BufAutor.isEmpty())
						d.SetAutor(BufAutor);
					if(!BufVerlag.isEmpty())
						d.SetVerlag(BufVerlag);
					bool ok = false;
   					int dur = QInputDialog :: getInt(this, "Dauer (max 180 min)", "Dauer (min) : ", ListDisc.at(i).GetDur(), 1, 180, 1, &ok);
					if(ok){
						d.SetDur(dur);
						ListDisc.replace(i ,d);
						ListDiscHit.clear();
						Display_M->append(Succes);
					}
					break;
				}
			}
		}
	}else if(hitCount == 0){
		Display_M->append(ErrNoMedium);
	}else if(hitCount < 1){
		Display_M->append(ErrManyMediums);
	}else{
	}

}

void bibo :: Lend(){

	//clear lineEdits and displays
	this->In_Titel->clear();
	this->In_Autor->clear();
	this->In_Verlag->clear();
	this->In_MID->clear();
	this->In_Name->clear();
	this->In_VName->clear();
	this->In_Uid->clear();
	this->Display_User->clear();
	this->Display_M->clear();
	this->Lends_U->clear();
	this->Lends_M->clear();
	this->Display_L->clear();

	int lmc = ListBookHit.size() + ListDiscHit.size() + ListJournalHit.size();
	if(ListUserHit.size() == 1 && lmc == 1){
	//exactly one user and one medium choosen		
		//QInputDialog to get duration of lend
		bool ok;
   		int dur = QInputDialog :: getInt(this, "Leihdauer in Tagen", "max 60: ", 0, 1, 60, 1, &ok);
		if (ok){
			//set lend object and add to list
			lends l;
			l.setDate(dur);
			l.setUid(ListUserHit.at(0).GetID());
			if(!ListBookHit.isEmpty()){
				int n = ListBook.size();
				for(int i = 0; i < n; i++){
					if(ListBookHit.at(0).cmpID(ListBook.at(i))){
						book buf = ListBook.takeAt(i);
						if(buf.decAmount()){
							ListBook.insert(i, buf);
							l.setMid(buf.GetID());
							l.setUid(ListUserHit.at(0).GetID());
							ListUserHit.clear();
							ListLend.append(l);
							Display_L->append("Buch verliehen.");
							break;
						}else{
							ListBook.insert(i, buf);	
							ListUserHit.clear();
							Display_L->append("Dieses Buch ist vergriffen.");
							break;
						}
					}
				}
			}else if(!ListDiscHit.isEmpty()){
				int n = ListDisc.size();
				for(int i = 0; i < n; i++){
					if(ListDiscHit.at(0).cmpID(ListDisc.at(i))){
						disc buf = ListDisc.takeAt(i);
						if(buf.decAmount()){
							ListDisc.insert(i, buf);
							l.setMid(buf.GetID());
							l.setUid(ListUserHit.at(0).GetID());
							ListUserHit.clear();
							ListLend.append(l);
							Display_L->append("CD/DVD verliehen");
							break;
						}else{
							ListDisc.insert(i, buf);
							ListUserHit.clear();
							Display_L->append("Diese CD/DVD ist vergriffen.");
							break;
						}
					}
				}			
			}else{
				int n = ListJournal.size();
				for(int i = 0; i < n; i++){
					if(ListJournalHit.at(0).cmpID(ListJournal.at(i))){
						journal buf = ListJournal.takeAt(i);
						if(buf.decAmount()){
							ListJournal.insert(i, buf);
							l.setMid(buf.GetID());
							l.setUid(ListUserHit.at(0).GetID());
							ListUserHit.clear();
							ListLend.append(l);
							Display_L->append("Zeitschrift verliehen");
							break;
						}else{
							ListJournal.insert(i, buf);
							ListUserHit.clear();
							Display_L->append("Diese Zeitschrift ist vergriffen.");
							break;
						}
					}
				}
			}
		}else
			Display_L->append("Fehler");		
	}else{
		//error message to much or no mediums/users choosen
		Display_L->append(QString::fromUtf8("Wählen Sie genau ein Medium und einen Benutzer aus."));
	}

}

void bibo :: Restore(){

	//clear lineEdits and displays
	this->In_Titel->clear();
	this->In_Autor->clear();
	this->In_Verlag->clear();
	this->In_MID->clear();
	this->In_Name->clear();
	this->In_VName->clear();
	this->In_Uid->clear();
	this->Display_User->clear();
	this->Display_M->clear();
	this->Lends_U->clear();
	this->Lends_M->clear();
	this->Display_L->clear();

	int lmc = ListBookHit.size() + ListDiscHit.size() + ListJournalHit.size();
	if(ListUserHit.size() == 1 && lmc == 1){
	//exactly one user and one medium choosen		
		if(!ListBookHit.isEmpty()){
			int n = ListLend.size();
			for(int i = 0; i < n; i++){
				if(ListUserHit.at(0).cmpID(ListLend.at(i).getUID()) && ListBookHit.at(0).cmpID(ListLend.at(i).getMID())){
					int m = ListBook.size();
					for(int j = 0; j < m; j++){
						if(ListBook.at(j).cmpID(ListBookHit.at(0))){
							book buf = ListBook.takeAt(j);
							buf.incAmount();
							ListBook.insert(j, buf);
							break;
						}
					}

					if(ListLend.at(i).due()){
						Display_L->append(QString::fromUtf8("Die Rückgabe des Buches ist ") + QString :: number(- ListLend.at(i).daysLeft()) + QString::fromUtf8(" Tage überfällig."));
					}else{
						Display_L->append(QString::fromUtf8("Pünktliche Rückgabe."));
					}
					break;
				}
			}
			}else if(!ListDiscHit.isEmpty()){
				int n = ListLend.size();
				for(int i = 0; i < n; i++){
					if(ListUserHit.at(0).cmpID(ListLend.at(i).getUID()) && ListDiscHit.at(0).cmpID(ListLend.at(i).getMID())){

						int m = ListDisc.size();
						for(int j = 0; j < m; j++){
							if(ListDisc.at(j).cmpID(ListDiscHit.at(0))){
								disc buf = ListDisc.takeAt(j);
								buf.incAmount();
								ListDisc.insert(j, buf);
								break;
							}
						}

						if(ListLend.at(i).due()){
							Display_L->append(QString::fromUtf8("Die Rückgabe des Buches ist ") + QString :: number(- ListLend.at(i).daysLeft()) + QString::fromUtf8(" Tage überfällig."));
						}else{
							Display_L->append(QString::fromUtf8("Pünktliche Rückgabe."));
						}
						break;
					}
				}
			}else if(!ListJournalHit.isEmpty()){
				int n = ListLend.size();
				for(int i = 0; i < n; i++){
					if(ListUserHit.at(0).cmpID(ListLend.at(i).getUID()) && ListJournalHit.at(0).cmpID(ListLend.at(i).getMID())){

						int m = ListJournal.size();
						for(int j = 0; j < m; j++){
							if(ListJournal.at(j).cmpID(ListJournalHit.at(0))){
								journal buf = ListJournal.takeAt(j);
								buf.incAmount();
								ListJournal.insert(j, buf);
								break;
							}
						}

						if(ListLend.at(i).due()){
							Display_L->append(QString::fromUtf8("Die Rückgabe des Buches ist ") + QString :: number(- ListLend.at(i).daysLeft()) + QString::fromUtf8(" Tage überfällig."));
						}else{
							Display_L->append(QString::fromUtf8("Pünktliche Rückgabe."));
						}
						break;
					}
				}
			}else{
			}
	}else{
		Display_L->append(QString::fromUtf8("Wählen Sie genau einen Benutzer und ein Medium aus"));
	}
}

void bibo :: DisplayLends(){

	//clear lineEdits and displays
	this->In_Titel->clear();
	this->In_Autor->clear();
	this->In_Verlag->clear();
	this->In_MID->clear();
	this->In_Name->clear();
	this->In_VName->clear();
	this->In_Uid->clear();
	this->Display_User->clear();
	this->Display_M->clear();
	this->Lends_U->clear();
	this->Lends_M->clear();
	this->Display_L->clear();

	if(ListUserHit.size() == 1){
		int n = ListLend.size();
		//go through llist of lends
		for(int i = 0; i < n; i++){
			//check if lend has same uid as chosen user
			if(ListLend.at(i).cmpUID(ListUserHit.at(0))){
				QString bufMID = ListLend.at(i).getMID();
				int m = ListBook.size();
				for(int j = 0; j < m; j++){
					if(ListBook.at(j).cmpID(bufMID)){
						this->Display_L->append("Buch gefunden. Verbleibende Leihdauer: " + QString :: number(ListLend.at(i).daysLeft()));
						this->Display_L->append(ListBook.at(j).display());
						break;
					}
				}
				m = ListDisc.size();
				for(int j = 0; j < m; j++){
					if(ListDisc.at(j).cmpID(bufMID)){
						this->Display_L->append("CD/DVD gefunden. Verbleibende Leihdauer: " + QString :: number(ListLend.at(i).daysLeft()));
						this->Display_L->append(ListDisc.at(j).display());
						break;
					}
				}
				m = ListJournal.size();
				for(int j = 0; j < m; j++){
					if(ListJournal.at(j).cmpID(bufMID)){
						this->Display_L->append("Zeitschrift gefunden. Verbleibende Leihdauer: " + QString :: number(ListLend.at(i).daysLeft()));
						this->Display_L->append(ListJournal.at(j).display());
						break;
					}
				}
			}
		}
	}else{
		this->Display_L->append(QString::fromUtf8("Wählen Sie genau einen Benutzer aus."));
	}
}

void bibo :: DueLends(){

	//clear lineEdits and displays
	this->In_Titel->clear();
	this->In_Autor->clear();
	this->In_Verlag->clear();
	this->In_MID->clear();
	this->In_Name->clear();
	this->In_VName->clear();
	this->In_Uid->clear();
	this->Display_User->clear();
	this->Display_M->clear();
	this->Lends_U->clear();
	this->Lends_M->clear();
	this->Display_L->clear();
    
    bool found = false;

	int n = ListLend.size();
	for(int i = 0; i < n; i++ ){
		if(ListLend.at(i).due()){
			QString bufUID = ListLend.at(i).getUID();
			QString bufMID = ListLend.at(i).getMID();
			int n = ListBook.size();
			for(int i = 0; i < n; i++){
				if(ListBook.at(i).cmpID(bufMID)){
					Display_L->append(QString::fromUtf8("fälliges Buch gefunden: "));
					Display_L->append(ListBook.at(i).display());
					int m = ListUser.size();
					for(int j = 0; j < m; j++){
						if(ListUser.at(j).cmpID(bufUID)){
							Display_L->append("ausgeliehen von: ");
							Display_L->append(ListUser.at(i).display());
                            found = true;
							break;
						}
					}
					break;
				}
			}
			n = ListDisc.size();
			for(int i = 0; i < n; i++){
				if(ListDisc.at(i).cmpID(bufMID)){
					Display_L->append(QString::fromUtf8("fällige CD/DVD gefunden: "));
					Display_L->append(ListDisc.at(i).display());
					int m = ListUser.size();
					for(int j = 0; j < m; j++){
						if(ListUser.at(j).cmpID(bufUID)){
							Display_L->append("ausgeliehen von: ");
							Display_L->append(ListUser.at(i).display());
                            found = true;
							break;
						}
					}
					break;
				}
			}
			n = ListJournal.size();
			for(int i = 0; i < n; i++){
				if(ListJournal.at(i).cmpID(bufMID)){
					Display_L->append(QString::fromUtf8("fällige Zeitschrift gefunden: "));
					Display_L->append(ListJournal.at(i).display());
					int m = ListUser.size();
					for(int j = 0; j < m; j++){
						if(ListUser.at(j).cmpID(bufUID)){
							Display_L->append("ausgeliehen von: ");
							Display_L->append(ListUser.at(i).display());
                            found = true;
							break;
						}
					}
					break;
				}
			}
		}
	}
	
	if(!found){
        Display_L->append(QString::fromUtf8("Keine fälligen Ausleihen gefunden."));
    }
}


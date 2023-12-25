#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper.h"

#define VBUFSIZE 128
#define ORTSIZE 20
#define KUNRSIZE 10
#define NAMESIZE 20


int getNiederlassung(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, char* pNiderl);
int getAuftraege(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, int* pMitID);
int getInfo(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, int* pMitID);
int getMitarbeiter(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, char* pNiederlassung, int* pMitID);

int main(int argc, char *argv[]) {
  
  	
  // sql vars
  SQLHENV henv = SQL_NULL_HENV; // environment
  SQLHDBC hdbc = SQL_NULL_HDBC; // connection
  SQLHSTMT hstmt = SQL_NULL_HSTMT; // statement

  // input vars
  char pNiederl[ORTSIZE + 1];
  int MitID;
  // sql connect
  sqlConnect(&henv, &hdbc, &hstmt);

  // program logic
  while(1){
	if(getNiederlassung(&henv, &hdbc, &hstmt, pNiederl) && getMitarbeiter(&henv, &hdbc, &hstmt, pNiederl, &MitID)){
  		getInfo(&henv, &hdbc, &hstmt, &MitID);	
  		getAuftraege(&henv, &hdbc, &hstmt, &MitID);
	}
  }
  puts("");

  // sql disconnect
  sqlDisconnect(&henv, &hdbc, &hstmt);

  return 0;
}

int getNiederlassung(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, char* pNiederl){

	//sql vars
	SQLCHAR niederl[ORTSIZE + 1];
	SQLLEN bindLen;
	SQLRETURN rc;

	//init buffer
	char vBuf[VBUFSIZE];
	
	//prepare SQL Statement
	char* sqlStatement = "SELECT DISTINCT(Ort) FROM Niederlassung";
	if(DEBUGINFO) printf("sqlStatement: %s\n", sqlStatement);

	//perform SQL Statement
	rc = SQLExecDirect(*hstmt, (SQLCHAR*) sqlStatement, SQL_NTS);
	if(!checkReturnCode("SQLExecDirect [hstmt, sqlStatement]", rc, *hstmt, SQL_HANDLE_STMT)){
		freeSQLHandles(henv, hdbc, hstmt); 
		exit(0);
	}

	//bind columns
	rc = SQLBindCol(*hstmt, 1, SQL_C_CHAR, niederl, ORTSIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 1, niederl]", rc, *hstmt, SQL_HANDLE_STMT);

	puts("");

	//fetch and print
	printf("Niederlassungsorte:\n");
	int i = 0;
	while((rc = SQLFetch(*hstmt)) != SQL_NO_DATA){
		if(rc == SQL_ERROR){
			printf("SQLFetch[hstmt]: SQL_ERROR\n");
			break;
		}
		if(rc == SQL_SUCCESS_WITH_INFO){
			printf("SQLFetch[hstmt]: SQL_SUCCESS_WITH_INFO\n");
		}
		
		printf(" %-*s\n",ORTSIZE, niederl);
		i++;
	}	

	//cancel SQL Statement
	rc = SQLCancel(*hstmt);
	if(!checkReturnCode("SQLCancel[hstmt]", rc, *hstmt, SQL_HANDLE_STMT)){
		freeSQLHandles(henv, hdbc, hstmt);
		exit(0);
	}

	if(i == 0) {printf("Keine Niederlassungen gefunden"); return 0;}	


	// get input
  	printf("\n Ort ? ");
  	fgets(vBuf, VBUFSIZE, stdin);
  	vBuf[strlen(vBuf) - 1] = 0;

        if(vBuf[0] == 'Q' || vBuf[0] == 'q'){
        	
		printf("Programm wird beendet.\n");               

                // sql disconnect
                sqlDisconnect(henv, hdbc, hstmt);

                exit(0);
        }

  	strncpy(pNiederl, vBuf, ORTSIZE); 

  	return strlen(pNiederl);
}	

int getMitarbeiter(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, char* pNiederl, int* pMitID){
	
	//SQL vars
	SQLINTEGER MitID;
	SQLCHAR MitName[NAMESIZE + 1];
	SQLCHAR MitVorname[NAMESIZE + 1];
	SQLLEN bindLen;
	SQLRETURN rc = 0;

	//input buffer
	char vBuf[VBUFSIZE];

	//prepare SQL Statement
	char* part1 = "SELECT MitID, MitVorname, MitName FROM Mitarbeiter M JOIN Niederlassung N ON M.MitNLNr = N.NLNr WHERE N.Ort ='";
	char* part2 = "'";
	int size = strlen(part1) + strlen(pNiederl) + strlen(part2) + 1;
	char* sqlStatement = malloc(sizeof(char) * size);
	
	//error handling malloc null pointer
	if(sqlStatement == NULL){
	        
		//cancel SQL Statement
		rc = SQLCancel(*hstmt);
        	if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
                	freeSQLHandles(henv, hdbc, hstmt); exit(0);
        	}

		// sql disconnect
  		sqlDisconnect(henv, hdbc, hstmt);
	}
	
	sprintf(sqlStatement, "%s%s%s", part1, pNiederl, part2);
	if(DEBUGINFO) printf("sqlStatement: %s\n", sqlStatement);

	//perform SQL Statement
	rc = SQLExecDirect(*hstmt, (SQLCHAR*) sqlStatement, SQL_NTS);
	if(!checkReturnCode("SQLExecDirect [hstmt, sqlStatement]", rc, *hstmt, SQL_HANDLE_STMT)){
		freeSQLHandles(henv, hdbc, hstmt);
		exit(0);
	}	

	//bind colums
	rc = SQLBindCol(*hstmt, 1, SQL_C_LONG, &MitID, 0, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 1, MitID]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 2, SQL_C_CHAR, MitVorname, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 2, MitVorname]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 3, SQL_C_CHAR, MitName, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 3, MitName]", rc, *hstmt, SQL_HANDLE_STMT);

	puts("");

	//fetch and print
	printf("Mitarbeiter in %s\n", pNiederl);
	int i = 0;
	while((rc = SQLFetch(*hstmt)) != SQL_NO_DATA){
		if(rc == SQL_ERROR){
			printf("SQLFetch [hstmt]: SQL_ERROR\n");
			break;
		}	
		if(rc == SQL_SUCCESS_WITH_INFO){
			printf("SQLFetch [hstmt]: SQL_SUCCESS_WITH_INFO\n");
		}	
	
		printf("  %d %-*s %-*s\n", MitID, NAMESIZE, MitVorname, NAMESIZE, MitName);

		i++;	
	}	

	// cancel statement handle
	rc = SQLCancel(*hstmt);
  	if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
    		freeSQLHandles(henv, hdbc, hstmt); exit(0);
  	}

  	if (i == 0) {
    		printf("Es gibt keine Mitarbeiter in %s\n", pNiederl);
		return 0;
  	}

	//free memory
	free(sqlStatement);
	
	// get input
        printf("\n Mitarbeiter (MitID) ? ");
        fgets(vBuf, VBUFSIZE, stdin);
        vBuf[strlen(vBuf) - 1] = 0;
		
	if(vBuf[0] == 'Q' || vBuf[0] == 'q'){
		
		printf("Programm wird beendet.\n"); 
				
                // sql disconnect
                sqlDisconnect(henv, hdbc, hstmt);

		exit(0);
	}
        

	if(isdigit(*vBuf)){
		*pMitID = atoi(vBuf);
		return *pMitID;
	}else{
		printf("Mitarbeiter-IDs können nur Zahlen sein\n");
		return 0;
	}	
}

int getInfo(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, int* pMitID){

	//SQL vars
	SQLINTEGER MitID;
	SQLCHAR MitName[NAMESIZE + 1];
	SQLCHAR MitVorname[NAMESIZE + 1];
	SQLCHAR MitJob[NAMESIZE + 1];
	SQLDOUBLE MitStundensatz;
	SQLINTEGER NLNr;
	SQLCHAR Niederlassung[ORTSIZE + 1];
	SQLLEN bindLen;

	//prepare SQL Statement
	int rc;
	char* part1 = "SELECT MitID, MitName, MitVorname, MitJob, MitStundensatz, M.MitNLNr, N.Ort FROM Mitarbeiter M JOIN Niederlassung N ON N.NLNr=M.MitNLNr WHERE M.MitID=";
	char* part2 = ";";
	char* cMitID = malloc(sizeof(char) * 10);
	sprintf(cMitID, "%d", *pMitID);
	int size = strlen(part1) + strlen(cMitID) + strlen(part2);
	char* sqlStatement = malloc(sizeof(char) * size);	
	
	//error handling malloc null pointer
        if(sqlStatement == NULL){

                //cancel SQL Statement
                rc = SQLCancel(*hstmt);
                if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
                        freeSQLHandles(henv, hdbc, hstmt); exit(0);
                }

                // sql disconnect
                sqlDisconnect(henv, hdbc, hstmt);
        }
	
	sprintf(sqlStatement, "%s%s%s", part1, cMitID, part2);
	
	//perform SQL Statement
        rc = SQLExecDirect(*hstmt, (SQLCHAR*) sqlStatement, SQL_NTS);
        if(!checkReturnCode("SQLExecDirect [hstmt, sqlStatement]", rc, *hstmt, SQL_HANDLE_STMT)){
                freeSQLHandles(henv, hdbc, hstmt);
                exit(0);
        }

	//bind columns
	rc = SQLBindCol(*hstmt, 1, SQL_C_LONG, &MitID, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 1, MitID]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 2, SQL_C_CHAR, MitName, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 2, MitName]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 3, SQL_C_CHAR, MitVorname, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 3, MitVorname]", rc, *hstmt, SQL_HANDLE_STMT);
		
	//Gebursttag nicht gefordert

	rc = SQLBindCol(*hstmt, 4, SQL_C_CHAR, MitJob, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 5, MitJob]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 5, SQL_C_DOUBLE, &MitStundensatz, 1024, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 6, MitStundensatz]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 6, SQL_C_LONG, &NLNr, 0, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 7 , NLNr]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 7, SQL_C_CHAR, Niederlassung, ORTSIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 8, Niderlassung]", rc, *hstmt, SQL_HANDLE_STMT);

        puts("");

	//fetch and print
        rc = SQLFetch(*hstmt); 
		
        if(rc == SQL_ERROR){
		printf("SQLFetch [hstmt]: SQL_ERROR\n");
       	}
        if(rc == SQL_SUCCESS_WITH_INFO){
        	printf("SQLFetch [hstmt]: SQL_SUCCESS_WITH_INFO\n");
        }

	if(rc == SQL_NO_DATA){
                printf("Es gibt keinen Mitarbeiter mit dieser Mitarbeiter-ID\n");
		
		//free memory
		free(sqlStatement);

		//cancel statement handle
		rc = SQLCancel(*hstmt);
		if(checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)){
			freeSQLHandles(henv, hdbc, hstmt);
		}
		
		return 0;
	}else{
       		printf("Mitarbeiter: %s %s\n Stellung: %s\n Stundensatz: %.2lf\n Niederlassung: %-20s (%d)\n", MitVorname, MitName, MitJob, MitStundensatz, Niederlassung, NLNr);
       		puts("");
	}

	//free memory
        free(sqlStatement);

        // cancel statement handle
        rc = SQLCancel(*hstmt);
        if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
        	freeSQLHandles(henv, hdbc, hstmt); exit(0);
        }

	return 1;
}


int getAuftraege(SQLHENV* henv, SQLHDBC* hdbc, SQLHSTMT* hstmt, int* pMitID){

	//new SQL vars
        SQLINTEGER AufNr;
        SQLINTEGER KunNr;
        SQL_TIMESTAMP_STRUCT AufDat;
        SQL_TIMESTAMP_STRUCT ErlDat;
        SQLDOUBLE Anfahrt;
	SQLLEN bindLen;
	SQLCHAR KunName[NAMESIZE + 1];
	SQLCHAR KunOrt[ORTSIZE + 1];
	SQLCHAR KunStrasse[ORTSIZE + 1];
	SQLCHAR KunPLZ[NAMESIZE + NAMESIZE + 1];

	
	//prepare Statement
       	int rc;
	char* sqlStatement;	
	char* part3 = "SELECT A.AufNr, A.KunNr, A.AufDat, A.ErlDat, A.Anfahrt, K.KunName, K.KunOrt, K.KunStrasse, K.KunPLZ FROM Auftrag A JOIN Kunde K ON A.KunNr=K.KunNr WHERE Dauer IS NULL AND DATEPART(ww, ErlDat) =  DATEPART(ww, GETDATE())+1  AND DATEPART(yyyy, ErlDat) = DATEPART(yyyy, GETDATE()) AND MitID ='";
        char* part4 = "'";
	char* cMitID = malloc(sizeof(char) * 10);
        sprintf(cMitID, "%d", *pMitID);
        int size = strlen(part3) + strlen(cMitID) + strlen(part4);
        sqlStatement = malloc(sizeof(char) * size);
        
	//error handling malloc null pointer
        if(sqlStatement == NULL){

                //cancel SQL Statement
                rc = SQLCancel(*hstmt);
                if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
                        freeSQLHandles(henv, hdbc, hstmt); exit(0);
                }

                // sql disconnect
                sqlDisconnect(henv, hdbc, hstmt);
        }
	
	sprintf(sqlStatement, "%s%s%s", part3, cMitID, part4);

        //perform SQL Statement
        rc = SQLExecDirect(*hstmt, (SQLCHAR*) sqlStatement, SQL_NTS);
        if(!checkReturnCode("SQLExecDirect [hstmt, sqlStatement]", rc, *hstmt, SQL_HANDLE_STMT)){
                freeSQLHandles(henv, hdbc, hstmt);
                exit(0);
        }

        //bind columns
        rc = SQLBindCol(*hstmt, 1, SQL_C_LONG, &AufNr, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 1, AufNr]", rc, *hstmt, SQL_HANDLE_STMT);

        rc = SQLBindCol(*hstmt, 2, SQL_C_LONG, &KunNr, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 2, KunNr]", rc, *hstmt, SQL_HANDLE_STMT);

        rc = SQLBindCol(*hstmt, 3, SQL_C_TIMESTAMP, &AufDat, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 3, AufDat]", rc, *hstmt, SQL_HANDLE_STMT);

        rc = SQLBindCol(*hstmt, 4, SQL_C_TIMESTAMP, &ErlDat, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 4, ErlDat]", rc, *hstmt, SQL_HANDLE_STMT);

        rc = SQLBindCol(*hstmt, 5, SQL_C_DOUBLE, &Anfahrt, 0, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 5, Anfahrt]", rc, *hstmt, SQL_HANDLE_STMT);
	
	rc = SQLBindCol(*hstmt, 6, SQL_C_CHAR, KunName, NAMESIZE + 1, &bindLen);
        checkReturnCode("SQLBindCol [hstmt, 6, KunName]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 7, SQL_C_CHAR, KunOrt, ORTSIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 7, KunOrt]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 8, SQL_C_CHAR, KunStrasse, ORTSIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 8, KunOrt]", rc, *hstmt, SQL_HANDLE_STMT);

	rc = SQLBindCol(*hstmt, 9, SQL_C_CHAR, KunPLZ, NAMESIZE + 1, &bindLen);
	checkReturnCode("SQLBindCol [hstmt, 9, KunPLZ]", rc, *hstmt, SQL_HANDLE_STMT);

	

       	//fetch and print
        int i = 0;
        while((rc = SQLFetch(*hstmt)) != SQL_NO_DATA){
                if(rc == SQL_ERROR){
                        printf("SQLFetch [hstmt]: SQL_ERROR\n");
                        break;
                }
                if(rc == SQL_SUCCESS_WITH_INFO){
                        printf("SQLFetch [hstmt]: SQL_SUCCESS_WITH_INFO\n");
                }

               	printf("Auftragsnummer: %-11d, Kundennummer: %-11d, Auftragsdatum: %-2d.%-2d.%-9d Erledigungsdatum: %-2d.%-2d.%-4d Anfahrt: %.2lf\n", AufNr, KunNr, AufDat.day, AufDat.month, AufDat.year, ErlDat.day, ErlDat.month, ErlDat.year, Anfahrt);
		printf("Kunde: %-20s, Ort: %-20s, Strasse: %-20s, PLZ: %s\n", KunName, KunOrt, KunStrasse, KunPLZ);
		printf("----------------------------------------------------------------------------------------------------------------------------\n\n");

                i++;
        }
	
	if(i == 0){printf("Für diesen Mitarbeiter gibt es keine zu erledigenden Aufträge.\n");}
	
	//free memory
        free(sqlStatement);

        // cancel statement handle
        rc = SQLCancel(*hstmt);
        if (!checkReturnCode("SQLCancel [hstmt]", rc, *hstmt, SQL_HANDLE_STMT)) {
                freeSQLHandles(henv, hdbc, hstmt); exit(0);
        }

	
	return 1;
}


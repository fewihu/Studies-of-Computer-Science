-- Aufgabe: 5.1
-- ----------------------------------------------------------
ALTER TABLE Einbau ADD CONSTRAINT Einbau_unique UNIQUE (FzNr, BtNr);
--Table EINBAU geändert.
-- Aufgabe: 5.2
-- ----------------------------------------------------------
CREATE TABLE Hersteller AS SELECT * FROM db01.Hersteller;
--Table HERSTELLER erstellt.
SELECT * FROM Hersteller;
-- Aufgabe: 5.3
-- ----------------------------------------------------------
SELECT * FROM all_constraints where owner = 'DB01' and table_name ='HERSTELLER';
-- Nr und Name NOT NULL
ALTER TABLE Hersteller MODIFY(HSTNAME NOT NULL);
ALTER TABLE Hersteller MODIFY(HSTNR NOT NULL);
--jeweils Fehlermeldung: "column to be modified to NOT NULL is already NOT NULL"
--scheinbar Constraint doch übernommen?
SELECT * FROM ALL_TAB_COLUMNS where owner = 'DB01' and table_name ='HERSTELLER';
SELECT * FROM ALL_CONS_COLUMNS where owner = 'DB01' and table_name='HERSTELLER';
--HstNr ist Primary Key
ALTER TABLE Hersteller ADD CONSTRAINT PK_Hersteller PRIMARY KEY(HSTNR);
--Table HERSTELLER geändert.
-- Aufgabe: 5.4
-- ----------------------------------------------------------
INSERT INTO Hersteller (HSTNR, HSTNAME, STRASSE, PLZ, ORT, KONTAKTAUFNAHME)
VALUES ('693253', 'Tower Zwickau', 'Kopernikusstr.60', '08056', 'Zwickau', (SELECT TO_DATE(SYSDATE, 'DD.MM.YY') FROM DUAL));
--1 Zeile eingefügt.
-- Aufgabe: 5.5
-- ----------------------------------------------------------
SELECT HSTNAME, PLZ, ORT FROM Hersteller ORDER BY PLZ;
-- Aufgabe: 5.6
-- ----------------------------------------------------------
SELECT * FROM Hersteller WHERE SYSDATE - KONTAKTAUFNAHME > 5*365;
--Differenz zweier DATE Werte in Tagen
-- Aufgabe: 5.7
-- ----------------------------------------------------------
ALTER TABLE Bauteil MODIFY (HstNr REFERENCES Hersteller(HSTNR));
--an alter table validating constraint failed because the table has child records.
--es gibt Herstellernummer in Bauteil, die es nicht in Hersteller gibt
--Herstellernummer müssen in Hersteller eingefügt werden
INSERT INTO Hersteller (HSTNR, HSTNAME)
VALUES ((SELECT UNIQUE HSTNR FROM BAUTEIL WHERE HstNR NOT IN (SELECT HSTNR FROM Hersteller)), 'unbekannt');
-- 1 Zeile eingefügt.
--Fremdschlüssel kann nun gesetzt werden: Table BAUTEIL geändert.
--Alternative: Bauteile mit HSTNR NOT IN Hersteller löschen
-- Aufgabe: 6.1
-- ----------------------------------------------------------
CREATE OR REPLACE TYPE TAdresse AS OBJECT(
Strasse VARCHAR2(50), Plz VARCHAR2(5), Ort VARCHAR2(50)
);
-- Type TADRESSE kompiliert
-- Aufgabe: 6.2
-- ----------------------------------------------------------
CREATE TABLE Lieferant(LiefNr NUMBER(6) PRIMARY KEY, Name VARCHAR2(20) NOT NULL, Adresse TAdresse
);
--Table LIEFERANT erstellt.
-- Aufgabe: 6.3
-- ----------------------------------------------------------
CREATE OR REPLACE VIEW Lieferant_OV(LiefNr, Name, Strasse, Plz, Ort) AS
SELECT l.LiefNr, l.Name, l.Adresse.Strasse, l.Adresse.Plz, l.Adresse.Ort
FROM Lieferant l;
--View LIEFERANT_OV erstellt.
SELECT * FROM Lieferant_OV;
-- Aufgabe: 6.4
-- ----------------------------------------------------------
INSERT INTO Lieferant_OV VALUES(752443, 'Laepple Teublitz', 'Maxstr. 12', '93158', 'Teublitz');
-- Damit in eine VIEW inserted werden kann darf sie keine Objekte enhalten
-- Aufgabe: 7.1
-- ----------------------------------------------------------
CREATE OR REPLACE TRIGGER Lieferant_OV_ins
INSTEAD OF INSERT
ON Lieferant_OV
FOR EACH ROW
BEGIN
IF INSERTING THEN
INSERT INTO Lieferant
VALUES(:NEW.LiefNr, :NEW.Name, TAdresse(:NEW.Strasse, :NEW.Plz, :NEW.Ort));
END IF;
END;
/
--Trigger LIEFERANT_OV_INS kompiliert
INSERT INTO Lieferant_OV VALUES(752443, 'Laepple Teublitz', 'Maxstr. 12', '93158', 'Teublitz');
--1 Zeile eingefügt.
SELECT * FROM Lieferant_OV;
-- neuer Datensatz eingefügt
-- Aufgabe: 7.2
-- ----------------------------------------------------------
--a)
CREATE SEQUENCE sq_einbau INCREMENT BY 1 START WITH 2000;
--Sequence SQ_EINBAU erstellt.
--b)
CREATE OR REPLACE TRIGGER einbauPK
BEFORE INSERT OR UPDATE ON Einbau
FOR EACH ROW
BEGIN
IF INSERTING THEN
SELECT 'E' || sq_einbau.NEXTVAL
INTO :NEW.EbNr FROM dual;
END IF;
IF UPDATING THEN
:NEW.EbNr := :OLD.Ebnr;
END IF;
END;
/
--Trigger EINBAUPK kompiliert
--c)
INSERT INTO Einbau VALUES(NULL, 5000, 10000, 1);
--1 Zeile eingefügt.
SELECT * FROM Einbau;
--EbNr E2000 vergeben-- Aufgabe: 7.3
-- ----------------------------------------------------------
CREATE OR REPLACE TRIGGER einbauPK
BEFORE INSERT OR UPDATE ON Einbau
FOR EACH ROW
DECLARE
exce EXCEPTION;
cnt INT;
BEGIN
IF INSERTING THEN
SELECT COUNT(*) INTO cnt
FROM Bauteil b
WHERE b.BAUGRUPPE IS NULL AND :NEW.Btnr = b.Btnr;
IF cnt = 0 THEN
RAISE exce;
END IF;
SELECT 'E' || sq_einbau.NEXTVAL
INTO :NEW.EbNr FROM dual;
END IF;
IF UPDATING THEN
:NEW.EbNr := :OLD.Ebnr;
END IF;
EXCEPTION
WHEN exce THEN
raise_application_error(-20999, 'Einfügen nicht möglich, da keine Baugruppe.');
END;
/
--Trigger EINBAUPK kompiliert
-- Aufgabe: 7.3
-- ----------------------------------------------------------
INSERT INTO Einbau VALUES(NULL, 5001, 10001, 1);
INSERT INTO Einbau VALUES(NULL, 5010, 10002, 2);
SELECT * FROM Einbau;
SELECT BtNr, Baugruppe FROM Bauteil WHERE BtNr IN (5010,5001);
--5001 eingefügt, 5010 nicht
-- Aufgabe: 8.1
-- ----------------------------------------------------------
SET serveroutput on
CREATE OR REPLACE PROCEDURE MaxGewicht (limit IN INT)
IS
CURSOR cur IS
SELECT FzNr, Bezeichnung, Gewicht
FROM Fahrzeug
WHERE Gewicht <= limit;
BEGIN
FOR item IN cur
LOOP
DBMS_OUTPUT.PUT_LINE('FzNr:
' || item.FzNr);
DBMS_OUTPUT.PUT_LINE('Bezeichnung: ' || item.Bezeichnung);
DBMS_OUTPUT.PUT_LINE('Gewicht:
' || item.Gewicht);
DBMS_OUTPUT.PUT_LINE('----------------------------------------');
END LOOP;
END;
/
--Procedure MAXGEWICHT kompiliert
EXEC MaxGewicht(850);
-- Aufgabe: 8.2
-- ----------------------------------------------------------
CREATE OR REPLACE PROCEDURE readEinbau
IS
CURSOR cur1 IS
SELECT DISTINCT Bezeichnung, f.FzNr
FROM Fahrzeug f JOIN Einbau e ON e.FzNr = f.FzNr;CURSOR cur2 IS
SELECT Teilename, e.FzNr
FROM Bauteil b JOIN Einbau e ON e.BTNR = b.BTNR;
BEGIN
FOR fz IN cur1
LOOP
DBMS_OUTPUT.PUT_LINE(fz.Bezeichnung);
FOR bt IN cur2
LOOP
IF bt.FzNr = fz.FzNr THEN
DBMS_OUTPUT.PUT_LINE('.
'||bt.Teilename);
END IF;
END LOOP;
END LOOP;
END;
/
--Procedure READEINBAU kompiliert
EXEC readEinbau;
-- Aufgabe: 9.1
-- ----------------------------------------------------------
GRANT SELECT,INSERT ON Hersteller TO s99999;
--Grant erfolgreich.
-- Aufgabe: 9.2
-- ----------------------------------------------------------
SELECT * FROM s79138.Hersteller;
-- Aufgabe: 9.3
-- ----------------------------------------------------------
REVOKE SELECT,INSERT ON Hersteller FROM s99999;
--Revoke erfolgreich.

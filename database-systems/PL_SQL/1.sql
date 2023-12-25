-- Aufgabe: 1.1
-- ----------------------------------------------------------
CREATE TABLE Fahrzeug (
FzNr NUMBER(5) PRIMARY KEY,
Bezeichnung VARCHAR2(15) NOT NULL,
Gewicht float(20));
CREATE TABLE Bauteil (
BtNr number(5) Primary Key,
Teilename varchar2(20) Not Null,
Einbauzeit number(5),
HstNr varchar2(10),
Baugruppe number (5),
CONSTRAINT FK_BAUTEIL FOREIGN KEY (Baugruppe) REFERENCES Bauteil (BtNr));
CREATE TABLE Einbau (
EbNr varchar2(10) Primary Key,
BtNr number (5) Not Null,
FzNr number (5) Not Null,
Anzahl number (5),
CONSTRAINT FK_BT FOREIGN KEY (BtNr) REFERENCES Bauteil (BtNr),
CONSTRAINT FK_FZ FOREIGN KEY (FzNr) REFERENCES Fahrzeug (FzNr)
ON DELETE CASCADE);
--Table FAHRZEUG erstellt.
--Table BAUTEIL erstellt.
--Table EINBAU erstellt.
-- Aufgabe: 2.1
-- ----------------------------------------------------------
CREATE OR REPLACE TYPE TPreis AS OBJECT(
Preis NUMBER(10,2),
MEMBER FUNCTION Netto RETURN NUMBER,
MEMBER FUNCTION Brutto RETURN NUMBER,
MEMBER FUNCTION Umrechnung(Faktor NUMBER) RETURN NUMBER);
--Type TPREIS kompiliert
-- Aufgabe: 2.2
-- ----------------------------------------------------------
CREATE OR REPLACE TYPE BODY TPreis AS
MEMBER FUNCTION Netto
RETURN NUMBER IS
BEGIN
RETURN Preis;
END;
MEMBER FUNCTION Brutto
RETURN NUMBER IS
BEGIN
RETURN Preis * 1.19;
END;
MEMBER FUNCTION Umrechnung(Faktor NUMBER)
RETURN NUMBER IS
BEGIN
RETURN Preis * Faktor;
END;
END;
--Type Body TPREIS kompiliert
-- Aufgabe: 2.3
-- ----------------------------------------------------------
ALTER TABLE Fahrzeug ADD Preis TPreis;74
--Table FAHRZEUG geändert.
-- Aufgabe: 3.1
-- ----------------------------------------------------------
create type AnzTueren as varray(5) of varchar(10);
--Type ANZTUEREN kompiliert
-- Aufgabe: 3.2
-- ----------------------------------------------------------
ALTER TABLE Fahrzeug ADD Tuerzahl AnzTueren;
--Table FAHRZEUG geändert.-- Aufgabe: 3.3
-- ----------------------------------------------------------
INSERT INTO Fahrzeug VALUES(10000, 'BMW Z4 Roadstar', 900, TPreis(60000),
AnzTueren('2-Türer'));
INSERT INTO Fahrzeug VALUES(10001, 'VW Golf GTI', 800, TPreis(25000),
AnzTueren('3-Türer', '5-Türer'));
INSERT INTO Fahrzeug VALUES(10002, 'Audi A3', 850, TPREIS(30000),
AnzTueren('3-Türer'));
-- je: 1 Zeile eingefügt.
-- Aufgabe: 3.4
-- ----------------------------------------------------------
SELECT FzNr, Bezeichnung, f.preis.brutto() as Bruttopreis, tt.*
TABLE(f.Tuerzahl) tt;
from fahrzeug f,
SELECT FzNr, Bezeichnung, f.preis.umrechnung(1.5) as Umrechnung, tt.*
f, TABLE(f.Tuerzahl) tt;
from fahrzeug
-- Aufgabe: 3.5
-- ----------------------------------------------------------
CREATE OR REPLACE PROCEDURE bruttoD
IS
tt AnzTueren;
bez varchar2(15);
bpreis number(10,2);
BEGIN
FOR emp IN (SELECT FzNr FROM Fahrzeug) LOOP
SELECT f.tuerzahl, f.bezeichnung, f.preis.brutto() INTO tt, bez, bpreis FROM
fahrzeug f
WHERE FzNr = emp.FzNr;
IF tt(1) = '3-Türer' THEN
dbms_output.put_line(bez || ': ' ||bpreis);
END IF;
END LOOP emp;
END bruttoD;
BEGIN bruttoD(); END;
-- Aufgabe: 4.1
-- ----------------------------------------------------------
CREATE OR REPLACE TYPE tPreisentwicklung AS OBJECT (
PeNr varchar2(10), Netto number(10,2), Datum date
);
-- Type TPREISENTWICKLUNG kompiliert
-- Aufgabe: 4.2
-- ----------------------------------------------------------
CREATE OR REPLACE TYPE ntPreisentwicklung AS TABLE OF tPreisentwicklung;141
-- Type NTPREISENTWICKLUNG kompiliert
-- Aufgabe: 4.3
-- ----------------------------------------------------------
ALTER TABLE Bauteil ADD
Preis ntPreisentwicklung
NESTED TABLE Preis STORE AS PreisentwicklungNT;
--Table BAUTEIL geändert.
-- Aufgabe: 4.4
-- ----------------------------------------------------------
DELETE Bauteil;
INSERT INTO Bauteil (BtNr, Teilename, Einbauzeit, HstNr, Baugruppe, Preis) VALUES
(5000, 'Tuer links',
20, 134556, NULL, ntPreisentwicklung());
INSERT INTO Bauteil (BtNr, Teilename, Einbauzeit, HstNr, Baugruppe, Preis) VALUES
(5001, 'Spiegel rechts',10, 588797, NULL, ntPreisentwicklung());
INSERT INTO Bauteil (BtNr, Teilename, Einbauzeit, HstNr, Baugruppe, Preis) VALUES
(5002, 'Auspuff',
30, 693253, NULL, ntPreisentwicklung());-- je 1 Zeile eingefügt.
INSERT INTO TABLE(SELECT Preis FROM Bauteil WHERE BtNr = 5000) VALUES (7007, 900,
TO_DATE('2020/10/12', 'YYYY/MM/DD'));
INSERT INTO TABLE(SELECT Preis FROM Bauteil WHERE BtNr = 5001) VALUES (7008, 100,
TO_DATE('2020/10/12', 'YYYY/MM/DD'));
INSERT INTO TABLE(SELECT Preis FROM Bauteil WHERE BtNr = 5002) VALUES (7009,2000,
TO_DATE('2020/10/12', 'YYYY/MM/DD'));
-- je 1 Zeile eingefügt.
-- Aufgabe: 4.4
-- ----------------------------------------------------------
insert into Table (select b.Preis from bauteil b where b.BtNr=5000) values ('7019',
910, to_date('20.10.2020'));
insert into bauteil values (5003, 'Tuergriff links', 15, '134556', 5000,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5003) values ('7010',
20, to_date('12.10.2020'));
insert into Table (select b.Preis from bauteil b where b.BtNr=5003) values ('7018',
22, to_date('12.11.2020'));
insert into bauteil values (5004, 'Schlossriegel links', 5, '134556', 5000,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5004) values ('7011',
10, to_date('12.10.2020'));
insert into bauteil values (5005, 'Türgummi', 3, '134556', 5000,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5005) values ('7012',
5, to_date('12.10.2020'));
insert into bauteil values (5006, 'Auspuffrohr', 25, '693253', 5002,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5006) values ('7013',
30, to_date('12.10.2020'));
insert into bauteil values (5007, 'Schelle', 5, '693253', 5002, ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5007) values ('7014',
0.5, to_date('12.10.2020'));
insert into bauteil values (5008, 'Schraube', 1, '693253', 5007,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5008) values ('7015',
0.1, to_date('12.10.2020'));
insert into bauteil values (5009, 'Spiegelglas', 10, '588797', 5001,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5009) values ('7016',
2.30, to_date('12.10.2020'));
insert into bauteil values (5010, 'Gehäuse', 12, '693253', 5001,
ntPreisentwicklung());
insert into Table (select b.Preis from bauteil b where b.BtNr=5010) values ('7017',
6, to_date('12.10.2020'));
-- ohne Fehlermeldung
SELECT BtNr, Teilename, Einbauzeit, HstNr, Baugruppe FROM Bauteil;
-- insgesamt 11 Tupel, 3 ohne Baugruppe
-- Aufgabe: 4.6
-- ----------------------------------------------------------194
--a)
SELECT Level, Teilename FROM Bauteil CONNECT BY PRIOR BtNr = Baugruppe START WITH
BtNr IN (SELECT BtNR FROM Bauteil WHERE Baugruppe IS NULL);
--b)
SELECT LPAD(' ',3*Level-3) ||Teilename FROM Bauteil CONNECT BY PRIOR BtNr =
Baugruppe START WITH BtNr IN (SELECT BtNR FROM Bauteil WHERE Baugruppe IS NULL);
--c)
SELECT LPAD(' ',3*Level-3) ||Teilename AS Teil,
(CASE Level WHEN 1 THEN Einbauzeit ELSE NULL END) AS Einbau,
(CASE Level WHEN 1 THEN HstNr ELSE NULL END) AS Hersteller,
(CASE Level WHEN 1 THEN BtNr ELSE NULL END) AS Nummer
FROM Bauteil CONNECT BY PRIOR BtNr = Baugruppe START WITH BtNr = 5002;
-- Aufgabe: 4.7
-- ----------------------------------------------------------
SELECT b.Btnr, b.Teilename, p.Netto, p.Datum FROM Bauteil b, table(b.Preis) p;-- Aufgabe: 4.8
-- ----------------------------------------------------------
SELECT b.Teilename
FROM (SELECT * FROM Bauteil ORDER BY BtNr) b
WHERE ROWNUM <= 10;
-- Aufgabe: 4.9
-- ----------------------------------------------------------
--SELECT * FROM Bauteil;
SELECT *
FROM ( SELECT Baugruppe, b.Btnr, b.Teilename, b.Einbauzeit,
RANK() OVER (PARTITION BY Baugruppe ORDER BY b.Einbauzeit DESC) as r
FROM Bauteil b
WHERE Baugruppe IS NOT NULL)
WHERE r = 1;

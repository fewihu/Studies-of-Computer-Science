-- Aufgabe: 1.1
-- ----------------------------------------------------------
CREATE TABLE Mitarbeiter(
MitID CHAR(3) NOT NULL,
MitName VARCHAR(20) NOT NULL,
MitVorname VARCHAR(20) NULL,
MitGebDat DATE NOT NULL,
MitJob VARCHAR(20) NOT NULL,
MitStundensatz SMALLMONEY NULL,
MitEinsatzort VARCHAR(20) NULL,
CONSTRAINT PK_Mitarbeiter PRIMARY KEY (MitID)
);
CREATE TABLE Kunde(
KunNr int NOT NULL,
KunName VARCHAR(20) NOT NULL,
KunOrt VARCHAR(20) NULL,
KunPLZ char(5) NOT NULL,
KunStrasse varchar(20) NOT NULL,
CONSTRAINT PK_Kunde PRIMARY KEY (KunNR)
);
CREATE TABLE Ersatzteil(
EtID char(5) NOT NULL,
EtBezeichnung varchar(100) NOT NULL,
EtPreis smallmoney NOT NULL,
EtAnzLager int NOT NULL,
EtHersteller varchar(30) NOT NULL,
CONSTRAINT PK_Ersatzteil PRIMARY KEY (EtID)
);
-- Lösung:
--Die Befehle wurden erfolgreich ausgeführt.
-- Aufgabe: 1.2
-- ----------------------------------------------------------
INSERT INTO Mitarbeiter SELECT * FROM trommelhelden..quelleMitarbeiter;
INSERT INTO Kunde SELECT * FROM trommelhelden..quelleKunde;
INSERT INTO Ersatzteil SELECT * FROM trommelhelden..quelleErsatzteil;
-- Lösung: (12 Zeilen betroffen), (501 Zeilen betroffen), (24 Zeilen betroffen)
-- Aufgabe: 1.3
-- ----------------------------------------------------------
SELECT * INTO Auftrag FROM trommelhelden..quelleAuftrag;
SELECT * INTO Montage FROM trommelhelden..quelleMontage;
-- Lösung: (956 Zeilen betroffen), (820 Zeilen betroffen)
-- Aufgabe: 1.3
-- ----------------------------------------------------------
SELECT * INTO Auftrag FROM trommelhelden..quelleAuftrag;
SELECT * INTO Montage FROM trommelhelden..quelleMontage;
-- Lösung: (956 Zeilen betroffen), (820 Zeilen betroffen)
-- Aufgabe: 1.4
-- ----------------------------------------------------------
-- Tabelle Auftrag
--AufNR wird Primary Key von Auftrag
ALTER TABLE Auftrag
ADD CONSTRAINT PK_AufNR PRIMARY KEY (AufNR);
--MitID referenziert MitID aus Mitarbeiter
ALTER TABLE Auftrag
ADD CONSTRAINT FK_MitID
FOREIGN KEY (MitID) REFERENCES Mitarbeiter(MitID);
--KunNR referenziert KunNr aus Kunde
ALTER TABLE Auftrag
ADD CONSTRAINT FK_KunNr
FOREIGN KEY (KunNr) REFERENCES Kunde(KunNr);
-- Tabelle Montage
--EtID und AufNr wird zus. Primary KeyALTER TABLE Montage
ADD CONSTRAINT PK_Montage PRIMARY KEY (EtID,AufNR);
--EtID referenziert EtID aus Ersatzteil
ALTER TABLE Montage
ADD CONSTRAINT FK_EtID
FOREIGN KEY (EtID) REFERENCES Ersatzteil(EtID);
--AufNR referenziert AufNR aus Kund
ALTER TABLE Montage
ADD CONSTRAINT FK_AufNr
FOREIGN KEY (AufNr) REFERENCES Auftrag(AufNr);
-- Lösung:
--Die Befehle wurden erfolgreich ausgeführt.
-- Aufgabe: 1.5
-- ----------------------------------------------------------
SELECT *
FROM INFORMATION_SCHEMA.COLUMNS
WHERE table_name = 'Montage';
SELECT *
FROM INFORMATION_SCHEMA.COLUMNS
WHERE table_name = 'Auftrag';
-- Zeigt Datentypen und Constraints der entspr. Tabellen an
-- Aufgabe: 1.6
-- ----------------------------------------------------------
SELECT COUNT(*) FROM Auftrag;
SELECT COUNT(*) FROM Montage;
SELECT COUNT(*) FROM Kunde;
SELECT COUNT(*) FROM Mitarbeiter;
SELECT COUNT(*) FROM Ersatzteil;
-- Lösung:
-- Auftrag 956
-- Montage 820
-- Kunde 501
-- Mitarbeiter 12
-- Ersatzteil 24
-- Aufgabe: 2.1 a)
-- ----------------------------------------------------------
SELECT *
FROM Mitarbeiter
WHERE MitEinsatzort = 'Dresden';
-- Lösung: 4 Mitarbeiter
-- Aufgabe: 2.1 b)
-- ----------------------------------------------------------
SELECT *
FROM Kunde
WHERE KunOrt NOT LIKE 'Leipzig';
-- Lösung: Gibt alle Kunden aus, die nicht in Leipzig wohnen
-- Aufgabe 2.1 c)
-- ----------------------------------------------------------
SELECT *
FROM Ersatzteil
WHERE EtBezeichnung LIKE 'E%';
-- Lösung: 3 Ersatzteile
-- Aufgabe 2.1 d)
-- ----------------------------------------------------------
SELECT *
FROM Auftrag
WHERE Dauer > 3 OR Anfahrt > 80;
-- Lösung: 149 Aufträge
-- Aufgabe 2.1 e)
-- ----------------------------------------------------------
SELECT MitName, MitVorname, MitJobFROM Mitarbeiter
WHERE MitJob= 'Monteur' OR MitJob = 'Azubi'
ORDER BY MitName;
-- Lösung: 6 Mitarbeiter
-- Aufgabe 2.1 f)
-- ----------------------------------------------------------
SELECT *
FROM Auftrag
WHERE Dauer IS NULL;
-- Lösung: 385 nicht erledigte Aufträge
-- Aufgabe 2.1 g)
SELECT *, Anfahrt*2.5 AS Anfahrtskosten
FROM Auftrag
WHERE Dauer IS NOT NULL;
-- Lösung: 571 erledigte Aufträge
--Aufgabe 2.1 h)
-- ----------------------------------------------------------
SELECT EtPreis*EtAnzLager AS Warenwert
FROM Ersatzteil;
-- Lösung: Die Ausgabe beschreibt den gesammten Warenwert jeder verschiedenen
Ersatzteilart im Lager.
-- 24 Datensätze
-- Aufgabe 2.2 a)
-- ----------------------------------------------------------
SELECT MitName, MitVorname, MitGebDat, YEAR(GETDATE ( )) - YEAR(MitGebDat)
AS 'Alter Jahr' FROM Mitarbeiter;
-- Bemerkung: Alter in Jahren gibt an, wie alt der Mitarbeiter im Jahr der Abfrage wird, nicht sein oder ihr aktuelles Alter
-- Aufgabe 2.2 b)
-- ----------------------------------------------------------
SELECT AVG(DATEDIFF(DAY,AufDat,ErlDat)) AS Frist , MONTH(AufDat) AS Monat
FROM Auftrag
WHERE ErlDat IS NOT NULL
GROUP BY MONTH(AufDat)
ORDER BY MONTH(AufDat);
-- Lösung: Monat: 2,3,4,5,6,7 Frist jeweils 2 Tage

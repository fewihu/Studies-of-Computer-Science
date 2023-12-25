-- Vorbereitung
INSERT INTO Mitarbeiter
SELECT * FROM trommelhelden..quelleMitarbeiter
WHERE MitID NOT IN (SELECT MitID FROM Mitarbeiter);
INSERT INTO Kunde
SELECT * FROM trommelhelden..quelleKunde
WHERE KunNr NOT IN (SELECT KunNr FROM Kunde);
INSERT INTO Ersatzteil
SELECT * FROM trommelhelden..quelleErsatzteil
WHERE EtID NOT IN (SELECT EtID FROM Ersatzteil);
INSERT INTO Auftrag
SELECT * FROM trommelhelden..quelleAuftrag
WHERE Aufnr NOT IN (SELECT Aufnr FROM Auftrag);
INSERT INTO Montage
SELECT * FROM trommelhelden..quelleMontage
WHERE EtID NOT IN (SELECT EtID FROM Montage) AND AufNr NOT IN (SELECT AufNr FROM Montage);
-- Aufgabe: 1.2
-- ----------------------------------------------------------
--a
SELECT SUBSTRING(KunPLZ, 0,3) AS Anfang, SUBSTRING(KunPLZ,3,6) AS Ende FROM
(SELECT KunPLZ FROM Kunde GROUP BY KunPLZ) k;
-- Anzahl Datensätze: 226
--b
SELECT * INTO Niederlassung FROM trommelhelden..quelleNiederlassung;
-- 3 Datensätze
SELECT * INTO Gebiet FROM trommelhelden..quelleGebiet;
-- 8 Datensätze
SELECT * FROM Niederlassung;
SELECT * FROM Gebiet;
--PK Niederlassung
ALTER TABLE Niederlassung ADD CONSTRAINT PK_NLNr PRIMARY KEY (NLNr);
--PK Gebiet
ALTER TABLE Gebiet ADD CONSTRAINT PK_GID PRIMARY KEY (GebietID);
--FK Gebiet --> Niederlassung
ALTER TABLE Gebiet ADD CONSTRAINT FK_NLNr FOREIGN KEY (NLNr) REFERENCES Niederlassung(NLNr);
--c
ALTER TABLE Mitarbeiter ADD MitNLNr int;
--d
UPDATE Mitarbeiter
SET Mitarbeiter.MitNLNr = (SELECT NLNr FROM Niederlassung
WHERE Niederlassung.Ort = Mitarbeiter.MitEinsatzort);
-- e
ALTER TABLE Mitarbeiter DROP COLUMN MitEinsatzort;
--f
ALTER TABLE Mitarbeiter ADD CONSTRAINT FK_MitNLNr
FOREIGN KEY (MitNLNr) REFERENCES Niederlassung(NLNr);
-- Aufgabe: 1.3
-- ----------------------------------------------------------
SELECT * FROM Auftrag A
WHERE A.MitID IN (SELECT MitID FROM Mitarbeiter WHERE MitJob = 'Monteur');
-- alle Aufträge, die duch Monteuere bearbeiten werden/wurden
-- jedem Auftrag ist ein Mitarbeiter zugewiesen
SELECT * FROM Auftrag A
WHERE A.AufNr NOT IN (SELECT AufNr FROM Montage);-- zu jeder Montage gehört ein entsprechender Auftrag, aber
-- es gibt auch Aufträge ohne Montage von Ersatzteilen
SELECT A.AufNr, A.Dauer * M.MitStundensatz AS Lohnkosten
FROM Auftrag A
JOIN Mitarbeiter AS M ON M.MitID=A.MitID
WHERE A.Aufnr NOT IN(
SELECT Aufnr
FROM Montage
);
-- Lohnkosten aller Aufträge ohne Montage
-- Aufgabe: 1.4
-- ----------------------------------------------------------
INSERT INTO Auftrag
SELECT * FROM trommelhelden..quelledbs2Auftrag;
-- 581 Datensätze
INSERT INTO Montage
SELECT * FROM trommelhelden..quelledbs2Montage;
-- 354 Datensätze
-- Aufgabe: 1.5
-- ----------------------------------------------------------
SELECT * FROM Auftrag
WHERE DATEPART(year, AufDat) = DATEPART(year, GETDATE()) AND
DATEPART(month, AufDat) = DATEPART (month, GETDATE()) AND
MitID IS NULL;
-- neue Aufträge
SELECT * FROM Auftrag
WHERE DATEPART(year, AufDat) = DATEPART(year, GETDATE()) AND
DATEPART(month, AufDat) = DATEPART (month, GETDATE()) AND
MitID IS NOT NULL AND Dauer IS NULL;
-- geplante Aufträge
--
neu geplant erfüllt
--AufNr x
--MitID
x
--KunNr x
--AufDat x
--ErlDat
x
--Dauer
x
--Anfahrt
x
--Beschreibung
x
-- Aufgabe: 1.6
-- ----------------------------------------------------------
CREATE TABLE Rechnung(
KunNr INT NOT NULL,
AufNr INT NOT NULL,
RechDat DATE NOT NULL,
RechBetrag SMALLMONEY NOT NULL,
CONSTRAINT PK_Rechnung PRIMARY KEY (KunNr, AufNr),
CONSTRAINT FK_RKunNr FOREIGN KEY (KunNr) REFERENCES Kunde (KunNr),
CONSTRAINT FK_RAufNr FOREIGN KEY (AufNr) REFERENCES Auftrag (AufNr)
);
-- Aufgabe: 1.7
-- ----------------------------------------------------------
CREATE VIEW [neue Auträge] AS
SELECT AufNr, A.KunNr, M.MitID FROM Auftrag A
JOIN Kunde K ON A.KunNr = K.KunNr
JOIN Gebiet G ON SUBSTRING(K.KunPLZ,0,3) = G.GebietID
JOIN Mitarbeiter M ON M.MitNLNr = G.NLNr
WHERE A.MitID IS NULL;
-- 1275 Datensätze-- Aufgabe: 1.7
-- ----------------------------------------------------------
CREATE PROCEDURE Wochenplan
@MitID CHAR(3),
@Woche INT
AS
DECLARE @var INT
DECLARE ausfuehrung CURSOR FOR
SELECT Aufnr FROM Auftrag A
WHERE DATEPART(week,A.ErlDat) = @Woche
AND A.MitID = @MitID AND Dauer IS NULL;
OPEN ausfuehrung
FETCH ausfuehrung INTO @var
WHILE(@@FETCH_STATUS = 0)
BEGIN
SELECT ErlDat, KunName, KunOrt, KunPLZ, KunStrasse
FROM Auftrag A
JOIN Kunde K ON A.KunNr = K.KunNr
WHERE Aufnr = @var
FETCH ausfuehrung INTO @var
END
CLOSE ausfuehrung
DEALLOCATE ausfuehrung
EXECUTE Wochenplan @MitID = 103, @Woche = 43

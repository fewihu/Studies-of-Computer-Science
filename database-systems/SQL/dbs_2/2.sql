-- Aufgabe: 3.1
-- ----------------------------------------------------------
CREATE FUNCTION Anfahrtspreis (@Weg int) RETURNS INT AS
BEGIN
IF @Weg IS NULL
RETURN 0
DECLARE @anPreis decimal(5,2)
DECLARE @kmPreis decimal(5,2)
SELECT @kmPreis = 2.5
SELECT @anPreis = @kmPreis * @Weg
IF @anPreis < 30
SELECT @anPreis = 30
RETURN @anPreis
END
-- Die Befehle wurden erfolgreich ausgeführt.
SELECT Anfahrt, dbo.Anfahrtspreis(Anfahrt) AS Anfahrtspreis FROM Auftrag
WHERE Aufnr = 1000;
-- Anfahrt: 40 Anfahrtspreis: 100
SELECT Anfahrt, dbo.Anfahrtspreis(Anfahrt) AS Anfahrtspreis FROM Auftrag
WHERE Aufnr = 1000;
-- Anfahrt: NULL Anfahrtspreis: 0
-- Aufgabe: 3.2
-- ----------------------------------------------------------
CREATE FUNCTION Mindestbestand(@min int) RETURNS @table TABLE(
EtID char(5),
EtBezeichnung varchar(100),
EtPreis smallmoney,
EtAnzahl int,
EtHersteller varchar(30)
)
AS
BEGIN
INSERT INTO @table
SELECT * FROM Ersatzteil WHERE EtAnzLager < @min
RETURN
END
-- Die Befehle wurden erfolgreich ausgeführt.
DECLARE @testMin int
SET @testMin = 5
SELECT * FROM Mindestbestand(@testMin)
-- 5 Zeilen betroffen
-- Aufgabe: 4.1
-- ----------------------------------------------------------
-- a)
ALTER TABLE Mitarbeiter ADD CONSTRAINT DF_Mit_Job
DEFAULT 'Monteur' FOR MitJob
-- Die Befehle wurden erfolgreich ausgeführt.
-- Test:
INSERT INTO Mitarbeiter(MitID, MitName, MitVorname, MitGebDat)
VALUES(113, 'Müller', 'Felix', '1997-08-19')
-- neuen Mitarbeiter ohne Angabe von MitJob einfügen
SELECT MitJob FROM Mitarbeiter WHERE MitID = 113
-- MitJob des neuen Mitarbeiters ist Monteur-- Aufgabe: 4.1
-- ----------------------------------------------------------
-- b)
ALTER TABLE Auftrag ADD CONSTRAINT DF_Auftrag_AufDat
DEFAULT GetDate() FOR AufDat
-- Test:
INSERT INTO Auftrag (Aufnr, KunNr)
VALUES (3581, 1501)
-- AufDat nicht angegeben
DELETE Auftrag WHERE Aufnr = 3581
SELECT AufDat FROM Auftrag WHERE Aufnr = 3581
-- 2020-11-14
-- Aufgabe: 4.2
-- ----------------------------------------------------------
-- a)
ALTER TABLE Auftrag ADD CONSTRAINT CK_ErlDat
CHECK (Datediff(day,AufDat,ErlDat) >= 0 )
-- Test:
INSERT INTO Auftrag(AufNr, KunNr, AufDat, ErlDat)
VALUES(3582, 1501, GetDate(),'1999-01-01')
-- Die INSERT-Anweisung steht in Konflikt mit der CHECK -Einschränkung"CK_ErlDat"
INSERT INTO Auftrag(AufNr, KunNr, AufDat, ErlDat)
VALUES(3582, 1501, GetDate(), GetDate()+1)
-- (1 Zeile betroffen)
-- Aufgabe: 4.2
-- ----------------------------------------------------------
-- b)
ALTER TABLE Mitarbeiter ADD CONSTRAINT CK_MitID
CHECK(MitID > 99)
-- Test:
INSERT INTO Mitarbeiter (MitID, MitName, MitGebDat)
VALUES (11, 'Oberotto', GetDate())
-- MitID hat 2 Stellen
-- Die INSERT-Anweisung steht in Konflikt mit der CHECK -Einschränkung "CK_MitID"
INSERT INTO Mitarbeiter (MitID, MitName, MitGebDat)
VALUES (122, 'Oberotto', GetDate())
-- MitID hat 3 Stellen
-- (1 Zeile betroffen)
-- Aufgabe: 4.3
-- ----------------------------------------------------------
-- a)
ALTER TABLE Montage DROP CONSTRAINT FK_AufNr
-- Die Befehle wurden erfolgreich ausgeführt.
-- Anzeige der Datensätze:
SELECT AufDat, M.AufNr FROM Montage M
JOIN Auftrag A ON A.Aufnr = M.AufNr
WHERE Datepart(month, AufDat) = 5 AND Datepart(day,AufDat) = 1
-- -----------------------------------------------------------
SELECT AufDat, Aufnr FROM Auftrag
WHERE Datepart(month, AufDat) = 5 AND Datepart(day,AufDat) = 1
-- für beide: (1 Zeile betroffen)
-- neue FK-Beziehung mit Löschweitergabe:ALTER TABLE Montage ADD FOREIGN KEY(AufNr)
REFERENCES Auftrag ON DELETE CASCADE
-- Die Befehle wurden erfolgreich ausgeführt.
-- Löschen:
DELETE Auftrag WHERE Datepart(month, AufDat) = 5 AND Datepart(day,AufDat) = 1
-- (1 Zeile betroffen)
-- Anzeigen liefert für beide Statements: (0 Zeilen betroffen)
-- b)
-- Die FK-Beziehung mit Löschweitergabe ist für die Firma Trommelhelden nicht sinnvoll,
-- da die Daten der Montage(n) mglw länger benötigt werden als die Daten der Aufträge
-- (zB. um Montagekosten oder Ersatzteileinsatz zu bewerten), denkbar wäre eher eine
-- FK-Beziehung mit restrict oder set null

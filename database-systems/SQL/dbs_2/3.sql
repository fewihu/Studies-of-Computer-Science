-- Aufgabe: 5.1
-- ----------------------------------------------------------
CREATE TRIGGER Mit_JobStundensatz ON Mitarbeiter FOR INSERT, UPDATE AS
IF UPDATE (MitJob)
BEGIN
DECLARE @numSatz INT
DECLARE @numMit INT
SELECT @numSatz = (SELECT COUNT(MitID) FROM inserted WHERE
MitStundensatz IS NOT NULL)
SELECT @numMit = (SELECT COUNT(MitID) FROM inserted WHERE MitJob IN ('Monteur','Meister'))
IF @numSatz < @numMit
BEGIN
ROLLBACK
PRINT 'Es wurde nicht für alle Datensätze mit dem Job Monteur oder Meister ein Stundensatz angegeben'
END
ELSE
PRINT 'Änderung OK'
END
-- Die Befehle wurden erfolgreich ausgeführt.
-- a)
INSERT INTO Mitarbeiter (MitID,MitName,MitGebDat,MitJob)
VALUES(123,'Unterotto',GETDATE(),'Azubi')
-- Änderung OK (1 Zeile betroffen)
SELECT * FROM Mitarbeiter WHERE MitID = 123
-- MitID MitName
MitVorname MitGebDat MitJob MitStundensatz
-- 123
Unterotto NULL
2020-11-15 Azubi NULL
-- b)
BEGIN TRANSACTION
UPDATE Mitarbeiter SET MitJob = 'Monteur' WHERE MitID = 123
COMMIT TRANSACTION
-- schlägt mit Fehlermeldung aus Trigger fehl (SELECT wie oben):
-- Es wurde nicht für alle Datensätze mit dem Job Monteur oder Meister ein Stundensatz angegeben
-- Die Transaktion endete mit dem Trigger. Der Batch wurde abgebrochen.
-- b)
BEGIN TRANSACTION
UPDATE Mitarbeiter SET MitJob='Monteur', MitStundensatz=80 WHERE MitID = 123
COMMIT TRANSACTION
-- Erfolgsmeldung aus Trigger (1 Zeile betroffen)
SELECT * FROM Mitarbeiter WHERE MitID = 122
-- MitID MitName
MitVorname MitGebDat MitJob MitStundensatz
-- 123
Unterotto NULL
2020-11-15 Monteur 80,00
-- Aufgabe: 5.2
-- ----------------------------------------------------------
-- a)
CREATE TRIGGER Rechnung_erledigt ON Rechnung FOR INSERT AS
BEGIN
DECLARE @numAuf int -- Anzahl Aufträge die erledigt sind
DECLARE @numIn int -- Anzahl neuer Rechnungen
DECLARE @anfahrt int-- Anzahl Rechnung mit Betrag < Anfahrt
SELECT @numAuf = (SELECT COUNT(A.AufNr) FROM Auftrag A
JOIN inserted ON A.Aufnr = inserted.AufNr
WHERE A.ErlDat IS NOT NULL
AND A.DauerIS NOT NULL
AND A.Anfahrt IS NOT NULL)
SELECT @numIn = (SELECT COUNT(AufNr) FROM inserted)SELECT @anfahrt = (SELECT COUNT(A.Aufnr)
FROM Auftrag A JOIN inserted ON A.Aufnr = inserted.AufNr
WHERE inserted.RechBetrag < dbo.Anfahrtspreis(A.Anfahrt))
IF @numIn < @numAuf
BEGIN
ROLLBACK
PRINT 'Nicht erledigte Aufträge können nicht gebucht werden'
END
ELSE
BEGIN
IF @anfahrt <= 0
PRINT 'Änderung OK'
ELSE
BEGIN
ROLLBACK
PRINT 'Rechnungsbetrag deckt Anfahrt nicht'
END
END
END
-- Die Befehle wurden erfolgreich ausgeführt.
INSERT INTO Rechnung (KunNr, AufNr, RechDat, RechBetrag)
VALUES (1154,1264,'2020-10-15',154.5)
-- Möglich
INSERT INTO Rechnung (KunNr, AufNr, RechDat, RechBetrag)
VALUES (1151,1345,'2020-10-15',180)
-- Möglich
INSERT INTO Rechnung (KunNr, AufNr, RechDat, RechBetrag)
VALUES (1151,1263,'2020-10-15',50)
-- nicht möglich, da Anfahrtskosten nicht gedeckt
-- b)
INSERT INTO Rechnung SELECT KunNr, AufNr, Erldat, 80
FROM Auftrag WHERE AufNr IN (1008,1009,1010)
-- Trigger verhindert Einfügen, weil Rechnungsbetrag die Anfahrtskosten nicht deckt
-- Anfahrt jeweils 40km --> 100 Euro
INSERT INTO Rechnung SELECT KunNr, AufNr, Erldat, 175
FROM Auftrag WHERE Aufnr IN (1005,1006,1007)
-- Trigger lässt Einfügen zu
-- Anfahrt jeweils 40km --> 100 Euro und ErlDat, Dauer ist NOT NULL
-- c)
-- Das Rechnungsdatum sollte >= dem Erledigungsdatum eines Auftrags sein
-- Die Aufgabenstellung weiter gedacht: Der Rechnungsbetrag sollte <=
-- Anfahrtskosten + Ersatzteilpreis + Stundensatz * Dauer sein
-- Die Arbeitszeit eines Monteurs sollte nicht (wesentlich) größer als 40 Stunden pro Woche sein
-- Aufgabe: 5.3
-- ----------------------------------------------------------
-- a)
ALTER TABLE Auftrag ALTER COLUMN Beschreibung varchar(200)
-- Die Befehle wurden erfolgreich ausgeführt.
-- b)
-- Auftrag, Montage
-- Zeitstempel und Datenbankbenutzer sollten protokolliert werden,
-- damit Änderungen nachvollzogen werden können; die Art der Änderungs-
-- aktion ist entscheindend, um im Fehlerfall Änderungen rückgängig zu
-- machen (insert muss gelöscht werden, delete muss wieder eingefügt werden)
-- c)
DROP TRIGGER Auftrag_delete
CREATE TRIGGER Auftrag_delete ON Auftrag FOR DELETE ASBEGIN
SELECT * FROM deleted JOIN Montage on Montage.AufNr = deleted.Aufnr
ROLLBACK TRANSACTION
END
BEGIN TRANSACTION
DELETE Auftrag WHERE AufDat = '2020-10-05'
-- 0 Zeilen ausgegeben
SELECT * FROM Auftrag WHERE AufDat = '2020-10-05'
-- 5 Zeilen
-- Beobachtung: Im Trigger sind die zugehörigen Montage -Daten nicht verfügbar --> durch JOIN 0 Tupel
-- Begründung: Die FK-Beziehung mit ON DELETE CASCADE greift vor dem Trigger
-- Bemerkung: FK-Beziehung zu Rechnung ohne ON DELETE CASCADE
-- d)
-- Alternative: Für Montage wird ein eigener ON DELETE Trigger angelegt, der nach dem CASCADE der
-- FK-Beziehung die Daten protokolliert
-- Für Rechnung gibt es keinen Konflikt, da es keine Datensätze in Rechnung gibt;
-- es müsste auch ON DELETE CASCADE gesetzt werden, mit eigenem DELETE Trigger (siehe c)
-- e)
-- Protokolltabellen erzeugen:
-- Auftrag:
CREATE TABLE AuftragProtokoll
(Aufnr int, MitID char(3), KunNr int, AufDat date, ErlDate date,
Dauer decimal(5,1), Anfahrt int, Beschreibung varchar(200) ,
Änderungsdatum date, Änderung varchar(6), Benutzer varchar(20))
CREATE TRIGGER LoeschProtokollAuftrag ON Auftrag FOR DELETE AS
BEGIN
INSERT INTO AuftragProtokoll
SELECT *, GETDATE(), 'DELETE', CURRENT_USER FROM deleted;
END
-- Montage:
CREATE TABLE MontageProtokoll
(EtID char(5), Aufnr int, Anzahl int,
Änderungsdatum date, Änderung varchar(6), Benutzer varchar(20))
CREATE TRIGGER LoeschProtokollMontage ON Montage FOR DELETE AS
BEGIN
INSERT INTO MontageProtokoll
SELECT *, GETDATE(), 'DELETE', CURRENT_USER FROM deleted;
END
-- f)
BEGIN TRANSACTION
DELETE Auftrag WHERE DATEPART(month, ErlDat) = 5
SELECT * FROM MontageProtokoll
SELECT * FROM AuftragProtokoll
ROLLBACK TRANSACTION

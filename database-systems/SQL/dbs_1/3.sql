-- Aufgabe: 3.1
-- ----------------------------------------------------------
-- parametrisiert
CREATE PROCEDURE Mitarbeiter_Kunde
@MitNr CHAR(3), @KundenNr INT, @Zaehler INT OUTPUT
AS
SELECT @Zaehler = COUNT(KunNr)
FROM Auftrag A
WHERE A.KunNr = @KundenNr AND A.MitID = @MitNr
DECLARE @Anzahl INT
EXECUTE Mitarbeiter_Kunde @MitNr=105, @KundenNr=1003, @Zaehler = @Anzahl
OUTPUT PRINT CONVERT(CHAR(3), @Anzahl);
-- unparametrisiert
CREATE PROCEDURE Mitarbeiter_Kunde2
AS
SELECT COUNT(KunNr) AS Aufträge , M.MitID, A.KunNr
FROM Auftrag A
Join Mitarbeiter M ON A.MitID = M.MitID
GROUP BY M.MitID, A.KunNr
-- Aufgabe: 3.2
-- ----------------------------------------------------------
CREATE PROCEDURE Ersatzteile
@Anzahl INT
AS
SELECT *
FROM(SELECT SUM(Anzahl) AS Anzahl, EtID FROM Montage GROUP BY EtID) AS A
WHERE A.Anzahl > @Anzahl;
EXECUTE Ersatzteile @Anzahl = 32
-- Aufgabe: 3.3
-- ----------------------------------------------------------
CREATE PROCEDURE autofill
@Anzahl INT
AS
DECLARE @date date = (SELECT MIN(AufDat) FROM Auftrag WHERE Dauer IS NULL);
DECLARE @i INT = @Anzahl;
DECLARE @acc_anzahl DATE;
WHILE @date <= GETDATE() AND @i > 0 AND @Anzahl > 0
BEGIN
PRINT 'Resttage: ' + CAST(@i AS CHAR(3))
SET @i = @i -1;
SELECT COUNT(Aufnr) AS Anzahl
FROM Auftrag
WHERE AufDat = @date AND MitID IS NOT NULL AND Dauer IS NULL;
SET @date = DATEADD(day, 1, @date);
PRINT 'Bearbeite Aufträge vom: ' + convert(varchar(12),@date);
DECLARE @auftraege INT;
UPDATE Auftrag
SET Dauer = ((convert(int,rand()*9)+1)*0.5),
Anfahrt = (convert(int,rand()*95)+5),
Beschreibung = 'durch ****** bearbeitet'
WHERE Aufnr IN (
SELECT Aufnr
FROM Auftrag
WHERE AufDat = @date AND MitID IS NOT NULL AND Dauer IS NULL
);
END
EXECUTE autofill @Anzahl = 50-- Aufgabe: 3.4
-- ----------------------------------------------------------
CREATE PROCEDURE autofill2
@Anzahl INT
AS
DECLARE @date date = (SELECT MIN(AufDat) FROM Auftrag WHERE Dauer IS NULL);
DECLARE @i INT = @Anzahl;
DECLARE @acc_anzahl DATE;
DECLARE @var INT;
WHILE @date <= GETDATE() AND @i > 0 AND @Anzahl > 0
BEGIN
PRINT 'Resttage: ' + CAST(@i AS CHAR(3))
SET @i = @i -1;
SELECT COUNT(Aufnr) AS Anzahl
FROM Auftrag
WHERE AufDat = @date AND MitID IS NOT NULL AND Dauer IS NULL;
SET @date = DATEADD(day, 1, @date);
PRINT 'Bearbeite Aufträge vom: ' + convert(varchar(12),@date);
DECLARE @auftraege INT;
UPDATE Auftrag
SET Dauer = ((convert(int,rand()*9)+1)*0.5),
Anfahrt = (convert(int,rand()*95)+5),
Beschreibung = 'durch ***** bearbeitet'
DECLARE ausfuehrung CURSOR FOR
SELECT Aufnr
ROM Auftrag
WHERE AufDat = @date AND MitID IS NOT NULL AND Dauer IS NULL
OPEN ausfuehrung
FETCH ausführung INTO @var
WHILE(@@FETCH_STATUS = 0)
BEGIN
UPDATE Auftrag
SET Dauer = ((convert(int,rand()*9)+1)*0.5),
Anfahrt = (convert(int,rand()*95)+5),
Beschreibung = 'durch ***** bearbeitet'
WHERE Aufnr = @var
FETCH ausfuehrung INTO @var
END
CLOSE ausfuehrung
DEALLOCATE ausfuehrung
END
EXECUTE autofill2 @Anzahl = 50

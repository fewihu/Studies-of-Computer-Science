-- Aufgabe: 2.6 a)
-- ----------------------------------------------------------
SELECT * FROM Mitarbeiter
WHERE MitID NOT IN
(SELECT MitID FROM Auftrag
WHERE DATEPART(mm, AufDat) = DATEPART(mm, GETDATE())
AND MitID IS NOT NULL
GROUP BY MitID);
-- Aufgabe: 2.6 b)
-- ----------------------------------------------------------
SELECT A.Aufnr, A.Anfahrt * 2.5 AS Anfahrtskosten, A.Dauer * M.MitStundensatz AS Lohnkosten
FROM Auftrag A
JOIN Mitarbeiter AS M ON M.MitID=A.MitID
WHERE A.Aufnr NOT IN
(SELECT Aufnr
FROM Montage);
-- Aufgabe: 2.6 c)
-- ----------------------------------------------------------
SELECT * FROM Auftrag
WHERE ErlDat =
(SELECT MAX(ErlDat)
FROM Auftrag);
-- Aufgabe: 2.6 d)
-- ----------------------------------------------------------
-- mit unkorellierter Unterabfrage:
SELECT * FROM
(SELECT MAX(Dauer) AS MDauer, Auftrag.MitID FROM Auftrag
WHERE Dauer IS NOT NULL
GROUP BY Auftrag.MitID) UA
JOIN Auftrag A
ON UA.MDauer=A.Dauer AND UA.MitId=A.MitId;
-- mit korellierter Unterabfrage:
SELECT * FROM Auftrag A
WHERE A.Dauer =
(SELECT MAX(Dauer) AS MDauer
FROM Auftrag B WHERE A.MitID = B.MitID);
-- Aufgabe: 2.7 a)
-- ----------------------------------------------------------
SELECT * FROM
(SELECT A.MitId, SUM(Anfahrt) AS Monatskilometer
FROM Auftrag A
WHERE DATEPART(mm, A.ErlDat) = 4
GROUP BY A.MitID
HAVING SUM(Anfahrt) > 1500) UA
JOIN Mitarbeiter M ON M.MitId=UA.MitId;
-- Aufgabe: 2.7 b)
-- ----------------------------------------------------------
SELECT SUM(M.Anzahl) AS AnzVerbaut, M.EtID
FROM Montage M
GROUP BY M.EtID
HAVING SUM(M.Anzahl) <
(SELECT E.EtAnzLager
FROM Ersatzteil E
WHERE E.EtID=M.EtID);
-- Aufgabe: 2.8 a)
-- ----------------------------------------------------------
INSERT INTO Kunde
VALUES(1501,'Kunze','Dresden','01189','Bachgasse 25');
-- (1 Zeile betroffen)
INSERT INTO Auftrag
VALUES(2400,NULL,1501,'26.06.2020',NULL,NULL,NULL,NULL);-- (1 Zeile betroffen)
-- Aufgabe: 2.8 b)
-- ----------------------------------------------------------
UPDATE Mitarbeiter SET MitName = 'Nass' WHERE MitName='Igel';
-- Bemerkung: SELECT * FROM Mitarbeiter WHERE Mitarbeiter.MitName='Nass';
-- -> zwei Mitarbeiterinnen Anna Nass, allerdings kein Konflikt,
-- da der Primary Key (MitId) verschieden ist
-- Aufgabe: 2.8 c)
-- ----------------------------------------------------------
-- geringster Studensatz eines Monteurs (75.00):
-- SELECT MIN(MitStundensatz) FROM Mitarbeiter WHERE MitJob = 'Monteur';
-- Auswahl aller Azubis(2):
-- SELECT * FROM Mitarbeiter WHERE MitJob = 'Azubi';
UPDATE Mitarbeiter
SET MitStundensatz =
(SELECT MIN(MitStundensatz) FROM Mitarbeiter
WHERE MitJob = 'Monteur')
WHERE MitJob = 'Azubi';
-- 2 Zeilen betroffen
-- Aufgabe: 2.8 d)
-- ----------------------------------------------------------
-- SELECT * FROM Ersatzteil WHERE EtHersteller = 'Hauknecht'; -> 4 Ergebnisse
DELETE Ersatzteil WHERE EtHersteller = 'Hauknecht';
-- Konflikt mit Tabelle Montage durch FK!
DELETE Montage WHERE EtID IN (SELECT EtID FROM Ersatzteil WHERE EtHersteller = 'Hauknecht');
-- (134 Zeilen betroffen)
DELETE Ersatzteil WHERE EtHersteller = 'Hauknecht';
-- (4 Zeilen betroffen)
-- Aufgabe: 2.8 d)
-- ----------------------------------------------------------
-- Montagen zu im März erledigten Aufträgen löschen
DELETE Montage
WHERE Aufnr IN
(SELECT Aufnr FROM Auftrag WHERE DATEPART(mm, ErlDat) = 3);
-- 222 Zeilen betroffen
-- Tabellenvariable für Auftragsnummern
DECLARE @myKunNr TABLE (KunNr int);
-- mit entsprechenden Auftragsnummer initialisieren
INSERT INTO @myKunNr
SELECT KunNr FROM Auftrag
WHERE DATEPART(mm, ErlDat) = 3
GROUP BY KunNr;
--165 Zeilen betroffen)
-- im März erledigte Aufträge löschen
DELETE Auftrag
WHERE Aufnr IN
(SELECT Aufnr FROM Auftrag WHERE DATEPART(mm, ErlDat) = 3);
-- 193 Zeilen betroffen
-- Kunden zu im März erledigten Aufträgen löschen
DELETE Kunde
WHERE KunNr IN (SELECT KunNr FROM @myKunNr) AND KunNr NOT IN
(SELECT KunNr FROM Auftrag GROUP BY KunNr);
--34 Zeilen betroffen
-- Aufgabe: 2.9 a)
-- ----------------------------------------------------------SELECT MitEinsatzort, KunOrt FROM Mitarbeiter CROSS JOIN Kunde;
-- Bedeutung: Das kartesische Produkt ist die einfachste JOIN-Art
-- und liefert die Grundmenge für alle anderen JOINs
-- Aufgabe: 2.9 b)
-- ----------------------------------------------------------
SELECT * FROM Mitarbeiter M JOIN Kunde K ON M.MitEinsatzort=K.KunOrt;U:\SQLQuery2.sql
-- Aufgabe: 2.9 c)
-- ----------------------------------------------------------
SELECT * FROM Mitarbeiter M, Kunde K WHERE K.KunOrt=M.MitEinsatzort;
-- Aufgabe: 2.10
-- ----------------------------------------------------------
-- Differenz:
SELECT KunOrt FROM Kunde
EXCEPT
SELECT MitEinsatzort FROM Mitarbeiter;
-- bzw
SELECT MitEinsatzort FROM Mitarbeiter
EXCEPT
SELECT KunOrt FROM Kunde;
-- Durchschnitt:
SELECT KunOrt FROM Kunde
INTERSECT
SELECT MitEinsatzort FROM Mitarbeiter;
-- Vereinigung:
SELECT KunOrt FROM Kunde
UNION
SELECT MitEinsatzort FROM Mitarbeiter;
-- Aufgabe: 2.11
-- ----------------------------------------------------------
CREATE VIEW Auftragswert AS
SELECT A.Aufnr, A.ErlDat, K.KunOrt, M.Anzahl*E.EtPreis AS Materialwert,
A.Anfahrt*2.5 AS Anfahrtskosten, A.Dauer*Mi.MitStundensatz AS Lohnkosten
FROM Kunde K
JOIN Auftrag A ON K.KunNr=A.KunNr
JOIN Montage M ON A.Aufnr=M.AufNr
JOIN Ersatzteil E ON M.EtID=E.EtID
JOIN Mitarbeiter Mi ON A.MitID=Mi.MitID
WHERE A.ErlDat IS NOT NULL AND K.KunOrt IS NOT NULL AND A.Dauer IS NOT NULL
AND A.Anfahrt IS NOT NULL AND Mi.MitStundensatz IS NOT NULL;
SELECT Aufnr, Materialwert + Anfahrtskosten + Lohnkosten AS Gesamtwert
FROM Auftragswert;
-- Aufgabe: 2.11
-- ----------------------------------------------------------
SELECT Aufnr,
CASE WHEN Anfahrt*2.5 < 30 THEN 30
ELSE Anfahrt*2.5
END AS Anf
FROM Auftrag
WHERE Anfahrt IS NOT NULL;

-- Aufgabe 2.2 c)
-- ----------------------------------------------------------
SELECT AufNr, ISNULL(Dauer, 0) AS Dauer
FROM Auftrag;
-- Lösung: 956 Datensätze
-- Aufgabe 2.3 a)
-- ----------------------------------------------------------
SELECT COUNT(KunNr) FROM Kunde;
-- Lösung: 501
-- Aufgabe 2.3 b)
-- ----------------------------------------------------------
SELECT KunOrt, COUNT(KunNr) FROM Kunde
GROUP BY KunOrt;
-- Lösung: 283 Datensätze
-- Aufgabe 2.3 c)
-- ----------------------------------------------------------
SELECT AVG(Dauer) AS AVGDauer, MitID FROM Auftrag
WHERE Dauer IS NOT NULL
GROUP BY MitID;
-- Lösung: 7 Mitarbeiter
-- Aufgabe 2.3 d)
-- ----------------------------------------------------------
SELECT AVG(Dauer) AS AVGDauer, MitID, ErlDat, COUNT(Aufnr) AS 'Aufträge pro Tag'
FROM Auftrag
WHERE Dauer IS NOT NULL
GROUP BY MitID, ErlDat;
-- Lösung: 381 Datensätze
-- Aufgabe 2.3 e)
-- ----------------------------------------------------------
SELECT MIN(ErlDat) FROM Auftrag
WHERE DATEDIFF(DAY, GETDATE(), ErlDat) > 0;
-- Lösung: 22.05.2020 (am 21.05.2020 abgefragt)
-- 2.4 a)
-- ----------------------------------------------------------
SELECT MitJob, AVG(DATEDIFF(YEAR, MitGebDat, GETDATE())) AS 'AVG Alter der Berufgruppe'
FROM Mitarbeiter
GROUP BY MitJob;
-- Lösung: 6 Datensätze
-- Semantik: Ermittelt durchschnittliches Alter der Berufsgruppen.
-- Aufgabe 2.4 b)
-- ----------------------------------------------------------
SELECT SUM(Dauer) AS Gesammtdauer, MitID, DATEPART(WEEKDAY,AufDat) AS Wochentag
FROM Auftrag
WHERE MitID IS NOT NULL
GROUP BY DATEPART(WEEKDAY,AufDat), MitID
ORDER BY MitID;
-- Lösung: 49 Datensätze
-- Aufgabe 2.5 a)
-- ----------------------------------------------------------
-- impliziter JOIN aus Aufgabenstellung
SELECT AufNR, EtBezeichnung, Anzahl, EtPreis, Anzahl*EtPreis AS Materialwert
FROM Montage m, Ersatzteil e
WHERE m.EtID=e.EtID
ORDER BY AufNr;
-- Semantik: Warenwert der verbauten Ersatzteile pro Montage
-- expliziter JOIN
SELECT AufNr, EtBezeichnung, Anzahl, EtPreis, Anzahl*EtPreis AS Materialwert
FROM Montage
JOIN Ersatzteil ON Montage.EtID=Ersatzteil.EtID
ORDER BY AufNR;-- Lösung: beide 820 Datensätze
-- Aufgabe 2.5b)
-- ----------------------------------------------------------
SELECT AufNr, Auftrag.MitID, Auftrag.Dauer, Mitarbeiter.MitStundensatz,
Mitarbeiter.MitStundensatz*Auftrag.Dauer AS Lohnkosten
FROM Auftrag
JOIN Mitarbeiter ON Auftrag.MitID=Mitarbeiter.MitID
WHERE Auftrag.Dauer IS NOT NULL
ORDER BY AufDat, Auftrag.MitID;
-- Lösung: 571 Datensätze
-- Aufgabe 2.5c)
-- ----------------------------------------------------------
SELECT Auftrag.Aufnr, Kunde.KunOrt, Auftrag.Anfahrt
FROM Kunde
JOIN Auftrag ON Kunde.KunNr=Auftrag.KunNr
WHERE Auftrag.Anfahrt > 50;
-- Lösung: 286 Datensätze
-- Aufgabe 2.5 d)
-- ----------------------------------------------------------
SELECT Kunde.KunNr, Kunde.KunName, Montage.EtID
FROM Kunde
JOIN Auftrag ON Auftrag.KunNr=Kunde.KunNr
JOIN Montage ON Montage.AufNr=Auftrag.Aufnr
WHERE Montage.EtID='P0110'
-- Lösung: 30 Datensätze
-- Aufgabe 2.5 e)
-- ----------------------------------------------------------
SELECT A.AufNr, M.MitStundensatz*A.Dauer AS Lohnkosten, A.Anfahrt*2.5 AS
Fahrtkosten, Mo.Anzahl*E.EtPreis AS Materialkosten
FROM Auftrag AS A
JOIN Mitarbeiter AS M ON A.MitID=M.MitID
JOIN Montage AS Mo ON Mo.AufNr=A.Aufnr
JOIN Ersatzteil AS E ON E.EtID=Mo.EtID;
-- Bemerkung: Lohn- und Fahrtkosten übersteigen die Materialkosten oft deutlich
-- die Reparatur lohnt sich selten
-- Aufgabe 2.5 f)
-- ----------------------------------------------------------
SELECT AufNr, ISNULL(Auftrag.MitID,'') AS MitID, ISNULL(Mitarbeiter.MitName,'')
AS Name
FROM Auftrag
LEFT JOIN Mitarbeiter ON Auftrag.MitID=Mitarbeiter.MitID
WHERE MONTH(Auftrag.AufDat) = MONTH(GETDATE());
-- Lösung: 208 Datensätze abgefragt am 21.05.2020
-- Aufgabe 2.5 g)
SELECT Montage.EtID, COUNT(Montage.Anzahl) AS Anzahl
FROM Auftrag
JOIN Montage ON Auftrag.Aufnr=Montage.AufNr
WHERE MONTH(Auftrag.AufDat) = MONTH(GETDATE()) -1
GROUP BY Montage.EtID;
-- Lösung: 24 Datensätze abgefragt am 21.05.2020

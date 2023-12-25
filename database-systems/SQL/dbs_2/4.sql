-- Aufgabe: 6.1
-- ----------------------------------------------------------
-- a) und b)
BEGIN TRANSACTION
DELETE Montage
SELECT * FROM Montage
ROLLBACK TRANSACTION
SELECT * FROM Montage
-- c)
-- Atomarität: Alle Bestandteile einer Transaktion ausgeführt, oder gar keiner
-- Konsistent: Vor - und Nachzustand einer Transaktion ist konsistenter Zustand
-- Isolation: Zwischenzustände von Transaktionen werden nicht von anderen Transktionen benutzt
-- Dauerhaftigkeit: Daten nach Commit dauerhaft gespeichert
-- 6.2
-- in Zusammenarbeit mit ***********
-- a)
USE ii18s79214
-- Der Serverprinzipal "smb\s79138" kann unter dem aktuellen Sicherheitskontext nicht
-- auf die ii18s79214-Datenbank zugreifen.
-- b) und c)
-- d)
USE ii18s79214
-- Die Befehle wurden erfolgreich ausgeführt.
-- e)
SELECT * FROM Auftrag
-- Die SELECT-Berechtigung wurde für das Auftrag-Objekt, ii18s79214-Datenbank, dbo-Schema, verweigert.
-- f)
-- g)
SELECT * FROM Auftrag
-- (1517 Zeilen betroffen) -> funktioniert
-- h)
-- i)
BEGIN TRANSACTION
SELECT * FROM Auftrag WITH (holdlock)
-- (1517 Zeilen betroffen)
-- j) und k)
-- l)
COMMIT TRANSACTION
-- Die Befehle wurden erfolgreich ausgeführt.
-- m)
-- n)
BEGIN TRANSACTION
DELETE Auftrag WHERE AufNr=5423
ROLLBACK TRANSACTION
-- Die DELETE-Berechtigung wurde für das Auftrag-Objekt, ii18s79214-Datenbank, dbo-Schema, verweigert.
-- o)
-- p)
BEGIN TRANSACTION
DELETE Auftrag WHERE AufNr=5423
COMMIT
-- (1 Zeile betroffen)-- q) und r)
-- s)
CREATE TABLE Auftrag (
Aufnr int,
MitID char(3),
KunNr int,
AufDat date,
ErlDat date,
Dauer decimal(5,1),
Anfahrt int,
Beschreibung varchar(200)
)
INSERT INTO Auftrag VALUES(
6000, 150,666,GETDATE(), GETDATE(), 2, 40,'Kartoffelsalat'
)
-- (1 Zeile betroffen) Die Befehle wurden erfolgreich ausgeführt.
-- t) und u)
-- v)
DROP TABLE Auftrag
-- Die Befehle wurden erfolgreich ausgeführt.

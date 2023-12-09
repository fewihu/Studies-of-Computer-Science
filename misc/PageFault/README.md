# Page-Fault-Vermessung

* implementiert für GNU Linux jeglicher Distribution
* dient der experimentellen Bestimmung der Dauer eines Major Page Faults
* rtdsc() muss als Assembler programmiert werden: CPUID; Ablegen des Rückgabewerts in %rax; ebx, ecx und edx bereinigen
* zu übersetzen mit *gcc pf.c -D _GNU_SOURCE -D LARGE_SEG=X -D FINE_SEG=Y*
	* _GNU_SOURCE damit Semaphorenoperationen mit Timeout funktionieren
	* X entspricht Anzahl Segmenten (in GiB) die eher zügig belegt werden
	* Y entspricht Anzahl Segmenten (in MiB) die sehr vorsichtig belegt werden
	* Es sollten von gesamten Hauptspeicher ca. 2 GiB für Linux und den zu vermessenden Speicherbereich übrigbleiben 
* Das Programm wird das Zielsystem in einen kritischen Zustand bringen, bedeutungsvolle Systemzustände sollten gesichert werden
* durch das Messen der Taktfrequenz kann aus den Zugriffszeiten in Taktzyklen die Zugriffszeit in Sekunden bestimmt werden
	* für den Einsatz auf Laptopplattformen ist es sinnvoll das Downscaling des Prozessortakts mit Hilfe von *cpufrequtils* zu unterbinden, damit die Messung der Taktfrequenz aussagekräftig ist

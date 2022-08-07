# VisualCrypt

Mit dem Programm "visualCrypt" lassen sich Bilder im .bmp Format verschlüsseln.

## Programm erzeugen

Im ./source Verzeichnis befindet sich ein Makefile, das per Aufruf:
> make  

das Programm "visualCrypt" erstellt. Da das Programm Linux-spezifische Bibliotheken  
benötigt, wird die Verwendung unter einem Linux-Betriebssystem (oder Subsystem) empfohlen.

## Programm ausführen

Das ausführbare Programm lässt sich anschließend im ./source Verzeichnis finden.  
Übergabeparameter sind hierbei optional.

### Anwendungsbeispiele:
> ./visualCrypt

Zur Ausführung aus dem source Verzeichnis.

> ./source/visualCrypt

Zur Ausführung aus dem VisualCrypt Verzeichnis.

#### Es sind folgende Übergabeparameter für das Programm definiert:

> ./source/visualCrypt -h

Liefert Informationen über die Verwendung.

>./source/visualCrypt -s &lt;path to image file&gt;

Mit -s ist es möglich, das zu verschlüsselnde Bild auszuwählen.  
Nach der Option muss der Pfad zu einem validen BMP-File übergeben werden.  
Ohne den Parameter wird das Bild &lt;path to visualCrypt&gt;/image/cameraman.bmp verwendet.

>./source/visualCrypt -d &lt;path to target directory&gt;

Mit -d lässt sich das Zielverzeichnis auswählen, in welchem die erzeugten Shares  
abgelegt oder (zur Entschlüsselung) gesucht werden. Der angegebene Ordner muss  
hierfür bereits existieren. Die Stapelergebnisse der Shares, nach einer Entschlüsselung,  
werden dort ebenfalls gespeichert.  
Ohne den Parameter wird das Verzeichnis &lt;path to visualCrypt&gt;/image verwendet.

### Programm-Menü:

Die Optionspunkte 1. bis 5. bieten verschiedene Algorithmen zur Verschlüsselung  
eines BMP-Files an. Nach der Auswahl eines Algorithmus, wird stets die Anzahl der  
Shares erfragt, die generiert werden soll.

Mit dem Optionspunkt 6. lassen sich bereits erzeugte Shares wieder entschlüsseln.  
Da die Namen der Shares "share01.bmp", "share02.bmp" usw. lauten, müssen  
anschließend die Nummern des ersten und letzten Shares angegeben werden,  
die Teil der Entschlüsselung sein sollen.

Optionspunkt 7. startet eine Zeitmessung über alle Algorithmen.  
Achtung: Die Standardeinstellung dauert womöglich mehrere Stunden!  
Das Ergebnis der Zeitmessung lautet standardmäßig "timeMeasurement.log",  
und wird im Hauptverzeichnis des Programms (visualCrypt Ordner) abgelegt.  

### Weitere Optionen:

In "settings.h" sind Optionen ausgelagert, die nur bedingt angepasst werden müssen.  
Hierzu zählt zum Beispiel der "Threshold", welcher Einfluss auf den Kontrast des  
Stapelergebnis nimmt. Ebenso ist es hier möglich, die Anzahl der Messwiederholungen,  
der Zeitmessung, zu editieren.

## Programm löschen

Wird im source-Verzeichnis der Befehl:
> make clean

ausgeführt, werden alle Kompilate gelöscht.
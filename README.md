# Schwarmintelligenz
Ein kleinen C++ Programm, welches Schwarm/Schwärme simuliert und dies graphisch darstellt.

Zuerst wird ein Schwarm modelliert, welcher einen Anführer hat der sich per zufall fortbewegt. Der schwarm folgt dem Anführer.
Folgend wird ein zweiter Schwarm erstellt, wobei beide Schwärme sich aus dem Weg gehen.

# Idee 
Wenn ein Agent in Kontakt mit einem anderem Agenten, welcher Teil eines größeren Schwarms ist, wird dieser Schrittweise Teil des größeren Schwarms, bis dieser vollkommen Teil des größeren Schwarms wird. 

# Idee (Deprecated)
Wenn sich einzelne Agenten vom Schwarm trennen, können diese von Agenten des anderen Schwarms gefangen genommen werden, wenn die Gegenerischen Agenten in der überzahl sind. Die gegnerischen Agenten bringen dann die "gefangenen" Agenten zum eigenen Schwarm und verleiben diese nach einer gewissen Zeit dem eigenen Schwarm ein.

- 2 schwärme (A,B)
- Basis Verhalten von Agenten:
	Agenten versuchen fremde Agenten zu meiden
	Agenten suchen Kameraden
	Schwärme teilen sich ab gewisser größer


- Angriffsverhalten
	Schwärme nähern sich gegenseitig an (suchen "dünne" Agentenschicht)
	Angriff auf schweif: wenn Agentenschicht schicht dünn genug -> durchstoß um agenten zu "fressen"
	5% boost für "bite"
	nach Angriff "Rückzug" (bzw. generelles vermeiden "dünner" Agentenschichten -> zusammenziehen)
	"Durchfressene" Agenten wechseln die Zugehörigkeit
	
- Defensiv verhalten
	Wenn gegn. Angriff bevorsteht: Ausweichmanöver (5% boost als Schwarmbonus)
	

- Gewinner ist letzter Schwarm
 
-> keine "wilden" Agenten mehr, Solo-Agenten suchen finden entweder Schwarm oder werden gefressen

## TO-DO
### UI (Kevin)
- [x] Windows & Direct2D
- [x] Funktionen zum zeichnen des Schwarmes
- [ ] Code Cleanup

### Agenten (Nils)
- [x] Schwarm erstellen, der Distanz, Nähe und Richtung befolgt
- [ ] Schwarm umstellen, einen Anführer zu folgen
- [x] Zweiter Schwarm implementieren
- [x] Idee umsetzen

### Agenten-Verhalten (Theo)
- [ ] Kriegsgefangenen Verhalten

# Zeitplan
Bearbeitungszeit: 01.03.2021 - 19.04.2021

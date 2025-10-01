# PWM / Counter / Aktoren

## Überblick
Diese Veranstaltung ist die abschließende Einheit des Semesters und behandelt Signalgenerierung mittels PWM (Pulsweitenmodulation) sowie die Ansteuerung von Aktoren, insbesondere Gleichstrommotoren. Es gibt keinen Test und keine Hausaufgabe - die Abnahme erfolgt direkt vor Ort.

## Lernziele
- Verständnis verschiedener Timer-Betriebsmodi (CTC, Fast PWM, Phase Correct PWM)
- Erzeugung von PWM-Signalen zur Motorsteuerung
- Praktische Anwendung der H-Brückenschaltung
- Drehzahl- und Drehrichtungssteuerung von Gleichstrommotoren

## Theoretische Grundlagen

### Timer-Betriebsmodi für Signalgenerierung
**1. CTC-Mode (Clear Timer on Compare Match)**
- Timer zählt bis zum Vergleichswert (OCR0)
- Bei Erreichen wird der Zähler zurückgesetzt
- Output-Pin kann bei jedem Match getoggelt werden
- Erzeugung von Rechtecksignalen mit variabler Frequenz

**2. Fast PWM Mode**
- Timer zählt kontinuierlich von 0 bis 255
- Output wird bei Erreichen von OCR0 gesetzt/gelöscht
- Konstante Frequenz, variable Pulsbreite
- Frequenzberechnung: f_PWM = f_IO / (N × 256)
- Gut geeignet für Motorsteuerung

**3. Phase Correct PWM Mode**
- Timer zählt vorwärts (0→255) und rückwärts (255→0)
- Symmetrischer Impulsverlauf
- Frequenzberechnung: f_PWM = f_IO / (N × 510)
- Geringere Frequenz als Fast PWM, aber gleichmäßigerer Impuls

### Motorsteuerung

**Drehzahlsteuerung**
- Elektrische Energie: E = ∫ u(t) · i(t) dt
- PWM-Methode: Variation der Einschaltdauer bei konstanter Spannung
- Pulsbreite bestimmt die mittlere Energiezufuhr
- OCR0-Wert steuert die Pulsbreite (0-255 für 8-Bit-Timer)

**Drehrichtungssteuerung**
- H-Brückenschaltung ermöglicht Umpolung des Motors
- 4 elektronische Schalter (Transistoren) in H-Anordnung
- Motor in der Brücke
- Motortreiber L293D: Fertiger IC mit 2 H-Brücken
  - Bis zu 500mA pro Brücke
  - INPUT1/INPUT2: Drehrichtung
  - ENABLE: Freigabe (ideal für PWM-Signal)
  - OUTPUT1/OUTPUT2: Motoranschluss

## Versuchsaufbau

**Teil 1: Signalgenerierung**
- LEDs an Port C
- OC0-Pin (PB3) zur Signalausgabe
- 2 Taster für Frequenzeinstellung (PB6, PB7)
- Terminalprogramm zur Ausgabe

**Teil 2: Motorsteuerung**
- EN (Enable) → OC0 (PB3) - PWM-Signal
- IN1 → PB0 - Drehrichtung Bit 1
- IN2 → PB1 - Drehrichtung Bit 2
- OUT1/OUT2 → Drehzahlsensoren (optional)
- VCC, GND → Stromversorgung

Taster:
- PB6: TASTER_UP (Drehzahl erhöhen)
- PB7: TASTER_DOWN (Drehzahl verringern)
- PB5: TASTER_STOP (Notstopp)

## Aufgabenstellung

### Teil 1: Signalgenerierung mit Timer0
**1.1: Rechtecksignal mit fester Frequenz**
- Ziel: CTC-Mode konfigurieren und Rechtecksignal ausgeben
- Timer0 Konfiguration:
```c
// WGM01:0 = '10' (CTC Mode)
// COM01:0 = '01' (Toggle OC0 bei Compare Match)
// CS02:0 = '101' (Prescaler 1024)
// OCR0 = 0x80
```
- Arbeitsschritte:
  - Timer0 im CTC-Modus initialisieren
  - Frequenz des Ausgangssignals berechnen
  - Programm flashen und testen
  - Signal mit Oszilloskop messen und verifizieren

**1.2: Terminalausgabe der Frequenz**
- Ziel: Berechnete Frequenz auf Terminal ausgeben
- Berechnungsformeln:
```c
Periode = ((2*1024) * (OCR0+1)) / (F_CPU/1000000);
Frequenz = 1000000 / Periode;
```
- Arbeitsschritte:
  - Programm aus 1.1 kopieren
  - Frequenzberechnung implementieren
  - Ausgabe mit sprintf formatieren
  - Terminal-Ausgabe testen

**1.3: Einstellbare Frequenz**
- Ziel: Frequenz über Taster verändern
- Funktionalität:
  - TASTER_UP: OCR0 erhöhen → höhere Frequenz
  - TASTER_DOWN: OCR0 verringern → niedrigere Frequenz
  - 256 Frequenzstufen verfügbar
- Arbeitsschritte:
  - Tastenabfrage implementieren
  - Zustandsautomat für Tastenentprellung erstellen
  - OCR0-Wert bei Tastendruck anpassen
  - Aktuelle Frequenz auf Terminal ausgeben
  - Mit Oszilloskop verifizieren

### Teil 2: Motorsteuerung mit PWM
**2.1: Grundlegende PWM-Motorsteuerung**
- Ziel: Motor mit festem PWM-Signal betreiben
- Timer0 Konfiguration:
```c
// WGM01:0 = '01' (Phase Correct PWM)
// COM01:0 = '10' (Clear on up, Set on down)
// CS02:0 = '100' (Prescaler 256)
// OCR0 = 0x30 (niedrige Startdrehzahl)
```
- Arbeitsschritte:
  - Timer0 für PWM konfigurieren
  - Drehrichtung über IN1/IN2 festlegen
  - Motor über EN-Pin ansteuern
  - PWM-Signal auf Oszilloskop prüfen
  - Drehzahlsensor-Signale beobachten

**2.2: Variable Drehzahlsteuerung**
- Ziel: Drehzahl über Taster steuern
- Funktionalität:
  - TASTER_UP: OCR0++ → Drehzahl erhöhen
  - TASTER_DOWN: OCR0-- → Drehzahl verringern
  - Wertebereich: 0-255 (mit Begrenzung!)
  - Eine Stufe pro Tastendruck
- Arbeitsschritte:
  - Tastensteuerung implementieren
  - Überlauf-Schutz einbauen (0↔255)
  - Tastenentprellung sicherstellen
  - Drehzahlstufe auf Terminal ausgeben
  - Motorverhalten testen

**2.3: Notstopp-Funktion**
- Ziel: Sicherheitsfunktion zum sofortigen Motorstopp
- Funktionalität:
  - TASTER_STOP (PB5): Sofortiger Motorstopp
  - OCR0 = 0 setzen
  - Unabhängig von aktueller Drehzahl
- Arbeitsschritte:
  - Stop-Taste in Tastenabfrage integrieren
  - Bei Betätigung: Motor sofort stoppen
  - Funktion sicher testen

## Register-Konfigurationen
**TCCR0 (Timer/Counter Control Register)**
```
Bit:  7      6      5      4      3      2      1      0
      FOC0   WGM00  COM01  COM00  WGM01  CS02   CS01   CS00
```
**WGM (Waveform Generation Mode):**
- 00 → Normal Mode
- 01 → Phase Correct PWM
- 10 → CTC
- 11 → Fast PWM

**COM (Compare Output Mode):**
- 00 → OC0 nicht verbunden
- 01 → Toggle OC0 bei Compare Match
- 10 → Clear OC0 bei Compare Match
- 11 → Set OC0 bei Compare Match

**CS (Clock Select / Prescaler):**
- 000 → Kein Takt (gestoppt)
- 001 → 1
- 010 → 8
- 011 → 64
- 100 → 256
- 101 → 1024

## Berechnungsbeispiele
**Frequenzberechnung CTC-Mode**
```
f_out = f_CPU / (2 × N × (OCR0 + 1))
```
Beispiel:  
- f_CPU = 16 MHz  
- N = 1024 (Prescaler)  
- OCR0 = 127  

```
f_out = 16.000.000 / (2 × 1024 × 128) = 61,04 Hz
```

**PWM Duty Cycle**
```
Duty Cycle = (OCR0 / 255) × 100%
```
Beispiel:  
- OCR0 = 64  
- Duty Cycle = (64/255) × 100% = 25,1%

## Debugging-Strategien
- **Simulation**
  - Tastenabfrage im Simulator testen
  - PWM-Verhältnis am PINB3 beobachten
  - Wait(1ms) in while-Schleife für Breakpoints einfügen
  - Register-Werte während Ausführung prüfen
- **Oszilloskop**
  - PWM-Signal direkt am OC0-Pin messen
  - Frequenz und Duty Cycle verifizieren
  - Drehzahlsensor-Signale analysieren
  - Periodendauer messen
- **Terminal**
  - Kontrollausgaben für Drehzahlstufe
  - Frequenzberechnung verifizieren
  - Debug-Informationen bei Ereignissen
- **Motor-Sicherheit**
  - PWM-Signal erst ohne Motor testen
  - EN-Pin erst nach erfolgreicher Signalprüfung verbinden
  - Notstopp-Funktion vor ausgiebigen Tests implementieren
  - Keine gefährlichen Tests durchführen!

## Häufige Fehlerquellen
- Motor dreht nicht → DDRB nicht konfiguriert, OC0-Pin als Ausgang setzen  
- Überlauf bei OCR0 → Keine Grenzkontrolle, Werte auf 0-255 begrenzen  
- Falsche Drehrichtung → IN1/IN2 vertauscht, Pin-Zuordnung prüfen  
- Zu hohe/niedrige Frequenz → Falscher Prescaler, Prescaler-Wert anpassen  
- Mehrfach-Inkrementierung → Keine Tastenentprellung, Zustandsautomat implementieren  
- Motor brummt nur → PWM-Frequenz zu niedrig, höheren Prescaler wählen  

## Abnahmekriterien

**Funktionale Anforderungen:**
- ✓ Signalgenerierung mit einstellbarer Frequenz funktioniert
- ✓ Variable Motorsteuerung über Tasten implementiert
- ✓ Notstopp-Funktion vorhanden und getestet
- ✓ Terminalausgabe korrekt

**Dokumentation:**
- ✓ Frequenzberechnung mit vollständigem Rechenweg
- ✓ Code gut kommentiert und strukturiert
- ✓ Messwerte dokumentiert

## Technische Details

**L293D Motortreiber**
- Dual H-Bridge IC (2 Motoren steuerbar)
- Max. 600mA pro Kanal (mit Kühlung)
- Interne Schutzdioden vorhanden
- TTL-kompatible Eingänge
- Achtung: Wärmeentwicklung beachten!

**Drehzahlsensor**
- Gabellichtschranken-Prinzip
- Propellerflügel unterbricht Lichtstrahl
- Output-Frequenz proportional zur Drehzahl
- Nutzbar für geschlossene Regelkreise (hier nicht verwendet)

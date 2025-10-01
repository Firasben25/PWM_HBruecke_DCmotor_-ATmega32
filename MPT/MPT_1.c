
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MPT_MAIN.h"
#include "MPT_UTIL.h"
#include "MPT_1.h"

#ifdef ENABLE_A_1

#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define TASTER_DDR  DDRB
#define TASTER_PIN  PINB
#define TASTER_UP   PB6
#define TASTER_DOWN PB7
#define FG_DDR      DDRB
#define FG_PORT     PORTB
#define FG_PIN      PB3
#define FG_DELTA    10


// A_1_1: Timer0 als Frequenzgenerator mit fester Frequenz.
void A_1_1(void)
{
	
	// Richtungsregister f�r LEDs auf Ausgang
	LED_DDR = 0b11111111;
	
	// Alle LEDs aus
	LED_PORT = 0b11111111;
	
	// Richtungsregister f�r Impulsausgang
	FG_DDR = (1<<FG_PIN);
	
	// Konfigurationsregister:
	// CTC Mode
	// Vorteiler 1024
	TCCR0 = (1<<WGM01)| (0<<WGM00)| (0<<COM01)| (1<<COM00)| (1<<CS02)| (0<<CS01)| (1<<CS00);
	
	TCNT0 = (0<<TCNT0);
	
	// Vergleichsregister initialisieren
	OCR0=0x80;
	
	// Interruptmaskenregister setzen:
	// TOIE0:0 = INT  ausl�sen bei �berlauf -> Timer0 nicht aktiv
	// OCIE0:0 = INT ausl�sen bei Vergleich -> Timer0 nicht aktiv
	TIMSK = (0<<OCIE0)| (0<<TOIE0);
	
	
	CLR_BIT(LED_PORT,0);
	
	while (1){
		
	}
	
}

// A_1_2: Timer0 als Frequenzgenerator mit fester Frequenz und Terminalausgabe.
void A_1_2(void)
{
	uint32_t freq, period;
	char puffer[80];
	
	// Richtungsregister für LEDs auf Ausgang
	LED_DDR = 0b11111111;
	
	// Alle LEDs aus
	LED_PORT = 0b11111111;
	
	// Usart-Schnittstelle initialisiert
	UsartInit(8,1,0,9600);
	
	// Konfigurationsregister:
	// CTC Mode
	// Normaler Vorteiler 1024
	TCCR0 = (1<<WGM01)| (0<<WGM00)| (0<<COM01)| (1<<COM00)| (1<<CS02)| (0<<CS01)| (1<<CS00);
	TCNT0 = (0<<TCNT0);
	
	// Richtungsregister f�r Impulsausgang
	FG_DDR = (1<<FG_PIN);
	
	// Vergleichsregister initialisieren
	OCR0=0x80;
	
	// Interruptmaskenregister setzen:
	// TOIE0:0 = INT  ausl�sen bei �berlauf  -> Timer0 nicht aktiv
	// OCIE0:0 = INT  ausl�sen bei Vergleich -> Timer0 nicht aktiv
	TIMSK = (0<<OCIE0)| (0<<TOIE0);
	
	PORTC = CLR_BIT(PORTC,0);
	
	// Berechnung der Periode und Frequenz
	period = ((uint32_t)(2*1024)*(uint32_t)(OCR0+1))/((uint32_t)F_CPU/(uint32_t)1000000);
	freq = ((uint32_t)1000000)/period;
	
	// Terminalausgabe
	sprintf(puffer,"Frequenz = %ld Hz\r\n",freq);
	UsartPuts(puffer);
	
	while (1){
	}
}

// A_1_3: Timer0 als Frequenzgenerator mit einstellbarer Frequenz und Terminalausgabe.
void A_1_3(void)
{
	uint32_t freq, period;
	char cBuff[80];
	
	// Richtungsregister für LEDs auf Ausgang
	LED_DDR = 0b11111111;
	
	// Alle LEDs aus
	LED_PORT = 0b11111111;
	
	//Usart-Schnittstelle initialisiert
	UsartInit(8,1,0,9600);
	
	// Konfigurationsregister:
	// CTC Mode
	// Vorteiler 1024
	TCCR0 = (1<<WGM01)| (0<<WGM00)| (0<<COM01)| (1<<COM00)| (1<<CS02)| (0<<CS01)| (1<<CS00);
	
	TCNT0 = (0<<TCNT0);
	
	//Richtungsregister f�r Impulsausgang
	FG_DDR = (1<<FG_PIN);
	
	// Vergleichsregister initialisieren
	OCR0=0x80;
	
	// Interruptmaskenregister setzen:
	// TOIE0:0 = INT ausl�sen bei �berlauf  -> Timer0 nicht aktiv
	// OCIE0:0 = INT ausl�sen bei Vergleich -> Timer0 nicht aktiv
	TIMSK = (0<<OCIE0)| (0<<TOIE0);
	
	PORTC = CLR_BIT(PORTC,0);

	while (1){
		
		// Abfrage,ob Taster UP gedr�ckt und Taster DOWN nicht gedr�ckt
		if((PINB & (1<<6)) && !(PINB & (1<<7))){
			OCR0=OCR0+1;
			while((PINB & (1<<6)) && !(PINB & (1<<7))){
			}
		}
		Wait_x_ms(300); 
		
		//Abfrage, ob Taster DOWN gedr�ckt und Taster UP nicht gedr�ckt
		if((PINB & (1<<7)) && !(PINB & (1<<6)) ){
			OCR0=OCR0-1;
			while(!(PINB & (1<<6)) && (PINB & (1<<7))){
			}
		}
		
		//Berechnung der Periode und Frequenz
		period = ((uint32_t)(2*1024)*(uint32_t)(OCR0+1))/((uint32_t)F_CPU/(uint32_t)1000000);
		freq = ((uint32_t)1000000)/period;
		
		// Terminalausgabe
		sprintf(cBuff,"freq = %5d Hz\r\n",freq);
		UsartPuts(cBuff);
		Wait_x_ms(300);
	}
}

#endif 
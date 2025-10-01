
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MPT_MAIN.h"
#include "MPT_UTIL.h"
#include "MPT_2.h"

#ifdef ENABLE_A_2

#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define FG_DDR      DDRB
#define FG_PORT     PORTB
#define FG_PIN      PB3
#define FG_DELTA    10
#define LED_DELAY   50
#define TASTER_DDR  DDRB
#define TASTER_PIN  PINB
#define TASTER_STOP PB5
#define TASTER_SET  PB5
#define TASTER_UP   PB6
#define TASTER_DOWN PB7

#define    MOTOR_DIR_DDR DDRB
#define    MOTOR_PWM_DDR DDRB

#define    MOTOR_DIR_PORT PORTB

#define MOTOR_DIR0_PIN PB0
#define MOTOR_DIR1_PIN PB1
#define MOTOR_PWM_PIN  PB3 /* OC0 */


// A_2_1: Motorsteuerung mit fester Drehzahl und Terminalausgabe.
void A_2_1(void)
{

    uint32_t freq, period;
    char puffer[80];

    // Richtungsregister f�r die LEDs
    LED_DDR = 0b11111111;

    // Alle LEDs ausgeschaltet
    LED_PORT = 0b11111111;

    // Richtungsregister f�r Impulsausgang
    FG_DDR = (1 << FG_PIN);

    // Richtungsregister f�r Motorrichtung
    MOTOR_DIR_DDR = 0b11111111;
	
    // Richtungsregister f�r Motor-PWM-Ausgang
    MOTOR_PWM_DDR = 0b11111111;

    // Motordrehrichtung
    CLR_BIT(MOTOR_DIR_PORT,MOTOR_DIR0_PIN);
    SET_BIT(MOTOR_DIR_PORT,MOTOR_DIR1_PIN);

    // Usart-Schnittstelle
    UsartInit(8,1,0,9600);

    TCNT0 = 0;

    // Vergleichsregister
    OCR0  = 0x30;

    // Konfigurationsregister Normaler Betrieb Vorteiler 256
    TCCR0 = (0<<WGM01) | (1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<CS02) | (0<<CS01) | (0<<CS00);

    // Interruptmaskenregister setzen
    TIMSK |= (0<<TOIE0) | (0<<OCIE0);

    CLR_BIT(LED_PORT,0);

    sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
    UsartPuts(puffer);

    while(1)
    {
    }

}

//##############################################################################

// A_2_2: Motorsteuerung mit einstellbarer Drehzahl und Terminalausgabe.
void A_2_2(void)
{
	
	uint32_t freq, period;
	char puffer[80];
	
	// Richtungsregister f�r die LEDs
	LED_DDR = 0b11111111;

	// Alle LEDs ausgeschaltet
	LED_PORT = 0b11111111;
	
	//Richtungsregister f�r Impulsausgang
	FG_DDR = (1 << FG_PIN);
	
	//Richtungsregister f�r Motorrichtung
	MOTOR_DIR_DDR = 0b11111111;
	
	//Richtungsregister f�r Motor-PWM-Ausgang
	MOTOR_PWM_DDR = 0b11111111;
	
	//Motordrehrichtung
	CLR_BIT(MOTOR_DIR_PORT,MOTOR_DIR0_PIN);
	SET_BIT(MOTOR_DIR_PORT,MOTOR_DIR1_PIN);
	
	//Usart-Schnittstelle
	UsartInit(8,1,0,9600);

	TCNT0 = 0;

	// Vergleichsregister
	OCR0  = 0x30;

	// Konfigurationsregister 
	// PWM Phase 
	// Vorteiler 256
	TCCR0 = (0<<WGM01) | (1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<CS02) | (0<<CS01) | (0<<CS00);

	// Interruptmaskenregister setzen
	TIMSK |= (0<<TOIE0) | (0<<OCIE0);

	CLR_BIT(LED_PORT,0);

	sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
	UsartPuts(puffer);
	
	while(1)
	{
		// Tasterabfrage
		if (BIT_IS_SET(TASTER_PIN,TASTER_UP) && OCR0<255)
		{
			OCR0 = OCR0+1;
			sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
			UsartPuts(puffer);
			while(BIT_IS_SET(TASTER_PIN,TASTER_UP))
			{
				
			}
			Wait_x_ms(500);
		}
		
		// Tasterabfrage
		if (BIT_IS_SET(TASTER_PIN,TASTER_DOWN) && OCR0>0)
		{
			OCR0 =OCR0-1;
			sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
			UsartPuts(puffer);
			while(BIT_IS_SET(TASTER_PIN,TASTER_DOWN))
			{
			}
			Wait_x_ms(500);
		}
		
		period = ((uint32_t)(2*1024)*(uint32_t)(OCR0+1))/((uint32_t)F_CPU/(uint32_t)1000000);
		freq = ((uint32_t)1000000)/period;
	}
}

// A_2_3: Motorsteuerung mit einstellbarer Drehzahl, Stopfunktion und Terminalausgabe.
void A_2_3(void)
{

uint32_t freq, period;
char puffer[80];

   // Richtungsregister f�r die LEDs
   LED_DDR = 0b11111111;

   // Alle LEDs ausgeschaltet
   LED_PORT = 0b11111111;

   //Richtungsregister f�r Impulsausgang
   FG_DDR = (1 << FG_PIN);

   //Richtungsregister f�r Motorrichtung
   MOTOR_DIR_DDR = 0b11111111;
   
   //Richtungsregister f�r Motor-PWM-Ausgang
   MOTOR_PWM_DDR = 0b11111111;

   //Motordrehrichtung
   CLR_BIT(MOTOR_DIR_PORT,MOTOR_DIR0_PIN);
   SET_BIT(MOTOR_DIR_PORT,MOTOR_DIR1_PIN);

   //Usart-Schnittstelle
   UsartInit(8,1,0,9600);

   TCNT0 = 0;

   // Vergleichsregister
   OCR0  = 55;

   // Konfigurationsregister Normaler Betrieb Vorteiler 256
   TCCR0 = (0<<WGM01) | (1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<CS02) | (0<<CS01) | (0<<CS00);

   // Interruptmaskenregister setzen
   TIMSK |= (0<<TOIE0) | (0<<OCIE0);

   CLR_BIT(LED_PORT,0);

   sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
   UsartPuts(puffer);

   while(1)
   {
 	   // Tasterabfrage
	   if (BIT_IS_SET(TASTER_PIN,TASTER_UP) && OCR0<255)
	   {
		   OCR0 = OCR0+1;
		   sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
		   UsartPuts(puffer);
		   while(BIT_IS_SET(TASTER_PIN,TASTER_UP))
	       {
			
	   }
	   Wait_x_ms(500);
	}
	
	// Tasterabfrage
	if (BIT_IS_SET(TASTER_PIN,TASTER_DOWN) && OCR0>0)
	{
		OCR0 =OCR0-1;
		sprintf(puffer, "Drehzahlstufe= %i \n\r", OCR0);
		UsartPuts(puffer);
		while(BIT_IS_SET(TASTER_PIN,TASTER_DOWN))
		{
		}
		Wait_x_ms(500);
	}
	
	//Notstop Taster
	if (BIT_IS_SET(TASTER_PIN,4) && OCR0>20){
		OCR0=0;
		UsartPuts("Not-Aus betaetigt\n\r");
		while(BIT_IS_SET(TASTER_PIN,4))
		{
		}
		Wait_x_ms(500);
		
		
	}
	
	period = ((uint32_t)(2*1024)*(uint32_t)(OCR0+1))/((uint32_t)F_CPU/(uint32_t)1000000);
	freq = ((uint32_t)1000000)/period;
}
}

#endif 
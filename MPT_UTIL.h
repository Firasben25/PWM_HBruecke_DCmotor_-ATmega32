#ifndef MPT_UTIL_H_
#define MPT_UTIL_H_

void Wait_x_ms(uint16_t x_ms);

#define SET_BIT(REG,BIT)     REG  |=  (1<<BIT)        /* Setzen        des Bits BIT in Register REG */
#define CLR_BIT(REG,BIT)     REG  &= ~(1<<BIT)        /* L�schen       des Bits BIT in Register REG */
#define TGL_BIT(REG,BIT)     REG  ^=  (1<<BIT)        /* Komplemtieren des Bits BIT in Register REG */
#define BIT_IS_SET(REG,BIT)  ((REG & (1<<BIT)) != 0)  /* Testen, ob das Bits BIT in Register REG gesetzt  ('1') ist */
#define BIT_IS_CLR(REG,BIT)  ((REG & (1<<BIT)) == 0)  /* Testen, ob das Bits BIT in Register REG gel�scht ('0') ist */

#define SBI SET_BIT     /* Kurzform zu SET_BIT */
#define CBI CLR_BIT     /* Kurzform zu CLR_BIT */
#define TBI TGL_BIT     /* Kurzform zu TGL_BIT */
#define BIS BIT_IS_SET  /* Kurzform zu BIT_IS_SET */
#define BIC BIT_IS_CLR  /* Kurzform zu BIT_IS_CLR */

#define LED_0   0
#define LED_1   1
#define LED_2   2
#define LED_3   3
#define LED_4   4
#define LED_5   5
#define LED_6   6
#define LED_7   7
#define LED_ALL 8

/*
void LedOn(uint8_t Led);
void LedOff(uint8_t Led);
void LedToggle(uint8_t Led);
*/

#define F_CPU 16000000

#define USART_EVEN_PARITY 1
#define USART_ODD_PARITY  2

void UsartInit(uint8_t CharBits, uint8_t ParBit, uint8_t StopBits, uint32_t Baudrate);
/*
 *  1 Zeichen auf USART-Kanal ausgeben
 */
void UsartPutc(uint8_t Data);
/*
 *  String auf USART-Kanal ausgeben
 */
void UsartPuts(char* pString);
/*
 *  1 Zeichen von USART-Kanal einlesen
 */
uint8_t UsartGetc(void);


#endif /* MPT_UTIL_H_ */

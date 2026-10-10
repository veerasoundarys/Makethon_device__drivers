
#include "timer.h"

/* Timer1 registers: ATmega2560, 16 MHz */

#define TCCR1A_REG (*(volatile unsigned char *)0x80)
#define TCCR1B_REG (*(volatile unsigned char *)0x81)

#define TCNT1L_REG (*(volatile unsigned char *)0x84)
#define TCNT1H_REG (*(volatile unsigned char *)0x85)

#define OCR1AL_REG (*(volatile unsigned char *)0x88)
#define OCR1AH_REG (*(volatile unsigned char *)0x89)

#define TIMSK1_REG (*(volatile unsigned char *)0x6F)

/* Timer1 bit positions */
#define WGM12_BIT  3
#define CS11_BIT   1
#define CS10_BIT   0
#define OCIE1A_BIT 1

void TIMER1_init(void)
{
    /* Stop Timer1 */
    TCCR1A_REG = 0;
    TCCR1B_REG = 0;

    /* Clear timer counter */
    TCNT1H_REG = 0;
    TCNT1L_REG = 0;

    /* Set compare value = 24999 */
    OCR1AH_REG = (24999 >> 8);
    OCR1AL_REG = (24999 & 0xFF);

    /* Enable CTC mode and prescaler 64 */
    TCCR1A_REG = 0;
    TCCR1B_REG = (1 << WGM12_BIT)
               | (1 << CS11_BIT)
               | (1 << CS10_BIT);

    /* Enable Timer1 Compare A interrupt */
    TIMSK1_REG |= (1 << OCIE1A_BIT);
}

#include <stdint.h>
#include "pwm.h"

/* DDRB register */
#define DDRB_REG   (*(volatile uint8_t *)0x24)

/* PORTB register */
#define PORTB_REG  (*(volatile uint8_t *)0x25)

/* Timer1 control registers */
#define TCCR1A_REG (*(volatile uint8_t *)0x80)
#define TCCR1B_REG (*(volatile uint8_t *)0x81)

/* Timer1 compare registers */
#define OCR1AL_REG (*(volatile uint8_t *)0x88)
#define OCR1AH_REG (*(volatile uint8_t *)0x89)

#define OCR1BL_REG (*(volatile uint8_t *)0x8A)
#define OCR1BH_REG (*(volatile uint8_t *)0x8B)

#define OCR1CL_REG (*(volatile uint8_t *)0x8C)
#define OCR1CH_REG (*(volatile uint8_t *)0x8D)

/* Timer1 bit positions */
#define WGM10_BIT  0
#define WGM12_BIT  3

#define COM1A1_BIT 7
#define COM1B1_BIT 5
#define COM1C1_BIT 3

#define CS11_BIT   1
#define CS10_BIT   0


/* Initialize selected PWM channel */
void PWM_Init(uint8_t channel)
{
    /* Configure the selected output pin */
    if (channel == PWM_OC1A)
    {
        DDRB_REG |= (1U << 5);
    }
    else if (channel == PWM_OC1B)
    {
        DDRB_REG |= (1U << 6);
    }
    else if (channel == PWM_OC1C)
    {
        DDRB_REG |= (1U << 7);
    }
    else
    {
        return;
    }

    /*
     * Fast PWM 8-bit mode:
     * WGM12 = 1, WGM10 = 1
     * Prescaler = 64
     */
    TCCR1A_REG |= (1U << WGM10_BIT);

    TCCR1B_REG = (1U << WGM12_BIT)
               | (1U << CS11_BIT)
               | (1U << CS10_BIT);

    /* Enable non-inverting PWM for selected channel */
    if (channel == PWM_OC1A)
    {
        TCCR1A_REG |= (1U << COM1A1_BIT);

        OCR1AH_REG = 0;
        OCR1AL_REG = 0;
    }
    else if (channel == PWM_OC1B)
    {
        TCCR1A_REG |= (1U << COM1B1_BIT);

        OCR1BH_REG = 0;
        OCR1BL_REG = 0;
    }
    else if (channel == PWM_OC1C)
    {
        TCCR1A_REG |= (1U << COM1C1_BIT);

        OCR1CH_REG = 0;
        OCR1CL_REG = 0;
    }
}


/* Set PWM duty: 0 to 255 */
void PWM_SetDuty(uint8_t channel, uint8_t duty)
{
    if (channel == PWM_OC1A)
    {
        OCR1AH_REG = 0;
        OCR1AL_REG = duty;
    }
    else if (channel == PWM_OC1B)
    {
        OCR1BH_REG = 0;
        OCR1BL_REG = duty;
    }
    else if (channel == PWM_OC1C)
    {
        OCR1CH_REG = 0;
        OCR1CL_REG = duty;
    }
}


/* Stop PWM on selected channel */
void PWM_Stop(uint8_t channel)
{
    if (channel == PWM_OC1A)
    {
        TCCR1A_REG &= (uint8_t)~(1U << COM1A1_BIT);
        PORTB_REG &= (uint8_t)~(1U << 5);
    }
    else if (channel == PWM_OC1B)
    {
        TCCR1A_REG &= (uint8_t)~(1U << COM1B1_BIT);
        PORTB_REG &= (uint8_t)~(1U << 6);
    }
    else if (channel == PWM_OC1C)
    {
        TCCR1A_REG &= (uint8_t)~(1U << COM1C1_BIT);
        PORTB_REG &= (uint8_t)~(1U << 7);
    }
}

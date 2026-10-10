#include "ultra.h"
#include "gpio.h"

/* ATmega2560 GPIO registers */
#define PINB_REG   (*(volatile uint8_t *)0x23)
#define DDRB_REG   (*(volatile uint8_t *)0x24)
#define PORTB_REG  (*(volatile uint8_t *)0x25)

/* ATmega2560 Timer4 registers */
#define TCCR4A_REG (*(volatile uint8_t *)0xA0)
#define TCCR4B_REG (*(volatile uint8_t *)0xA1)
#define TCNT4L_REG (*(volatile uint8_t *)0xA4)
#define TCNT4H_REG (*(volatile uint8_t *)0xA5)

/* Ultrasonic pins */
#define TRIG_BIT 1       /* PB1 */
#define ECHO_BIT 2       /* PB2 */

/* Timer4: 16 MHz / 8 = 2 MHz
   One timer tick = 0.5 us */
#define ULTRA_TIMEOUT_TICKS 60000U

static void ULTRA_TimerReset(void)
{
    TCNT4H_REG = 0;
    TCNT4L_REG = 0;
}

static uint16_t ULTRA_TimerRead(void)
{
    uint8_t low;
    uint8_t high;

    /* Read low byte first to latch the high byte */
    low = TCNT4L_REG;
    high = TCNT4H_REG;

    return ((uint16_t)high << 8) | low;
}

static void ULTRA_Delay10us(void)
{
    ULTRA_TimerReset();

    while (ULTRA_TimerRead() < 20U)
    {
        /* 20 ticks x 0.5 us = 10 us */
    }
}

void ULTRA_Init(void)
{
    /* Trigger = output, Echo = input */
    DDRB_REG |= (1U << TRIG_BIT);
    DDRB_REG &= (uint8_t)~(1U << ECHO_BIT);

    /* Trigger low and disable Echo pull-up */
    PORTB_REG &= (uint8_t)~(1U << TRIG_BIT);
    PORTB_REG &= (uint8_t)~(1U << ECHO_BIT);

    /* Configure Timer4 in normal mode, prescaler = 8 */
    TCCR4A_REG = 0x00;
    TCCR4B_REG = 0x00;

    ULTRA_TimerReset();

    TCCR4B_REG = (1U << 1);
}

unsigned int ULTRA_GetDistanceCm(void)
{
    uint16_t start_time;
    uint16_t echo_time;

    /* Ensure Trigger starts LOW */
    PORTB_REG &= (uint8_t)~(1U << TRIG_BIT);
    ULTRA_Delay10us();

    /* Send trigger pulse */
    PORTB_REG |= (1U << TRIG_BIT);
    ULTRA_Delay10us();
    PORTB_REG &= (uint8_t)~(1U << TRIG_BIT);

    /* Wait for Echo to become HIGH */
    ULTRA_TimerReset();

    while ((PINB_REG & (1U << ECHO_BIT)) == 0)
    {
        if (ULTRA_TimerRead() >= ULTRA_TIMEOUT_TICKS)
        {
            return 0U;
        }
    }

    /* Measure the HIGH pulse */
    start_time = ULTRA_TimerRead();

    while ((PINB_REG & (1U << ECHO_BIT)) != 0)
    {
        if ((uint16_t)(ULTRA_TimerRead() - start_time)
            >= ULTRA_TIMEOUT_TICKS)
        {
            return 0U;
        }
    }

    echo_time = (uint16_t)(ULTRA_TimerRead() - start_time);

    /* Distance in cm: ticks / 116 */
    return (unsigned int)(echo_time / 116U);
}

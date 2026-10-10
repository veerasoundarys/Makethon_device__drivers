#include "ir.h"

/* ATmega2560 Port K registers */
#define PINK_REG  (*(volatile uint8_t *)0x106)
#define DDRK_REG  (*(volatile uint8_t *)0x107)
#define PORTK_REG (*(volatile uint8_t *)0x108)

/* IR sensor output: PK0 / Arduino Mega A8 */
#define IR_BIT 0

void IR_Init(void)
{
    /* PK0 = input */
    DDRK_REG &= (uint8_t)~(1U << IR_BIT);

    /* Enable internal pull-up */
    PORTK_REG |= (1U << IR_BIT);
}

uint8_t IR_Read(void)
{
    /* Active-LOW sensor: LOW means object detected */
    if ((PINK_REG & (1U << IR_BIT)) == 0U)
    {
        return 0U;
    }

    return 1U;
}
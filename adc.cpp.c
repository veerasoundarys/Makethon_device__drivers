#include "adc.h"

/* ATmega2560 ADC registers */
#define ADCL_REG   (*(volatile unsigned char *)0x78)
#define ADCH_REG   (*(volatile unsigned char *)0x79)
#define ADCSRA_REG (*(volatile unsigned char *)0x7A)
#define ADCSRB_REG (*(volatile unsigned char *)0x7B)
#define ADMUX_REG  (*(volatile unsigned char *)0x7C)

/* ADC bits */
#define REFS0_BIT  6
#define ADEN_BIT   7
#define ADSC_BIT   6
#define ADPS2_BIT  2
#define ADPS1_BIT  1
#define ADPS0_BIT  0
#define MUX5_BIT   3

void ADC_init(void)
{
    /* Reference voltage = AVCC, result right-adjusted */
    ADMUX_REG = (1U << REFS0_BIT);

    /* Clear MUX5 initially */
    ADCSRB_REG &= (unsigned char)~(1U << MUX5_BIT);

    /* Enable ADC, prescaler = 128 */
    ADCSRA_REG = (1U << ADEN_BIT)
               | (1U << ADPS2_BIT)
               | (1U << ADPS1_BIT)
               | (1U << ADPS0_BIT);
}

unsigned int ADC_read(unsigned char channel)
{
    unsigned char low;
    unsigned char high;

    /* ATmega2560 has ADC channels 0 to 15 */
    if (channel > 15U)
    {
        return 0U;
    }

    /* Select ADC channel */
    if (channel >= 8U)
    {
        ADCSRB_REG |= (1U << MUX5_BIT);
        ADMUX_REG = (1U << REFS0_BIT) | (channel & 0x07U);
    }
    else
    {
        ADCSRB_REG &= (unsigned char)~(1U << MUX5_BIT);
        ADMUX_REG = (1U << REFS0_BIT) | channel;
    }

    /* Start conversion */
    ADCSRA_REG |= (1U << ADSC_BIT);

    /* Wait for conversion to finish */
    while (ADCSRA_REG & (1U << ADSC_BIT))
    {
    }

    /* Read low byte first, then high byte */
    low = ADCL_REG;
    high = ADCH_REG;

    return ((unsigned int)high << 8) | low;
}

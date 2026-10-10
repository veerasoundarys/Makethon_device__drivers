#include "gpio.h"

/*
 * ATmega2560 GPIO register addresses
 *
 * DDRx  = Data Direction Register
 * PORTx = Output Register
 * PINx  = Input Register
 *
 * No Arduino libraries or avr/io.h used.
 */

/* DDR registers */
#define ddr_a (*(volatile uint8_t *)0x21)
#define ddr_b (*(volatile uint8_t *)0x24)
#define ddr_c (*(volatile uint8_t *)0x27)
#define ddr_d (*(volatile uint8_t *)0x2A)
#define ddr_e (*(volatile uint8_t *)0x2D)
#define ddr_f (*(volatile uint8_t *)0x30)
#define ddr_g (*(volatile uint8_t *)0x33)
#define ddr_h (*(volatile uint8_t *)0x101)
#define ddr_j (*(volatile uint8_t *)0x104)
#define ddr_k (*(volatile uint8_t *)0x107)
#define ddr_l (*(volatile uint8_t *)0x10A)

/* PORT registers */
#define out_a (*(volatile uint8_t *)0x22)
#define out_b (*(volatile uint8_t *)0x25)
#define out_c (*(volatile uint8_t *)0x28)
#define out_d (*(volatile uint8_t *)0x2B)
#define out_e (*(volatile uint8_t *)0x2E)
#define out_f (*(volatile uint8_t *)0x31)
#define out_g (*(volatile uint8_t *)0x34)
#define out_h (*(volatile uint8_t *)0x102)
#define out_j (*(volatile uint8_t *)0x105)
#define out_k (*(volatile uint8_t *)0x108)
#define out_l (*(volatile uint8_t *)0x10B)

/* PIN registers */
#define in_a (*(volatile uint8_t *)0x20)
#define in_b (*(volatile uint8_t *)0x23)
#define in_c (*(volatile uint8_t *)0x26)
#define in_d (*(volatile uint8_t *)0x29)
#define in_e (*(volatile uint8_t *)0x2C)
#define in_f (*(volatile uint8_t *)0x2F)
#define in_g (*(volatile uint8_t *)0x32)
#define in_h (*(volatile uint8_t *)0x100)
#define in_j (*(volatile uint8_t *)0x103)
#define in_k (*(volatile uint8_t *)0x106)
#define in_l (*(volatile uint8_t *)0x109)


/* Set the complete port direction */
void GPIO_port_set(char port, int mode)
{
    uint8_t value;

    value = (mode == OUTPUT) ? 0xFF : 0x00;

    switch (port)
    {
        case 'a': ddr_a = value; break;
        case 'b': ddr_b = value; break;
        case 'c': ddr_c = value; break;
        case 'd': ddr_d = value; break;
        case 'e': ddr_e = value; break;
        case 'f': ddr_f = value; break;
        case 'g': ddr_g = value; break;
        case 'h': ddr_h = value; break;
        case 'j': ddr_j = value; break;
        case 'k': ddr_k = value; break;
        case 'l': ddr_l = value; break;
        default: break;
    }
}


/* Set one pin direction */
void GPIO_pin_set(char port, int mode, int pin)
{
    volatile uint8_t *ddr_reg = 0;

    if (pin < 0 || pin >= 8)
        return;

    switch (port)
    {
        case 'a': ddr_reg = &ddr_a; break;
        case 'b': ddr_reg = &ddr_b; break;
        case 'c': ddr_reg = &ddr_c; break;
        case 'd': ddr_reg = &ddr_d; break;
        case 'e': ddr_reg = &ddr_e; break;
        case 'f': ddr_reg = &ddr_f; break;
        case 'g': ddr_reg = &ddr_g; break;
        case 'h': ddr_reg = &ddr_h; break;
        case 'j': ddr_reg = &ddr_j; break;
        case 'k': ddr_reg = &ddr_k; break;
        case 'l': ddr_reg = &ddr_l; break;
        default: return;
    }

    if (mode == OUTPUT)
        *ddr_reg |= (uint8_t)(1U << pin);
    else
        *ddr_reg &= (uint8_t)~(1U << pin);
}


/* Write a complete byte to PORT */
void GPIO_pin_write(char port, volatile uint8_t no)
{
    switch (port)
    {
        case 'a': out_a = no; break;
        case 'b': out_b = no; break;
        case 'c': out_c = no; break;
        case 'd': out_d = no; break;
        case 'e': out_e = no; break;
        case 'f': out_f = no; break;
        case 'g': out_g = no; break;
        case 'h': out_h = no; break;
        case 'j': out_j = no; break;
        case 'k': out_k = no; break;
        case 'l': out_l = no; break;
        default: break;
    }
}


/* Write all port outputs HIGH or LOW */
void GPIO_port_write(char port, int out)
{
    uint8_t value = (out != 0) ? 0xFF : 0x00;

    switch (port)
    {
        case 'a': out_a = value; break;
        case 'b': out_b = value; break;
        case 'c': out_c = value; break;
        case 'd': out_d = value; break;
        case 'e': out_e = value; break;
        case 'f': out_f = value; break;
        case 'g': out_g = value; break;
        case 'h': out_h = value; break;
        case 'j': out_j = value; break;
        case 'k': out_k = value; break;
        case 'l': out_l = value; break;
        default: break;
    }
}


/* Read the complete input port */
uint8_t GPIO_port_read(char port)
{
    switch (port)
    {
        case 'a': return in_a;
        case 'b': return in_b;
        case 'c': return in_c;
        case 'd': return in_d;
        case 'e': return in_e;
        case 'f': return in_f;
        case 'g': return in_g;
        case 'h': return in_h;
        case 'j': return in_j;
        case 'k': return in_k;
        case 'l': return in_l;
        default: return 0;
    }
}


/* Read one input pin */
uint8_t GPIO_pin_read(char port, uint8_t pin)
{
    if (pin >= 8)
        return 0;

    return (GPIO_port_read(port) >> pin) & 1U;
}


/* Set all PORT output bits HIGH */
void GPIO_port_high(char port)
{
    GPIO_port_write(port, 1);
}


/* Set all PORT output bits LOW */
void GPIO_port_low(char port)
{
    GPIO_port_write(port, 0);
}


/* Set one output pin HIGH */
void GPIO_pin_high(char port, uint8_t pin)
{
    volatile uint8_t *port_reg = 0;

    if (pin >= 8)
        return;

    switch (port)
    {
        case 'a': port_reg = &out_a; break;
        case 'b': port_reg = &out_b; break;
        case 'c': port_reg = &out_c; break;
        case 'd': port_reg = &out_d; break;
        case 'e': port_reg = &out_e; break;
        case 'f': port_reg = &out_f; break;
        case 'g': port_reg = &out_g; break;
        case 'h': port_reg = &out_h; break;
        case 'j': port_reg = &out_j; break;
        case 'k': port_reg = &out_k; break;
        case 'l': port_reg = &out_l; break;
        default: return;
    }

    *port_reg |= (uint8_t)(1U << pin);
}


/* Set one output pin LOW */
void GPIO_pin_low(char port, uint8_t pin)
{
    volatile uint8_t *port_reg = 0;

    if (pin >= 8)
        return;

    switch (port)
    {
        case 'a': port_reg = &out_a; break;
        case 'b': port_reg = &out_b; break;
        case 'c': port_reg = &out_c; break;
        case 'd': port_reg = &out_d; break;
        case 'e': port_reg = &out_e; break;
        case 'f': port_reg = &out_f; break;
        case 'g': port_reg = &out_g; break;
        case 'h': port_reg = &out_h; break;
        case 'j': port_reg = &out_j; break;
        case 'k': port_reg = &out_k; break;
        case 'l': port_reg = &out_l; break;
        default: return;
    }

    *port_reg &= (uint8_t)~(1U << pin);
}

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Pin direction */
#define INPUT  0
#define OUTPUT 1

/* GPIO functions */
void GPIO_port_set(char port, int mode);
void GPIO_pin_set(char port, int mode, int pin);

void GPIO_pin_write(char port, volatile uint8_t no);
void GPIO_port_write(char port, int out);

uint8_t GPIO_port_read(char port);
uint8_t GPIO_pin_read(char port, uint8_t pin);

void GPIO_port_high(char port);
void GPIO_port_low(char port);

void GPIO_pin_high(char port, uint8_t pin);
void GPIO_pin_low(char port, uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#define PWM_OC1A 0
#define PWM_OC1B 1
#define PWM_OC1C 2

#ifdef __cplusplus
extern "C" {
#endif

void PWM_Init(uint8_t channel);
void PWM_SetDuty(uint8_t channel, uint8_t duty);
void PWM_Stop(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
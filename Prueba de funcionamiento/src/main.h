#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* Definiciones de pines */
#define LED_GPIO_PORT GPIOC
#define LED_PIN       GPIO_PIN_13

/* Prototipos */
void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

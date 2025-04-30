/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
/* Ajoute ici les GPIO si nécessaires (déjà utilisés dans main.c) */
#define LED_GPIO_Port GPIOI
#define LED_Pin GPIO_PIN_1

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define BEEP_GPIO_PORT GPIOB
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN GPIO_Pin_5

#define RELAY1_GPIO_PORT GPIOB
#define RELAY1_GPIO_CLK RCC_APB2Periph_GPIOB
#define RELAY1_GPIO_PIN GPIO_Pin_3

#define RELAY2_GPIO_PORT GPIOA
#define RELAY2_GPIO_CLK RCC_APB2Periph_GPIOA
#define RELAY2_GPIO_PIN GPIO_Pin_15

// �������˿ڶ���
#define BeepOnt PBout(5) // BEEP,�������ӿ�
#define Beepin PBin(5)   // BEEP,�������ӿ�

#define relay1in PBin(3)
#define relay2in PAin(15)

#define relay1out PBout(3) // LED�ӿ�
#define relay2out PAout(15) // LED�ӿ�

void RELAY_GPIO_Config(void);

#endif /* __RELAY_H */

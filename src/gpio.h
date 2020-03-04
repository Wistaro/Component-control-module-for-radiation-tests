/*
 * gpio.h
 *
 *  Created on: 18 févr. 2020
 *      Author: WilliamR
 */

#ifndef GPIO_H_
#define GPIO_H_

/*USEFUL MACROS*/
#define PIN_LED1 GPIO_PORT_P1, GPIO_PIN0
#define PIN_LED2 GPIO_PORT_P4, GPIO_PIN7

#define PIN_DELATCH_CH1 GPIO_PORT_P4, GPIO_PIN3
#define PIN_DELATCH_CH2 GPIO_PORT_P4, GPIO_PIN0
#define PIN_DELATCH_CH3 GPIO_PORT_P3, GPIO_PIN7

#define PIN_SS1_CURRENT GPIO_PORT_P1, GPIO_PIN4
#define PIN_SS2_CURRENT GPIO_PORT_P1, GPIO_PIN3
#define PIN_SS3_CURRENT GPIO_PORT_P1, GPIO_PIN2

#define PIN_BUTTON_1 GPIO_PORT_P2, GPIO_PIN1
#define PIN_BUTTON_2 GPIO_PORT_P1, GPIO_PIN1

#define PIN_DEBUG_OUT_1 GPIO_PORT_P8, GPIO_PIN2

#define STATE_IDLE 0
#define STATE_HOLD 1
#define STATE_OFF 2

void initGpio();
char delatch(char channelId);
char unlatch(char channelId);
void resetGpio();

char stateChannel1;
char stateChannel2;
char stateChannel3;

char flagDelatchCh1;
char flagDelatchCh2;
char flagDelatchCh3;



#endif /* GPIO_H_ */

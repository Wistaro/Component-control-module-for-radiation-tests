/*
 * pwm.h
 *
 *  Created on: 25 févr. 2020
 *      Author: WilliamR
 */

#ifndef PWM_H_
#define PWM_H_

#define TIMER_PERIOD 1250 //100us
#define INIT_DUTY_CYCLE  340 //1V

char setPwmOnChannel(char idChannel, uint16_t dutyCycle);
char setVoltageOnChannel(char idChannel, uint16_t voltage);
char setRealVoltageOnChannel(char idChannel, uint16_t realVoltage);

Timer_A_outputPWMParam pwm_channel1;
Timer_A_outputPWMParam pwm_channel2;
Timer_A_outputPWMParam pwm_channel3;

void initPwm();



#endif /* PWM_H_ */

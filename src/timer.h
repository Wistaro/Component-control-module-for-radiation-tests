/*
 * timer.h
 *
 *  Created on: 17 févr. 2020
 *      Author: WilliamR
 */

#ifndef TIMER_H_
#define TIMER_H_

#define TIMER_PERIOD 1250 //400us

void initTimerB();

char flagTimer100us;
char flagTimer1ms;

uint16_t cpt_1ms;
uint32_t cpt_delatch_ch1;
uint32_t cpt_delatch_ch2;
uint32_t cpt_delatch_ch3;


#endif /* TIMER_H_ */

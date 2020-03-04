/*
 * config.c
 *
 *  Created on: 21 févr. 2020
 *      Author: WilliamR
 */

#include "driverlib.h"
#include "config.h"
#include "gpio.h"

void resetConfig(){
    version = VERSION;
    author = AUTHOR;

    tholdCh1 = DEFAULT_THOLD;
    tholdCh2 = DEFAULT_THOLD;
    tholdCh3 = DEFAULT_THOLD;


    toffCh1 = DEFAULT_TOFF;
    toffCh2 = DEFAULT_TOFF;
    toffCh3 = DEFAULT_TOFF;

    isChannel1Active = 1;
    isChannel2Active = 0;
    isChannel3Active = 0;

    targetVoltCh1 = MIN_VOLTAGE_CH1;
    targetVoltCh2 = MIN_VOLTAGE_CH2;
    targetVoltCh3 = MIN_VOLTAGE_CH3;

    thresholdCh1 = DEFAULT_THRESHOLD_CH1;
    thresholdCh2 = DEFAULT_THRESHOLD_CH2;
    thresholdCh3 = DEFAULT_THRESHOLD_CH3;

    stateChannel1 = STATE_IDLE;
    stateChannel2 = STATE_IDLE;
    stateChannel3 = STATE_IDLE;

}


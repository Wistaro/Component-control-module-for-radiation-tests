/*
 * pwm.c
 *
 *  Created on: 25 févr. 2020
 *      Author: WilliamR
 */
#include "driverlib.h"
#include "pwm.h"


void initPwm(){

    //Init PWM Channel 1
    GPIO_setAsPeripheralModuleFunctionOutputPin(
                GPIO_PORT_P2,
                GPIO_PIN0
                );
            pwm_channel1.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
            pwm_channel1.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
            pwm_channel1.timerPeriod = TIMER_PERIOD;
            pwm_channel1.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
            pwm_channel1.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
            pwm_channel1.dutyCycle = INIT_DUTY_CYCLE;
            Timer_A_outputPWM(TIMER_A1_BASE, &pwm_channel1);

    //Init PWM Current channel 2
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P2,
            GPIO_PIN5
            );
        pwm_channel2.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
        pwm_channel2.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        pwm_channel2.timerPeriod = TIMER_PERIOD;
        pwm_channel2.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
        pwm_channel2.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
        pwm_channel2.dutyCycle = INIT_DUTY_CYCLE;
        Timer_A_outputPWM(TIMER_A2_BASE, &pwm_channel2);

    //Init PWM Current channel 3
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN5
            );
        pwm_channel3.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
        pwm_channel3.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        pwm_channel3.timerPeriod = TIMER_PERIOD;
        pwm_channel3.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4  ;
        pwm_channel3.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
        pwm_channel3.dutyCycle = INIT_DUTY_CYCLE;
        Timer_A_outputPWM(TIMER_A0_BASE, &pwm_channel3);
}

char setPwmOnChannel(char idChannel, uint16_t dutyCycle){
    switch(idChannel){
        case 1 :
            pwm_channel1.dutyCycle = dutyCycle;
            Timer_A_outputPWM(TIMER_A1_BASE, &pwm_channel1);
            break;
        case 2 :
            pwm_channel2.dutyCycle = dutyCycle;
            Timer_A_outputPWM(TIMER_A2_BASE, &pwm_channel2);
            break;
        case 3 :
            pwm_channel3.dutyCycle = dutyCycle;
            Timer_A_outputPWM(TIMER_A0_BASE, &pwm_channel3);
            break;
        default: break;
    }

    return 1;
}

char setVoltageOnChannel(char idChannel, uint16_t voltage){

    uint16_t dutyCycleTarget = 0;
    dutyCycleTarget = voltage/2.48;

    setPwmOnChannel(idChannel, dutyCycleTarget);

    return 1;
}

char setRealVoltageOnChannel(char idChannel, uint16_t realVoltage){
    switch(idChannel){
    case 1 :
        //setVoltageOnChannel(1,1000*((realVoltage/1000)*0.909-2.04));
        //setVoltageOnChannel(1,1000*(5.2 - (realVoltage/1000) ));

        //setVoltageOnChannel(1,1000*(6 - (realVoltage/1000) ));

        //setVoltageOnChannel(1,1000*(6.01 - 0.956*(realVoltage/1000) ));

        setVoltageOnChannel(1, realVoltage);

        break;
    case 2 :
        setVoltageOnChannel(2,realVoltage);
        break;
    case 3 :
        setVoltageOnChannel(3, realVoltage);
        break;
    default :
        setVoltageOnChannel(1,0);
        setVoltageOnChannel(2,0);
        setVoltageOnChannel(3,0);
        break;
    }

    return 1;
}

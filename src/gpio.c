/*
 * gpio.c
 *
 *  Created on: 18 févr. 2020
 *      Author: WilliamR
 */
#include <stdint.h>
#include <stdio.h>
#include "driverlib.h"
#include "gpio.h"
#include "pwm.h"
#include "uart.h"

void initGpio(){

    /*Debugging buttons*/
    GPIO_setAsInputPin(PIN_BUTTON_1); //button 1
    GPIO_setAsInputPin(PIN_BUTTON_2); //button 2

    GPIO_setAsInputPinWithPullUpResistor(PIN_BUTTON_1);
    GPIO_setAsInputPinWithPullUpResistor(PIN_BUTTON_2);

    GPIO_clearInterrupt(PIN_BUTTON_1);
    GPIO_clearInterrupt(PIN_BUTTON_2);

    GPIO_enableInterrupt(PIN_BUTTON_1);
    GPIO_enableInterrupt(PIN_BUTTON_2);

    /*Debug output*/
    GPIO_setAsOutputPin(PIN_DEBUG_OUT_1); //P8.2

    /* Debugging leds */
    GPIO_setAsOutputPin(PIN_LED1); //led1


    GPIO_setAsOutputPin(PIN_LED2); //led 2

    /*Délatcheur output*/
    GPIO_setAsOutputPin(PIN_DELATCH_CH1); //P4.3
    GPIO_setAsOutputPin(PIN_DELATCH_CH2); //P4.0
    GPIO_setAsOutputPin(PIN_DELATCH_CH3); //P3.7

    /*Slaves select for SPI*/

    GPIO_setAsOutputPin(PIN_SS1_CURRENT); //P1.4 - Sensor 1
    GPIO_setAsOutputPin(PIN_SS2_CURRENT); //P1.3 - Sensor 2
    GPIO_setAsOutputPin(PIN_SS3_CURRENT); //P1.2 - Sensor 3

    resetGpio();

    flagDelatchCh1 = 0;
    flagDelatchCh2 = 0;
    flagDelatchCh3 = 0;

}

void resetGpio(){
    GPIO_setOutputLowOnPin(PIN_LED1); //debug led
    GPIO_setOutputLowOnPin(PIN_LED2); //debug led

    GPIO_setOutputLowOnPin(PIN_DEBUG_OUT_1); //debug output

    GPIO_setOutputHighOnPin(PIN_DELATCH_CH1); //delatcheur
    GPIO_setOutputHighOnPin(PIN_DELATCH_CH2); //delatcheur
    GPIO_setOutputHighOnPin(PIN_DELATCH_CH3); //delatcheur

    GPIO_setOutputLowOnPin(PIN_SS1_CURRENT); //spi slave select current sensor 1
    GPIO_setOutputLowOnPin(PIN_SS2_CURRENT); //spi slave select current sensor 2
    GPIO_setOutputLowOnPin(PIN_SS3_CURRENT); //spi slave select current sensor 3
}
char delatch(char channelId){

    switch(channelId){
        case 1 : GPIO_setOutputLowOnPin(PIN_DELATCH_CH1); break;
        case 2 : GPIO_setOutputLowOnPin(PIN_DELATCH_CH2); break;
        case 3 : GPIO_setOutputLowOnPin(PIN_DELATCH_CH3); break;
        default : break;
    }


    return 1;

}
char unlatch(char channelId){

    switch(channelId){
            case 1 : GPIO_setOutputHighOnPin(PIN_DELATCH_CH1); break;
            case 2 : GPIO_setOutputHighOnPin(PIN_DELATCH_CH2); break;
            case 3 : GPIO_setOutputHighOnPin(PIN_DELATCH_CH3); break;
            default : break;
        }

    return 1;
}

//interruption routine for button 2
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT1_VECTOR)))
#endif
__interrupt void InterruptVectorPort1()
{
    if(__even_in_range( P1IV, P1IV_P1IFG7 ) == P1IV_P1IFG1){
        GPIO_clearInterrupt(PIN_BUTTON_2);

        setVoltageOnChannel(1, 3000); //debug only
    }
}

//interruption routine for button 1
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT2_VECTOR)))
#endif
__interrupt void InterruptVectorPort2()
{
    if(__even_in_range( P2IV, P2IV_P2IFG7 ) == P2IV_P2IFG1){
        GPIO_clearInterrupt(PIN_BUTTON_1);
        GPIO_toggleOutputOnPin(PIN_DEBUG_OUT_1); //debug only
        setVoltageOnChannel(1, 100); //debug only
    }
}

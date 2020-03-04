/*
 * -----------------
 * TILU 2 PROJECT
 * ------------------
 * CNES x ENSEEIHT
 *******************
 * Developed by WilliamR
 *
 * --------------
 * PIN MAPPING
 * --------------
 * >>>> VOLTAGE SENSOR PART
 * > Voltage sensor #1 : P6.0 [Analog IN]
 * > Voltage sensor #2 : P6.1 [Analog IN]
 * > Voltage sensor #3 : P6.3 [analog IN]
 *
 * >>>> DELATCHEUR
 * > To input delatcheur channel 1 : P4.3 [Logic OUT]
 * > To input delatcheur channel 2 : P4.0 [Logic OUT]
 * > To input delatcheur channel 3 : P3.7 [Logic OUT]
 *
 *
 * >>>> CURRENT SENSORS v2
 * > Current Sensor 1 : P6.4 [Analog IN] (A4)
 * > Current Sensor 2 : P6.5 [Analog IN] (A5)
 * > Current Sensor 2 : P7.0 [Analog IN] (A12)
 *
 * >>>> DC/DC with PWM
 * > Out DCDC Channel #1 : P2.0 [PWM OUT] (TA1.1)
 * > Out DCDC Channel #2 : P2.5 [PWM OUT] (TA2.2)
 * > Out DCDC Channel #3 : P1.5 [PWM OUT] (TA0.4)
 */
#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>
#include <msp430.h>
#include  "timer.h"
#include  "gpio.h"
#include "adc.h"
#include "uart.h"
#include "config.h"
#include "pwm.h"
#include "setup.h"


int main(void)
{
    WDT_A_hold(WDT_A_BASE); //hold watchdog

    launchInitSequence();

    /*Default setup*/
    setRealVoltageOnChannel(1,0);
    setRealVoltageOnChannel(2,0);
    setRealVoltageOnChannel(3,0);

    GPIO_setOutputHighOnPin(PIN_DELATCH_CH1); //enable load on channel 1
    GPIO_setOutputHighOnPin(PIN_DELATCH_CH2); // enable load on channel 2
    GPIO_setOutputHighOnPin(PIN_DELATCH_CH3); // enable load on


       while (1){

           interfaceListener(); //listen and handle data from interface

           voltageSensorListener(); //listen and handle data from voltage sensors

           currentSensorListenerV2(); //listen and handle data from currentSensors

       }
}



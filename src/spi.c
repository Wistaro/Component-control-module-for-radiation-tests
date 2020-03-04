/*
 * spi.c
 *
 *  Created on: 23 févr. 2020
 *      Author: WilliamR
 */

#include "driverlib.h"
#include "spi.h"
#include "gpio.h"
#include "uart.h"
#include "config.h"
#include <stdio.h>
#include <msp430.h>

void initSpi(){
    /*
    *------------------
    * SPI PINS REMINDER
    *-------------------
    * P3.2    UCB0CLK
    * P3.0    UCB0SIMO    MOSI
    * P3.1    UCABSOMI    MISO
    *
    * Slave select : see gpio.h
    *
    *
    */
    //Enable MOSI, MISO and clk
    GPIO_setAsPeripheralModuleFunctionInputPin(
                               GPIO_PORT_P3,
                               GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN0
                               );
    USCI_B_SPI_initMasterParam initSpiParam = {0};
    initSpiParam.clockPhase = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT    ;
    initSpiParam.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW  ;
    initSpiParam.clockSourceFrequency =  UCS_getSMCLK();
    initSpiParam.desiredSpiClock = SPI_CLK;
    initSpiParam.msbFirst = USCI_B_SPI_MSB_FIRST;
    initSpiParam.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;

    int debug = USCI_B_SPI_initMaster(USCI_B0_BASE, &initSpiParam);

    if(STATUS_FAIL == debug) {
            printf("Erreur spi init");
            return;
        }

    USCI_B_SPI_enable(USCI_B0_BASE);

        //Enable Receive interrupt
        USCI_B_SPI_clearInterrupt(USCI_B0_BASE,
                                   USCI_B_SPI_RECEIVE_INTERRUPT
                                   );
       USCI_B_SPI_enableInterrupt(USCI_B0_BASE,
                                   USCI_B_SPI_RECEIVE_INTERRUPT
                                   );
       activeChannel = 0; //no active channel, SS is high for all current sensors
       rawDataFromSpiChannel1 = 0x0000;
       rawDataFromSpiChannel1 = 0x0000;
       index2readSpi = 0;
       allBitRead = 0;

}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif

void USCI_B0_ISR(void)
{
    if(TRIG_IF_SPI_B){ //trig RXIF
        receiveDataOnSpi = USCI_B_SPI_receiveData(USCI_B0_BASE);
        flagDataReceivedOnSpi = 0x01;
    }
}

char currentSensorListener(){

    /*Handle data from current sensors
     *
     * STEP 1 :  Get the 8 bits LSB of the 16bits data (each value read by the uc is 8bits)
     * STEP 2 :  Get the 8bits MSB of the 16bits data (each value read by the uc is 8bits)
     * STEP 3 :  Calculate the value of the current (in mA)
     * STEP 4 :  Compare to the threshold
     * STEP 5 :  if the current value is above the threshold, delatch immediately
     * */

    if(flagDataReceivedOnSpi == 0x01){

        flagDataReceivedOnSpi = 0x00; //clear custom flag :p

        //STEP 1
        if(index2readSpi == 0){ //first 8bits read from the current sensor

            while (!(UCA0IFG&UCTXIFG));
            USCI_B_SPI_transmitData(USCI_B0_BASE, 0);

            switch(activeChannel){
                case 1 : rawDataFromSpiChannel1 = (uint16_t)receiveDataOnSpi; break; //put "receiveDataOnSpi" on LSB
                case 2 : rawDataFromSpiChannel2 = (uint16_t)receiveDataOnSpi; break; //put "receiveDataOnSpi" on LSB
                case 3 : rawDataFromSpiChannel3 = (uint16_t)receiveDataOnSpi; break;// put "receiveDataOnSpi" on LSB
                default : break;
            }

            index2readSpi = 1;

            return 1; //the 16bits data is not complete, so we return to wait the others 8 bits

        //STEP 2
        }else if(index2readSpi == 1){ //8 others bits read from the current sensor

                switch(activeChannel){
                   case 1 : rawDataFromSpiChannel1 = rawDataFromSpiChannel1 | (uint16_t)receiveDataOnSpi<<8; break;
                   case 2 : rawDataFromSpiChannel2 = rawDataFromSpiChannel2 | (uint16_t)receiveDataOnSpi<<8; break;
                   case 3 : rawDataFromSpiChannel3 = rawDataFromSpiChannel3 | (uint16_t)receiveDataOnSpi<<8; break;
                   default : break;
                 }

                index2readSpi = 0;
        }
        //STEP 3


        allBitRead = 0 ;

        computeCurrentValues();

        //STEP 4 and 5
        checkThreshold();
     }

    return 1;
}

void computeCurrentValues(){ //compute current values - in mA
    switch(activeChannel){
       case 1 : readCurrentChannel1 = (rawDataFromSpiChannel1 * LOW_CURRENT_CHANNEL_FACTOR)/1000; break;
       case 2 : readCurrentChannel2 = (rawDataFromSpiChannel2 * LOW_CURRENT_CHANNEL_FACTOR)/1000; break;
       case 3 : readCurrentChannel3 = (rawDataFromSpiChannel3 * HIGH_CURRENT_CHANNEL_FACTOR)/1000; break;
       default : break;
    }

}

void checkThreshold(){
    switch(activeChannel){
          case 1 :
              if(readCurrentChannel1 > thresholdCh1){
                  delatch(1);
              }else{
                  unlatch(1);
              }

             break;
          case 2 :
              if(readCurrentChannel2 > thresholdCh2){
                  delatch(2);
               }else{
                  unlatch(2);
               }
              break;
          case 3 :
              if(readCurrentChannel3 > thresholdCh3){
                delatch(3);
              }else{
                unlatch(3);
              }
              break;

          default : break;
       }
}


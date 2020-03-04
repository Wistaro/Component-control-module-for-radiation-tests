/*
 * setup.c
 *
 *  Created on: 28 f�vr. 2020
 *      Author: WilliamR
 */


#include "driverlib.h"
#include "setup.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"
#include  "timer.h"
#include  "gpio.h"
#include "adc.h"
#include "uart.h"
#include "config.h"
#include "pwm.h"

void launchInitSequence(){

    //Initialization sequence
   printf("********************\n");
   printf("PROJET TILU2 - CNES\n");
   printf("********************\n");
   printf("Version  %s\n", VERSION);

   printf(" \n\nInitialisation de la clock et des interruptions.... ");
   initGlobal();
   printf("OK!\n");

   printf("Initialisation du module GPIO.... ");
   initGpio();
   printf("OK!\n");

   printf("Initialisation du module TimerB.... ");
   initTimerB();
   printf("OK!\n");

   printf("Initialisation du module ADC 12 bits.... ");
   initAdc();
   printf("OK!\n");

   printf("Initialisation du module UART.... ");
   initUart();
   printf("OK!\n");

   printf("Initialisation du module SPI.... ");
   initSpi();
   printf("OK!\n");

   printf("Initialisation du module PWM.... ");
   initPwm();
   printf("OK!\n");

   printf("Initialisation de la configuration.... ");
   resetConfig();
   printf("OK!\n");


   printf("\nTous les modules sont initialis�s!\n\n");
}

void initGlobal(){

    __bis_SR_register(GIE); //enable global interrupt

    setupDCO();

}


void setupDCO(void)
{

      /* Power settings */
      SetVCoreUp(1u);
      SetVCoreUp(2u);
      SetVCoreUp(3u);


      UCSCTL3 = SELREF__REFOCLK;    // select REFO as FLL source
      UCSCTL6 = XT1OFF | XT2OFF;    // turn off XT1 and XT2

      /* Initialize DCO to 25.00MHz */
      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL0 = 0x0000u;                        // Set lowest possible DCOx, MODx
      UCSCTL1 = DCORSEL_6;                      // Set RSELx for DCO = 50 MHz
      UCSCTL2 = 762u;                            // Set DCO Multiplier for 25MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (762 + 1) * 32768 = 25.00MHz
      UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32*32*25MHz/32768Hz = 781250 = MCLK cycles for DCO to settle
      __delay_cycles(781250u);


      /* Loop until XT1,XT2 & DCO fault flag is cleared */
      do
      {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }
      while (SFRIFG1&OFIFG);                    // Test oscillator fault flag


}

void SetVCoreUp (unsigned int level)
{
    // Open PMM registers for write access
    PMMCTL0_H = 0xA5;
    // Set SVS/SVM high side new level
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    // Set SVM low side to new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    // Wait till SVM is settled
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Clear already set flags
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    // Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;
    // Wait till new level reached
    if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
    // Set SVS/SVM low side to new level
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
    // Lock PMM registers for write access
    PMMCTL0_H = 0x00;
}




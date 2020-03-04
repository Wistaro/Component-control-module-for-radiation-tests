#include "driverlib.h"
#include "timer.h"
#include <stdio.h>
#include "gpio.h"
#include "config.h"


void initTimerB(){
     GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    //Start timer in continuous mode sourced by SMCLK
     Timer_B_clearTimerInterrupt(TIMER_B0_BASE);

         Timer_B_initUpModeParam param = {0};
         param.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
         param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
         param.timerPeriod = TIMER_PERIOD;
         param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
         param.captureCompareInterruptEnable_CCR0_CCIE =
             TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
         param.timerClear = TIMER_B_DO_CLEAR;
         param.startTimer = true;
         Timer_B_initUpMode(TIMER_B0_BASE, &param);

         flagTimer100us = 0;
         flagTimer1ms = 0;
         cpt_1ms = 0;

         cpt_delatch_ch1 = 0;
         cpt_delatch_ch2 = 0;
         cpt_delatch_ch3 = 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERB0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMERB0_VECTOR)))
#endif
void TIMERB0_ISR (void)
{
    //Toggle P1.0 using exclusive-OR

    if(isChannel1Active == 0x01){
        if( stateChannel1 == STATE_HOLD){
            if(cpt_delatch_ch1 <= 0.5*(tholdCh1/100)){
                cpt_delatch_ch1 = cpt_delatch_ch1 + 1;
                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
                unlatch(1);
            }else{
                cpt_delatch_ch1 = 0;
                stateChannel1 = STATE_OFF;
                delatch(1);
                GPIO_setOutputLowOnPin(PIN_DEBUG_OUT_1);
            }
        }else if(stateChannel1 == STATE_OFF){
            if(cpt_delatch_ch1 <= 2*(toffCh1/100)){
                        cpt_delatch_ch1 = cpt_delatch_ch1 + 1;
                        delatch(1);
             }else{
                        cpt_delatch_ch1 = 0;
                        stateChannel1 = STATE_IDLE;
                        unlatch(1);
                    }
        }
    }

    if(isChannel2Active == 0x01){

        if( stateChannel2 == STATE_HOLD ){
            if(cpt_delatch_ch2 <= 0.5*(tholdCh2/100)){
                cpt_delatch_ch2 = cpt_delatch_ch2 + 1;
                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
                unlatch(2);
            }else{
                cpt_delatch_ch2 = 0;
                stateChannel2 = STATE_OFF;
                delatch(2);
                GPIO_setOutputLowOnPin(PIN_DEBUG_OUT_1);
            }
        }else if(stateChannel2 == STATE_OFF){
            if(cpt_delatch_ch2 <= 2*(toffCh2/100)){
                        cpt_delatch_ch2 = cpt_delatch_ch2 + 1;
                        delatch(2);
             }else{
                        cpt_delatch_ch2 = 0;
                        stateChannel2 = STATE_IDLE;
                        unlatch(2);
                    }
        }
    }


    if(isChannel3Active == 0x01){

        if( stateChannel3 == STATE_HOLD ){
            if(cpt_delatch_ch3 <= 0.5*(tholdCh3/100)){
                cpt_delatch_ch3 = cpt_delatch_ch3 + 1;
                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
                unlatch(3);
            }else{
                cpt_delatch_ch3 = 0;
                stateChannel3 = STATE_OFF;
                delatch(3);
                GPIO_setOutputLowOnPin(PIN_DEBUG_OUT_1);
            }
        }else if(stateChannel3 == STATE_OFF){
            if(cpt_delatch_ch3 <= 2*(toffCh3/100)){
                        cpt_delatch_ch3 = cpt_delatch_ch3 + 1;
                        delatch(3);
             }else{
                        cpt_delatch_ch3 = 0;
                        stateChannel3 = STATE_IDLE;
                        unlatch(3);
                    }
        }

    }

    if(cpt_1ms <= 8){
        cpt_1ms++;
        flagTimer1ms = 0;
    }else{
        flagTimer1ms = 1;
        cpt_1ms = 0;
    }

    if(flagTimer100us == 0){
        flagTimer100us = 1;
    }else{
        flagTimer100us= 0;
    }


}

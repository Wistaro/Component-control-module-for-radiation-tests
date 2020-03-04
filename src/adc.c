/*
 * ADC1 : P6.0 (A0)
 * ADC2 : P6.1 (A1)
 * ADC3 : P6.3 (A3)
 *
 * WARNING: P6.2 IS GROUNDED!
 */
#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "config.h"
#include "timer.h"
#include "gpio.h"
#include "pwm.h"



void initAdc (void)
{

    //Enable A/D channel inputs
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN3 +  GPIO_PIN4 +  GPIO_PIN5
        );
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P7,
            GPIO_PIN0
            );

    //Initialize the ADC12_A Module
    /*
     * Base address of ADC12_A Module
     * Use internal ADC12_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC12_A_init(ADC12_A_BASE,
        ADC12_A_SAMPLEHOLDSOURCE_SC,
        ADC12_A_CLOCKSOURCE_ADC12OSC  ,
        ADC12_A_CLOCKDIVIDER_1
        );

    ADC12_A_enable(ADC12_A_BASE);

    /*
     * Base address of ADC12_A Module
     * For memory buffers 0-7 sample/hold for 256 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Enable Multiple Sampling
     */
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
        ADC12_A_CYCLEHOLD_512_CYCLES,
        ADC12_A_CYCLEHOLD_512_CYCLES,
        ADC12_A_MULTIPLESAMPLESENABLE);

    //Configure Memory Buffers
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 0-7
     * Map input A0-A7 to memory buffer 0
     * Vref+ = AVcc
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */


    //Voltage sensor #1
    ADC12_A_configureMemoryParam param0 = {0};
    param0.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    param0.inputSourceSelect = ADC12_A_INPUT_A0;
    param0.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param0.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param0.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param0);

    //Voltage sensor #2
    ADC12_A_configureMemoryParam param1 = {0};
    param1.memoryBufferControlIndex = ADC12_A_MEMORY_1;
    param1.inputSourceSelect = ADC12_A_INPUT_A1;
    param1.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param1.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param1.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param1);

    //Voltage sensor #3
    ADC12_A_configureMemoryParam param2 = {0};
    param2.memoryBufferControlIndex = ADC12_A_MEMORY_2;
    param2.inputSourceSelect = ADC12_A_INPUT_A2;
    param2.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param2.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param2.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param2);

    //Voltage sensor #4
    ADC12_A_configureMemoryParam param3 = {0};
    param3.memoryBufferControlIndex = ADC12_A_MEMORY_3;
    param3.inputSourceSelect = ADC12_A_INPUT_A3;
    param3.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param3.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param3.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param3);

    //Current sensor #1
    ADC12_A_configureMemoryParam param4 = {0};
    param4.memoryBufferControlIndex = ADC12_A_MEMORY_4;
    param4.inputSourceSelect = ADC12_A_INPUT_A4;
    param4.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param4.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param4.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param4);

    //current sensor #2
    ADC12_A_configureMemoryParam param5 = {0};
    param5.memoryBufferControlIndex = ADC12_A_MEMORY_5;
    param5.inputSourceSelect = ADC12_A_INPUT_A5;
    param5.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param5.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param5.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param5);

    //current sensor #3
    ADC12_A_configureMemoryParam param6 = {0};
    param6.memoryBufferControlIndex = ADC12_A_MEMORY_6;
    param6.inputSourceSelect = ADC12_A_INPUT_A12;
    param6.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param6.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param6.endOfSequence = ADC12_A_ENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param6);

    //Enable memory buffer 3 interrupt
    ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12IFG6);
    ADC12_A_enableInterrupt(ADC12_A_BASE, ADC12IE6);

    ADC12_A_startConversion(ADC12_A_BASE,
                            ADC12_A_MEMORY_0,
                            ADC12_A_REPEATED_SEQOFCHANNELS);

    flagDataReceivedOnAdc = 0x00;
    realVoltageChannel1 = 0;
    realVoltageChannel2 = 0;
    realVoltageChannel3 = 0;
    realCurrentChannel1 = 0;
    realCurrentChannel2 = 0;
    realCurrentChannel3 = 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR (void)
{
    static uint16_t index = 0;

    if(TRIG_IF6){

        //Voltage sensors
        A0results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_0);
        A1results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_1);
        A2results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_2);
        A3results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_3);

        //Current sensors
        A4results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_4);
        A5results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_5);
        A6results[index] = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_6);

        //printf("testdfv : %d\n",  A6results[0]);

        flagDataReceivedOnAdc = 0x01;

        index++;

        if (index == Num_of_Results){
            (index = 0);
        }

        //ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12IFG6);
    }
}

uint16_t getAdcVoltageResultFiltered(int outputId){
     uint16_t buffVal_voltage[Num_of_Results];
     uint16_t outputVal_voltage = 0;
     long localSum_voltage = 0;

     int i = 0;
     //get values array
    switch(outputId){
        case 1 : memcpy(buffVal_voltage, A0results, sizeof(buffVal_voltage)); break;
        case 2 : memcpy(buffVal_voltage, A1results, sizeof(buffVal_voltage)); break;
        case 3 : memcpy(buffVal_voltage, A3results, sizeof(buffVal_voltage)); break;
        default: break;
    }

    //compute average value

    for(i = 0; i<Num_of_Results; i++){
        localSum_voltage = localSum_voltage + buffVal_voltage[i];
    }

    outputVal_voltage = localSum_voltage / Num_of_Results;

    return outputVal_voltage;

}

uint16_t getAdcCurrentResultFiltered(int outputId){
     uint16_t buffVal_current[Num_of_Results];
     uint16_t outputVal_current = 0;
     long localSum_current = 0;

     int j = 0;
     //get values array
    switch(outputId){
        case 1 : memcpy(buffVal_current, A4results, sizeof(buffVal_current)); break;
        case 2 : memcpy(buffVal_current, A5results, sizeof(buffVal_current)); break;
        case 3 : memcpy(buffVal_current, A6results, sizeof(buffVal_current)); break;
        default: break;
    }

    //compute average value

    for(j = 0; j<Num_of_Results; j++){
        localSum_current = localSum_current + buffVal_current[j];
    }

    outputVal_current = localSum_current / Num_of_Results;

    return outputVal_current;

}

char voltageSensorListener(){

    if(flagDataReceivedOnAdc == 0x01){


    flagDataReceivedOnAdc = 0x00; //reset custom flag :)

    //Get raw results

    ucVoltageChannel1_vsensor = getAdcVoltageResultFiltered(1);
    ucVoltageChannel2_vsensor = getAdcVoltageResultFiltered(2);
   ucVoltageChannel3_vsensor = getAdcVoltageResultFiltered(3);

    //Convert data -> voltage (0 - 3.6V) for channel 1
    ucVoltageChannel1_vsensor = (ucVoltageChannel1_vsensor & 0xFFC)>>2;
    ucVoltageChannel1_vsensor = 30*ucVoltageChannel1_vsensor;
    ucVoltageChannel1_vsensor = ucVoltageChannel1_vsensor / 9.25;

    //Convert data -> voltage (0 - 3.6V) for channel 2
    ucVoltageChannel2_vsensor = (ucVoltageChannel2_vsensor & 0xFFC)>>2;
    ucVoltageChannel2_vsensor = 30*ucVoltageChannel2_vsensor;
    ucVoltageChannel2_vsensor = ucVoltageChannel2_vsensor / 9.25;

    //Convert data -> voltage (0 - 3.6V) for channel 3
    ucVoltageChannel3_vsensor = (ucVoltageChannel3_vsensor & 0xFFC)>>2;
    ucVoltageChannel3_vsensor = 30*ucVoltageChannel3_vsensor;
    ucVoltageChannel3_vsensor = ucVoltageChannel3_vsensor / 9.25;

    //Calculate the real voltage on each channel
    realVoltageChannel1 = ucVoltageChannel1_vsensor*1.95;
    realVoltageChannel2 = ucVoltageChannel2_vsensor*5.5;
    realVoltageChannel3 = ucVoltageChannel3_vsensor*3.25;

    voltageStabilize();

    }


    return 1;

}

char currentSensorListenerV2(){

    flagDataReceivedOnAdc = 0x00; //reset custom flag :)

    //Get raw results
    ucVoltageChannel1_isensor = getAdcCurrentResultFiltered(1);
    ucVoltageChannel2_isensor = getAdcCurrentResultFiltered(2);
    ucVoltageChannel3_isensor = getAdcCurrentResultFiltered(3);


    //Calculate the real current on each channel
    realCurrentChannel1 = ucVoltageChannel1_isensor * 0.29;

    realCurrentChannel2 = ucVoltageChannel2_isensor * 0.333;
    realCurrentChannel3 = ucVoltageChannel3_isensor;

    currentDetectLachup();

    return 1;

}


uint32_t getLineVoltageOnChannel(int channelId){

    uint32_t output = 0;

    switch(channelId){
        case 1 : output = realVoltageChannel1; break;
        case 2 : output = realVoltageChannel2; break;
        case 3 : output = realVoltageChannel3; break;
        default: break;
    }

    return output;
}

uint32_t getLineCurrentOnChannel(int channelId){

    uint32_t output = 0;

    switch(channelId){
        case 1 : output = realCurrentChannel1; break;
        case 2 : output = realCurrentChannel2; break;
        case 3 : output = realCurrentChannel3; break;
        default: break;
    }

    return output;
}

void voltageStabilize(){
    if(isChannel1Active == 0x01 && flagTimer1ms == 0x01){

        if(getLineVoltageOnChannel(1) != targetVoltCh1){
            setRealVoltageOnChannel(1, targetVoltCh1);
        }
    }

    if(isChannel2Active == 0x01 && flagTimer1ms == 0x01){


        if(getLineVoltageOnChannel(2) != targetVoltCh2){
            setRealVoltageOnChannel(2, targetVoltCh2);
         }
    }

    if(isChannel3Active == 0x01 && flagTimer1ms == 0x01){

        if(getLineVoltageOnChannel(3) != targetVoltCh3){
            setRealVoltageOnChannel(3, targetVoltCh3);
         }
    }
}

char currentDetectLachup(){

    if(isChannel1Active == 0x01){
            if(realCurrentChannel1 >= thresholdCh1 && stateChannel1 == STATE_IDLE){
                //delatch(1); //delatch channel 1;

                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);

                if(tholdCh1 <= 500){
                    stateChannel1 = STATE_OFF;
                    delatch(1);
                    GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
                }else{
                    stateChannel1 = STATE_HOLD;
                }

                //printf("delatch\n");

            }
     }

    if(isChannel2Active == 0x01){
            if(realCurrentChannel2 >= thresholdCh2 && stateChannel2 == STATE_IDLE){
                //delatch(2); //delatch channel 1;
                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);

                if(tholdCh2 <= 500){
                    stateChannel2 = STATE_OFF;
                    delatch(2);
                    GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
                }else{
                    stateChannel2 = STATE_HOLD;
                }
            }
    }

    if(isChannel3Active == 0x01){
            if(realCurrentChannel3 >= thresholdCh3 && stateChannel3 == STATE_IDLE){
                //delatch(3); //delatch channel 1;
                stateChannel3 = STATE_HOLD;
                GPIO_setOutputHighOnPin(PIN_DEBUG_OUT_1);
            }
        }


    return 1;
}

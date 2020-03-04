/*
 * adc.h
 *
 *  Created on: 18 févr. 2020
 *      Author: WilliamR
 */

#ifndef ADC_H_
#define ADC_H_

void initAdc();

//Voltage sensors functions definition
uint16_t getAdcVoltageResultFiltered(int outputId);
uint32_t getLineVoltageOnChannel(int channelId);
uint16_t getAdcVoltageResultFiltered(int outputId);
char voltageSensorListener();
void voltageStabilize();

//Current sensors functions definition
uint16_t getAdcCurrentResultFiltered(int outputId);
uint32_t getLineCurrentOnChannel(int channelId);
uint16_t getAdcCurrentResultFiltered(int outputId);
char currentSensorListenerV2();
char currentDetectLachup();

/*Interrupt trigger macro from interrupt adc vector*/

#define TRIG_OVERFLOW __even_in_range(ADC12IV,34) == 2
#define TRIG_IF0 __even_in_range(ADC12IV,34) == 6
#define TRIG_IF1 __even_in_range(ADC12IV,34) == 8
#define TRIG_IF2 __even_in_range(ADC12IV,34) == 10
#define TRIG_IF3 __even_in_range(ADC12IV,34) == 12
#define TRIG_IF4 __even_in_range(ADC12IV,34) == 14
#define TRIG_IF5 __even_in_range(ADC12IV,34) == 16
#define TRIG_IF6 __even_in_range(ADC12IV,34) == 18

#define DEVIDE_FACTOR_VOLTAGE_UC 1137
#define   Num_of_Results   3

//voltage sensors
uint16_t A0results[Num_of_Results];
uint16_t A1results[Num_of_Results];
uint16_t A2results[Num_of_Results];
uint16_t A3results[Num_of_Results];

//current sensors
uint16_t A4results[Num_of_Results];
uint16_t A5results[Num_of_Results];
uint16_t A6results[Num_of_Results];

/*Voltage sensor datas*/
uint32_t ucVoltageChannel1_vsensor;
uint32_t ucVoltageChannel2_vsensor;
uint32_t ucVoltageChannel3_vsensor;

uint32_t realVoltageChannel1;
uint32_t realVoltageChannel2;
uint32_t realVoltageChannel3;

/*Current sensors datas*/
uint32_t ucVoltageChannel1_isensor;
uint32_t ucVoltageChannel2_isensor;
uint32_t ucVoltageChannel3_isensor;

uint32_t realCurrentChannel1;
uint32_t realCurrentChannel2;
uint32_t realCurrentChannel3;

char flagDataReceivedOnAdc;

#endif /* ADC_H_ */

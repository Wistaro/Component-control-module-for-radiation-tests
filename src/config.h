/*
 * config.h
 *
 *  Created on: 21 févr. 2020
 *      Author: WilliamR
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION "0.1"
#define AUTHOR "WilliamR"

/*Config for delatch*/
#define THOLD_MAX 100000 //us : 100ms
#define THOLD_MIN 500 //us : 0.5ms
#define TOFF_MAX 15000000 //us : 15s
#define TOFF_MIN 10000 //us : 10ms

/*DEFAULT CONFIG*/

//Global
#define DEFAULT_THOLD THOLD_MIN
#define DEFAULT_TOFF TOFF_MIN

//Channel #1
#define MAX_VOLTAGE_CH1 5500 //en mv
#define MIN_VOLTAGE_CH1 3200 //en mv
#define DEFAULT_THRESHOLD_CH1 500 //en mA
#define MAX_CURRENT_CH1 1000

//Channel #2
#define MAX_VOLTAGE_CH2 17000 //en mv
#define MIN_VOLTAGE_CH2 5500 //en mv
#define DEFAULT_THRESHOLD_CH2 500 //en mA
#define MAX_CURRENT_CH2 1000 //mA

//Channel #3
#define MAX_VOLTAGE_CH3 3200 //en mv
#define MIN_VOLTAGE_CH3 800 //en mv
#define DEFAULT_THRESHOLD_CH3 1500 //en mA
#define MAX_CURRENT_CH3 3000 //mA

/*variables*/

uint32_t tholdCh1 ;
uint32_t tholdCh2 ;
uint32_t tholdCh3 ;

uint32_t toffCh1 ;
uint32_t toffCh2 ;
uint32_t toffCh3 ;

uint32_t targetVoltCh1 ;
uint32_t targetVoltCh2 ;
uint32_t targetVoltCh3 ;

uint32_t thresholdCh1 ;
uint32_t thresholdCh2 ;
uint32_t thresholdCh3 ;

char isChannel1Active;
char isChannel2Active;
char isChannel3Active;

const char* version;
const char* author;

/*functions*/

void resetConfig();

#endif /* CONFIG_H_ */

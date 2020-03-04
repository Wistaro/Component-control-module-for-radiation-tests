/*
 * spi.h
 *
 *  Created on: 23 févr. 2020
 *      Author: WilliamR
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_CLK 1000000
#define TRIG_IF_SPI_B __even_in_range(UCB0IV,4)==2

#define       PIN_MOSI              GPIO_PIN0
#define       PIN_MISO              GPIO_PIN1
#define       PIN_SCLK               GPIO_PIN2
#define       SPI_PERIPHERAL    GPIO_PORT_P3,PIN_MISO+PIN_MOSI+PIN_SCLK

#define LOW_CURRENT_CHANNEL_FACTOR 61 //multiply factor to get the real current value (in uA) -- see Paul Lavau
#define HIGH_CURRENT_CHANNEL_FACTOR 150 //multiply factor to get the real current value (in uA) -- see Paul Lavau

void initSpi();
char currentSensorListener();
void computeCurrentValues();
void checkThreshold();

uint8_t receiveDataOnSpi;
uint8_t activeChannel;

uint16_t rawDataFromSpiChannel1; //raw data that the current sensor send
uint16_t rawDataFromSpiChannel2; //raw data that the current sensor send
uint16_t rawDataFromSpiChannel3; //raw data that the current sensor send

uint32_t readCurrentChannel1; //real current value on channel 1 (in mA)
uint32_t readCurrentChannel2; //real current value on channel 2 (in mA)
uint32_t readCurrentChannel3; //real current value on channel 3 (in mA)

char flagDataReceivedOnSpi;
char index2readSpi;

char allBitRead;



#endif /* SPI_H_ */

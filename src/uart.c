/*
 * uart.c
 *
 *  Created on: 22 févr. 2020
 *      Author: WilliamR
 */
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "uart.h"
#include "gpio.h"
#include "config.h"
#include "adc.h"
#include "setup.h"

//1000A = 3.2v //VOIE 1
//3.3V => 1.1


uint8_t stringFromUart1[100];

void initUart(){

    flagDataReceivedOnUart1 = 0x00;
    receivedDataOnUart1 = 0x00;

    GPIO_setAsPeripheralModuleFunctionInputPin(UCA1TXD_PIN);
        GPIO_setAsPeripheralModuleFunctionInputPin(UCA1RXD_PIN);

        // From Table 36-4 in the family user's manual where UCOS16 = 0 and
        //            baudrate = 9600
        //            clock freq = 1.048MHz
        // UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
        USCI_A_UART_initParam param = {0};
        param.selectClockSource = USCI_A_UART_CLOCKSOURCE_ACLK;
        param.clockPrescalar = 6;                                             // UCBRx
        param.firstModReg = 0;                                                  // UCBRFx
        param.secondModReg = 7;                                                 // UCBRSx
        param.parity = USCI_A_UART_NO_PARITY;
        param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
        param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
        param.uartMode = USCI_A_UART_MODE;
        param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;     // UCOS16 = 0

        if(STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &param))
        {
            GPIO_setOutputHighOnPin(PIN_LED2);
            return;
        }

        //Enable UART module for operation
        USCI_A_UART_enable(USCI_A1_BASE);

        //Enable Receive Interrupt
        USCI_A_UART_clearInterrupt(USCI_A1_BASE,
                                   USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(USCI_A1_BASE,
                                    USCI_A_UART_RECEIVE_INTERRUPT);

        __enable_interrupt();

        indexStringUart = 0;

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR(void)
{
        if(TRIG_IF_UART1){
            receivedDataOnUart1 = USCI_A_UART_receiveData(USCI_A1_BASE);
            flagDataReceivedOnUart1 = 0x01;

        }
      //USCI_A_UART_clearInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG );
}

char sendByteUart1(uint8_t input){
    USCI_A_UART_transmitData(USCI_A1_BASE, input);
    while(USCI_A_UART_queryStatusFlags(USCI_A1_BASE,USCI_A_UART_BUSY));
    return 1;
}

char sendStringUart1(uint8_t *strInput){
        int i = 0;
       int taille_tab = strlen((const char*)strInput);

       for(i = 0; i<= taille_tab; i++){
           sendByteUart1(strInput[i]);
       }

   return 1;
}
void cleanTab(){
    int i = 0;

    for(i = 0; i<100; i++){
        stringFromUart1[i] = 0;
    }
}

char interfaceListener(){
    if(flagDataReceivedOnUart1 == 0x01){
        flagDataReceivedOnUart1 = 0x00;

        if(receivedDataOnUart1 == NL ||  receivedDataOnUart1 == CR){
            stringFromUart1[indexStringUart] = 0;
            indexStringUart = 0;

            handleCommandFromInterface();
            cleanTab();

         }else{
             stringFromUart1[indexStringUart] = receivedDataOnUart1;
             indexStringUart = indexStringUart + 1;
         }

     }

   return 1;
}

char handleCommandFromInterface(){

    char* globalCommand = (char*)stringFromUart1;
    char* channel;
    char* argument;
    char* value2set;
    int i = 0;
    char buffer_conv[100];

    if(strstr(globalCommand, COMMAND_GET) != NULL) {

            int init_size = strlen(globalCommand);
            char delim[] = " ";

            char *extactData = strtok(globalCommand, delim);

            while(extactData != NULL)
            {
                i = i + 1;
                extactData = strtok(NULL, delim);

                if(i == 1){
                    argument = extactData;
                }else if(i == 2){
                    channel = extactData;
                }
            }
            if(i >= 2){
                if(strstr(argument, "thold") != NULL){
                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("thold1", tholdCh1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("thold2", tholdCh2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("thold3", tholdCh3);
                    }
                }else if(strstr(argument, "i") != NULL){
                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("i1", realCurrentChannel1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("i2", realCurrentChannel2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("i3", realCurrentChannel3);
                    }
                }else if(strstr(argument, "v") != NULL){
                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("v1", realVoltageChannel1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("v2", realVoltageChannel2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("v3", realVoltageChannel3);
                    }
                }else if(strstr(argument, "toff") != NULL){

                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("toff1", toffCh1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("toff2", toffCh2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("toff3", toffCh3);
                    }

                }else if(strstr(argument, "thrd") != NULL){
                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("thrd1", thresholdCh1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("thrd2", thresholdCh2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("thrd3", thresholdCh3);
                    }

                }else if(strstr(argument, "chon") != NULL){
                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("chon1", isChannel1Active);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("chon2", isChannel2Active);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("chon3", isChannel3Active);
                    }

                }else if(strstr(argument, "tgt") != NULL){

                    if(strstr(channel, "1") != NULL){

                        sendFormatedData("tgtv1", targetVoltCh1);

                    }else if(strstr(channel, "2") != NULL){

                        sendFormatedData("tgtv2", targetVoltCh2);

                    }else if(strstr(channel, "3") != NULL){

                        sendFormatedData("tgtv2", targetVoltCh3);
                    }
                }

            }

            free(extactData);

    }else if(strstr(globalCommand, COMMAND_SET) != NULL){

        int init_size = strlen(globalCommand);
        char delim[] = " ";

        char *extactData = strtok(globalCommand, delim);

        while(extactData != NULL){
            i = i + 1;
            extactData = strtok(NULL, delim);

            if(i == 1){
                argument = extactData;
            }else if(i == 2){
                channel = extactData;
            }else if(i == 3){
                value2set = extactData;
            }
        }

        if(i==4){

            uint32_t setValue = atol((const char *)value2set);

            if(strstr(argument, "thold") != NULL){
                  if(strstr(channel, "1") != NULL){

                      tholdCh1 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "2") != NULL){

                      tholdCh2 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "3") != NULL){

                      tholdCh3 = setValue;
                      sendStringUart1("Done.\n");
                  }
             }else if(strstr(argument, "toff") != NULL){

                  if(strstr(channel, "1") != NULL){

                      toffCh1 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "2") != NULL){

                      toffCh2 = setValue;

                  }else if(strstr(channel, "3") != NULL){

                      toffCh3 = setValue;
                      sendStringUart1("Done.\n");
                  }

              }else if(strstr(argument, "thrd") != NULL){
                  if(strstr(channel, "1") != NULL){

                      thresholdCh1 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "2") != NULL){

                      thresholdCh2 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "3") != NULL){

                      thresholdCh3 = setValue;
                      sendStringUart1("Done.\n");
                  }

              }else if(strstr(argument, "chon") != NULL){
                  if(strstr(channel, "1") != NULL){

                      isChannel1Active = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "2") != NULL){

                      isChannel2Active = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "3") != NULL){

                      isChannel3Active = setValue;
                      sendStringUart1("Done.\n");
                  }

              }else if(strstr(argument, "tgt") != NULL){

                  if(strstr(channel, "1") != NULL){

                      targetVoltCh1 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "2") != NULL){

                      targetVoltCh2 = setValue;
                      sendStringUart1("Done.\n");

                  }else if(strstr(channel, "3") != NULL){

                      targetVoltCh3 = setValue;
                      sendStringUart1("Done.\n");
                  }
              }


        }


    }else if(strstr(globalCommand, COMMAND_RESET) != NULL){
        sendStringUart1("Reinitialisation en cours...\n");

        launchInitSequence();
        GPIO_setOutputHighOnPin(PIN_DELATCH_CH1); //enable load on channel 1
            GPIO_setOutputHighOnPin(PIN_DELATCH_CH2); // enable load on channel 2
            GPIO_setOutputHighOnPin(PIN_DELATCH_CH3); // enable load on
        sendStringUart1("Programme reinitialise\n");

    }else if(strstr(globalCommand, COMMAND_DELATCH) != NULL){

        GPIO_setOutputLowOnPin(PIN_DELATCH_CH1); //enable load on channel 1
        GPIO_setOutputLowOnPin(PIN_DELATCH_CH2); // enable load on channel 2
        GPIO_setOutputLowOnPin(PIN_DELATCH_CH3); // enable load on
        //sendStringUart1("Delatched\n");

    }else if(strstr(globalCommand, COMMAND_UNLATCH) != NULL){

        GPIO_setOutputHighOnPin(PIN_DELATCH_CH1); //enable load on channel 1
        GPIO_setOutputHighOnPin(PIN_DELATCH_CH2); // enable load on channel 2
        GPIO_setOutputHighOnPin(PIN_DELATCH_CH3); // enable load on
        //sendStringUart1("Unlatched\n");

    }

    memset(channel, 0, sizeof(channel));
    memset(argument, 0, sizeof(argument));
    memset(globalCommand, 0, sizeof(globalCommand));
    memset(buffer_conv, 0, sizeof(buffer_conv));
    memset(value2set, 0, sizeof(value2set));

    return 1;

}
void sendFormatedData(char* prefix, char* value){
    char buffer[100];
    char buffer_val[100];

    itoa(value,buffer_val, 10);

    strcpy(buffer, prefix);
    strcat(buffer, "=");
    strcat(buffer, buffer_val);
    strcat(buffer, "\n");

    sendStringUart1(buffer);

    memset(buffer_val, 0, sizeof(buffer_val));
    memset(buffer, 0, sizeof(buffer));

    free(buffer_val);
    free(buffer);

}
char** str_split( char* str, char delim, int* numSplits )
{
    char** ret;
    int retLen;
    char* c;

    if ( ( str == NULL ) ||
        ( delim == '\0' ) )
    {
        /* Either of those will cause problems */
        ret = NULL;
        retLen = -1;
    }
    else
    {
        retLen = 0;
        c = str;

        /* Pre-calculate number of elements */
        do
        {
            if ( *c == delim )
            {
                retLen++;
            }

            c++;
        } while ( *c != '\0' );

        ret = malloc( ( retLen + 1 ) * sizeof( *ret ) );
        ret[retLen] = NULL;

        c = str;
        retLen = 1;
        ret[0] = str;

        do
        {
            if ( *c == delim )
            {
                ret[retLen++] = &c[1];
                *c = '\0';
            }

            c++;
        } while ( *c != '\0' );
    }

    if ( numSplits != NULL )
    {
        *numSplits = retLen;
    }

    memset(c, 0, sizeof(c));
    free(c);
    return ret;
}

void itoa(unsigned int value, char* result, int base){
    // check that the base if valid
        if (base < 2 || base > 36) { *result = '\0';}
        char* ptr = result;
        char* ptr1 = result;
        char* tmp_char;
        int tmp_value;

    do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
    }

}

/*
 * comAPI.h
 *
 *  Created on: Dec 8, 2023
 *      Author: zone107
 */

#ifndef INC_COMAPI_H_
#define INC_COMAPI_H_

#include "stm32f4xx_hal.h"

uint8_t syscodeTransmitBuffer[2];
uint8_t syscodeReceiveBuffer[2];
char payloadBuffer[256];
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;


void transmitSyscodeForward(uint8_t* syscodeToTransmit);
void transmitSyscodeBackward(uint8_t* syscodeToTransmit);
void receiveSyscodeFromNext(uint8_t* syscodeToReceive);
void receiveSyscodeFromPrevious(uint8_t* syscodeToReceive);
void transmitPayloadForward(char* payloadToTransmit,uint8_t payloadSize);
void transmitPayloadBackward(char* payloadToTransmit,uint8_t payloadSize);
void receivePayloadFromNext(char* payloadToReceive,uint8_t payloadSize);
void receivePayloadFromPrevious(char* payloadToReceive,uint8_t payloadSize);
void syscodeBufferCoder(uint8_t syscode,uint8_t payloadSize);
void syscodeBufferDecoder(uint8_t* syscodeBuffer,uint8_t* syscode,uint8_t* payloadSize);

void transmitMessageToPc(char* message,uint8_t messageSize);
void receiveMessageFromPc(char* message,uint8_t messageSize);

#endif /* INC_COMAPI_H_ */

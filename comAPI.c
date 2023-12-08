/*
 * comAPI.c
 *
 *  Created on: Dec 8, 2023
 *      Author: zone107
 */

#include "comAPI.h"

void transmitSyscodeForward(uint8_t* syscodeToTransmit){
	HAL_UART_Transmit(&huart3,syscodeToTransmit,2,1000);
}
void transmitSyscodeBackward(uint8_t* syscodeToTransmit){
	HAL_UART_Transmit(&huart1,syscodeToTransmit,2,1000);
}
void receiveSyscodeFromNext(uint8_t* syscodeToReceive){
	HAL_UART_Receive(&huart3,syscodeToReceive,2,1000);
}
void receiveSyscodeFromPrevious(uint8_t* syscodeToReceive){
	HAL_UART_Receive(&huart1,syscodeToReceive,2,1000);
}
void transmitPayloadForward(char* payloadToTransmit,uint8_t payloadSize){
	HAL_UART_Transmit(&huart3,payloadToTransmit,payloadSize,1000);
}
void transmitPayloadBackward(char* payloadToTransmit,uint8_t payloadSize){
	HAL_UART_Transmit(&huart1,payloadToTransmit,payloadSize,1000);
}
void receivePayloadFromNext(char* payloadToReceive,uint8_t payloadSize){
	HAL_UART_Receive(&huart3,payloadToReceive,payloadSize,1000);
}
void receivePayloadFromPrevious(char* payloadToReceive,uint8_t payloadSize){
	HAL_UART_Receive(&huart1,payloadToReceive,payloadSize,1000);
}
void syscodeBufferCoder(uint8_t syscode,uint8_t payloadSize){
	syscodeTransmitBuffer[0]=syscode;
	syscodeTransmitBuffer[1]=payloadSize;
}
void syscodeBufferDecoder(uint8_t* syscodeBuffer,uint8_t* syscode,uint8_t* payloadSize){
	*syscode=syscodeBuffer[0];
	*payloadSize=syscodeBuffer[1];
}



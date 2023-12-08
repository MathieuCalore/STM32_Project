/*
 * syscodeHandler.c
 *
 *  Created on: Dec 8, 2023
 *      Author: zone107
 */

#include "syscodeHandler.h"
#include "comAPI.h"

void treatSyscode(uint8_t syscode, uint8_t payloadSize,char* payloadBuffer){
		swicth (syscode) {

			case HANDSHAKE:
				flags->handshakeFlag=1;
				break;
			case ACK:
				flags->ackFlag=1;
				break;
			case RACK:
				flags->rackFlag=1;
				break;
			case RESET:

				break;
			case NACK:

				break;
			case KEY:

				break;
			case MESSAGE:
				if(flags->cardPos==MIDDLE){
					syscodeBufferCoder(MESSAGE,payloadSize);
					transmitSyscodeForward(syscodeTransmitBuffer);
					transmitPayloadForward(payloadBuffer,payloadSize);
				}else if(flags->cardPos==OUT){
					transmitMessageToPc(payloadBuffer,payloadSize);
				}
				break;
			default:
				break;
		}


		}
}

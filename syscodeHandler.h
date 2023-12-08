/*
 * syscodeHandler.h
 *
 *  Created on: Dec 8, 2023
 *      Author: zone107
 */

#ifndef INC_SYSCODEHANDLER_H_
#define INC_SYSCODEHANDLER_H_

//syscodes declaration:

const uint8_t ALLGOOD = 0; //Syscode qui n’enclenche pas de traitement particulier (ne doit pas être utilisé), pas de payload.
const uint8_t HANDSHAKE = 1; //(réservé à l’init) Demande d’acknowledgement à la carte suivant dans la chaîne, pas de payload.
const uint8_t ACK = 2; //Syscode de retour pour s’assurer de la bonne réception du message, pas de payload.
const uint8_t RACK = 3; //(réservé à l’init) Syscode de retour pour s’assurer que l’acknowledgement a été bien reçu, pas de payload.
const uint8_t RESET = 4; //Syscode forçant une séquence d’initialisation de la chaîne, pas de payload.
const uint8_t NACK = 5; //Syscode indiquant qu’un problème a eu lieu, le payload est un  message d'erreur codé sur 1 octet.
const uint8_t KEY = 6; //Syscode qui permet de préparer la prochaine carte à recevoir une clé d'encryptage, le payload est la clé de 128 octets?
const uint8_t MESSAGE = 7; //Syscode indiquant que le message va être transmis, le payload est le message (de taille max 255 octets).

//card state declaration
const uint8_t UNDEFINED = 0; //correspond à une carte non initialisée
const uint8_t IN = 1; //correspond à la première carte de la chaîne.
const uint8_t OUT = 2; //correspond à la dernière carte de la chaîne.
const uint8_t MIDDLE = 3; //correspond à une carte au millieu de la chaîne.

//flags declaration
typedef struct flagList flagList;
struct flagList{
	uint8_t handshakeFlag=0;
	uint8_t ackFlag=0;
	uint8_t rackFlag=0;
	uint8_t errorCode=0;
	uint8_t cardPos=UNDEFINED;
};

flagList flags;

void treatSyscode(uint8_t syscode, uint8_t payloadSize,char* payloadBuffer);

#endif /* INC_SYSCODEHANDLER_H_ */

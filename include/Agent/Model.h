/*
 * Model.c - Agente Baseado em Modelos
 *
 *  Created on: 2 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_AGENT_MODEL_C_
#define INCLUDE_AGENT_MODEL_C_

#include "Agent.h"

typedef struct __modelPKG__ {
	trash* foundedTrash; // Guarda a localização dos lixos que encontrou mas não pode carregar

} ModelPKG; // Pacote com o modelo do mundo. Fica no espaço reservado do agente

#ifdef COLOR_OUTPUT
#include <Color.h>
#else
#include <UTF_char.h>
#define PERSON_ICON 	"M"
#endif

Agent* newModelAgent();

int _runMA(Agent* aG, trash** lixos);

void _runMALeft(Agent* aG);

void _runMARight(Agent* aG);

void _runMAUp(Agent* aG);

void _runMADown(Agent* aG);

void _runMARandom(Agent* aG);

void _holdMA(Agent* aG, trash** trList);

void _releaseMA(Agent* aG, trash** trList);

void _drawMA(Agent* aG);

#endif /* INCLUDE_AGENT_MODEL_C_ */

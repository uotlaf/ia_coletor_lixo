/*
 * Model.h
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_MODEL_H_
#define INCLUDE_MODEL_H_

#include "Agent.h"
#include "Agent/Simple.h"
#define MODEL_AGENT_NAME "Modelos"

// Agente baseado em modelos
// É o agente simples que procura e guarda a localização de todos os que encontrar no caminho.
// Quando volta para a pesquisa reversa, primeiro verifica se tá faltando pegar algum dos que ele viu 

typedef struct __modelAgentReserved__ { // Item que fica na área de reservados
	AgentStatus status; 		// Guarda o status atual do afd
	Trash* foundedTrash;		// Lixos que foram encontrados conforme o agente andou no mapa
} ModelAgentReserved;


Agent* newModelAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

uint16_t _runMA(Agent** aG, Trash** lixos, Window** window);

#endif /* INCLUDE_MODEL_H_ */

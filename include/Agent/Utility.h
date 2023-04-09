/*
 * Utility.h
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_UTILITY_H_
#define INCLUDE_UTILITY_H_

#include "Agent.h"
#include "Agent/Simple.h"
#define UTILITY_AGENT_NAME "Utilidade"

// Agente baseado em utilidade
// Recebe o modelo do ambiente todo e faz uma lista de maiores valores
// Quando volta para a pesquisa reversa, primeiro verifica se tá faltando pegar algum dos que ele viu 

Agent* newUtilityAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

Trash* utilitySearchNextTrash(Agent** aG, Trash** trList);

uint16_t _runUA(Agent** aG, Trash** lixos, Window** window);

#endif /* INCLUDE_UTILITY_H_ */
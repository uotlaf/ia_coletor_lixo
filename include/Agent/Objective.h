/*
 * Objective.h
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_OBJECTIVE_H_
#define INCLUDE_OBJECTIVE_H_

#include "Agent.h"
#include "Agent/Simple.h"
#define OBJECTIVE_AGENT_NAME "objetivos"

// Agente baseado em objetivos
// Recebe o modelo do ambiente todo e faz uma lista de proximidade
// Quando volta para a pesquisa reversa, primeiro verifica se tá faltando pegar algum dos que ele viu 

Agent* newObjectiveAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

Trash* searchNextTrash(Agent** aG, Trash** trList);

uint16_t _runOA(Agent** aG, Trash** lixos, Window** window);

#endif /* INCLUDE_OBJECTIVE_H_ */
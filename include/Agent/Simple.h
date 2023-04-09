/*
 * Simple.h
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_SIMPLES_H_
#define INCLUDE_SIMPLES_H_

#include "Agent.h"
#define SIMPLE_AGENT_NAME "Reativo Simples"

// Agente Reativo Simples

Agent* newSimpleAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

uint16_t _runSA(Agent** aG, Trash** lixos, Window** window);

#endif /* INCLUDE_SIMPLES_H_ */

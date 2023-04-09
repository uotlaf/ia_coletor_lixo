/*
 * Simple.h
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_SIMPLES_H_
#define INCLUDE_SIMPLES_H_

#include "Agent.h"
#define SIMPLE_AGENT_NAME "Agente Reativo Simples"

Agent* newSimpleAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

uint16_t _runSA(Agent** aG, Trash** lixos, Window** window);

void _runSALeft(Agent** aG, Window** window);

void _runSARight(Agent** aG, Window** window);

void _runSAUp(Agent** aG, Window** window);

void _runSADown(Agent** aG, Window** window);

void _runSARandom(Agent** aG, Window** window);

void _holdSA(Agent** aG, Trash** trList, Window** window);

void _releaseSA(Agent** aG, Trash** trList, Window** window);

#endif /* INCLUDE_SIMPLES_H_ */

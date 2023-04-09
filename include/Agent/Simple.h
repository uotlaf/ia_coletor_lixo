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

// Agente simples

// SA_SEARCH_NORMAL:
// 1: Vai para (1,1)
// 2: Se Y é ímpar, vai para direita até larguraDoCampo-1
// 3: Se está na ultima linha, 4:. Se não, 6:
//   4: vai para direita até larguraDoCampo
//     5: SA_SEARCH_REVERSE
//   6: Anda 3 para baixo
// 7: Se Y é par, Volta para a esquerda até (1,currentY)
// 5: Anda 3 para baixo 

// SA_SEARCH_REVERSE:


typedef enum __SAStatus__ {
    SA_SEARCH_NORMAL,   // Procura de cima para baixo, esquerda para a direita
    SA_SEARCH_REVERSE,  // Procura de baixo para cima, direira para a esquerda
    SA_HOLD,            // Procura a lixeira
    SA_VIEWED           // Avistou algum item: Vai de encontro a ele
} SimpleAgentStatus;

Agent* newSimpleAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

uint16_t _runSA(Agent** aG, Trash** lixos, Window** window);

void followLocalization(Agent** aG, Window** window, uint16_t x, uint16_t y);

void _runSALeft(Agent** aG, Window** window);

void _runSARight(Agent** aG, Window** window);

void _runSAUp(Agent** aG, Window** window);

void _runSADown(Agent** aG, Window** window);

void _runSARandom(Agent** aG, Window** window);

void _holdSA(Agent** aG, Trash** trList, Window** window);

void _releaseSA(Agent** aG, Trash** trList, Window** window);

#endif /* INCLUDE_SIMPLES_H_ */

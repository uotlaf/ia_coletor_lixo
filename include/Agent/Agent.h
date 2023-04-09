/*
 * Agent.h
 *
 *  Created on: 2 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_AGENT_AGENT_H_
#define INCLUDE_AGENT_AGENT_H_
#include <Lixo.h>
#include <Terminal.h> // Window

#ifdef COLOR_OUTPUT
#include <Color.h>
#else
#include <UTF_char.h>
#define PERSON_ICON 	 UTF_PERSON
#endif
#define DEFAULT_AGENT_NAME "Default Agent"

typedef enum __AStatus__ {
    AGENT_STATUS_SEARCH_NORMAL,   // Procura de cima para baixo, esquerda para a direita
    AGENT_STATUS_SEARCH_REVERSE,  // Procura de baixo para cima, direira para a esquerda
    AGENT_STATUS_HOLD             // Procura a lixeira
} AgentStatus;


typedef struct __trashLocation__ {
	uint16_t x, y;
	Trash* founded;
} TrashLocation;

typedef struct __Agent__ {
	uint16_t x, y, score;
	uint16_t targetX, targetY;
	AgentStatus status;

	uint16_t oldx, oldy; // Usado para redesenhar o mapa
	Trash* holdItem;
	char* name;

	// Execução
	uint16_t (*run)(struct __Agent__** aG, Trash** tr, Window** window);
	void (*destroy) (struct __Agent__** aG);
	
	// Desenho
	void (*draw)(struct __Agent__** aG, Window** window);
	void (*drawFOV) (struct __Agent__** aG, Window** window);
	void (*clean) (struct __Agent__** aG, Window** window);
	
	// Movimentação
	void (*up) (struct __Agent__** aG, Window** window);
	void (*down) (struct __Agent__** aG, Window** window);
	void (*left) (struct __Agent__** aG, Window** window);
	void (*right) (struct __Agent__** aG, Window** window);
	void (*random) (struct __Agent__** aG, Window** window);
	void (*hold) (struct __Agent__** aG, Trash** trList, Window** window);
	void (*release) (struct __Agent__** aG, Trash** trList, Window** window);

	// Reservado para agentes específicos
	void* reserved; 
} Agent;

enum Actions {
	ACTION_UP, ACTION_DOWN,
	ACTION_LEFT, ACTION_RIGHT,
	ACTION_HOLD, ACTION_RELEASE,
	ACTION_NOOP, ACTION_RANDOM // Movimenta para um local aleatório
};



Agent* newAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore);

void _drawAgent(Agent** aG, Window** window);

void _drawFOV(Agent** aG, Window** window);

void _clean(Agent** aG, Window** window);

float findYinLine(uint16_t x, uint16_t Ax, uint16_t Ay, uint16_t Bx, uint16_t By);

TrashLocation* checkForTrashInFOV(Agent** aG, Trash** trList);

bool isOdd(int64_t number);

void _runAgentLeft(Agent** aG, Window** window);

void _runAgentRight(Agent** aG, Window** window);

void _runAgentUp(Agent** aG, Window** window);

void _runAgentDown(Agent** aG, Window** window);

void _runAgentRandom(Agent** aG, Window** window);

void _agentHold(Agent** aG, Trash** trList, Window** window);

void _agentRelease(Agent** aG, Trash** trList, Window** window);

void followLocalization(Agent** aG, Window** window, uint16_t x, uint16_t y);

void _destroy(Agent** aG);

#endif /* INCLUDE_AGENT_AGENT_H_ */

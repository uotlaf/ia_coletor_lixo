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
#define PERSON_ICON 	 "a"
#endif
#define DEFAULT_AGENT_NAME "Default Agent"

typedef struct __Agent__ {
	uint16_t x, y, score;
	uint16_t oldx, oldy; // Usado para redesenhar o mapa
	Trash* hold;
	void* package; // Reservado para agentes específicos
	uint16_t (*run)(struct __Agent__** aG, Trash** tr, Window** window);
	void (*draw)(struct __Agent__** aG, Window** window);
	void (*drawFOV) (struct __Agent__** aG, Window** window);
	void (*clean) (struct __Agent__** aG, Window** window);
	void (*destroy) (struct __Agent__** aG);
	char* name;
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

float findYinLine(int x, int Ax, int Ay, int Bx, int By);

void _destroy(Agent** aG);

#endif /* INCLUDE_AGENT_AGENT_H_ */

/*
 * Simple.c - Agente simples baseado em tabelas
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */


#include "Agent/Simple.h"
#include <stdio.h>		// sprintf()
#include <Terminal.h> 	// drawObject()
#include <stdlib.h>		// malloc()
#include <UTF_char.h>	// PERSON_ICON
#include <unistd.h>
#include <Ansi.h>		// ANSI
#include <Lixo.h>		// trash*

#define ACTION_UP 		_runSAUp(aG, window)
#define ACTION_DOWN 	_runSADown(aG, window)
#define ACTION_LEFT 	_runSALeft(aG, window)
#define ACTION_RIGHT 	_runSARight(aG, window)
#define ACTION_RANDOM	_runSARandom(aG, window)
#define ACTION_HOLD		_holdSA(aG, trList, window)
#define ACTION_RELEASE	_releaseSA(aG, trList, window)
#define ACTION_NOOP

/*
 * Agente simples
 *
 * Agente--------
 * 				|
 * 				|
 * --------------
 * |
 * |
 * --------------Lixeira
 */


Agent* newSimpleAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore) {
	Agent* temp = newAgent(initialX, initialY, initialScore);
	temp->run 	= (*_runSA);
	temp->name  = "Agente RSimples";
	return temp;
}

uint16_t _runSA(Agent** aG, Trash** trList, Window** window) {

	// Verifica qual ação fazer
	// Se está segurando um lixo = anda até a lixeira(ou tenta)
	if ((*aG)->hold) {
		// Se está na lixeira, solta
		if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) 	ACTION_RELEASE;

		// Se não, Descobre qual o próximo ponto para ir
		int cur_x = ((*aG)->x);
		if (cur_x == (*aG)->x) {
			if ((*aG)->x < (*window)->fieldColumns-1) 	cur_x = (*aG)->x+1;
			else				cur_x = (*aG)->x-1;
		}

		float nextY = findYinLine(cur_x, (*aG)->x, (*aG)->y, (*window)->fieldColumns-1, (*window)->fieldRows-1);
		// Vai para o próximo ponto
		if 		((*aG)->y < (int)nextY) ACTION_DOWN;
		else if ((*aG)->y > (int)nextY) ACTION_UP;
		else {
			if ((*aG)->x < (*window)->fieldColumns-1) 	ACTION_RIGHT;
			else				ACTION_LEFT;
		}

		return 1;
	}

	// Se não, verifica onde tá a próxima lixeira

	if 		(containsTrashin(*trList, (*aG)->x, (*aG)->y)) 		ACTION_HOLD;
	else if (containsTrashin(*trList, (*aG)->x, (*aG)->y-1))	ACTION_UP;
	else if (containsTrashin(*trList, (*aG)->x, (*aG)->y+1))	ACTION_DOWN;
	else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y))	ACTION_LEFT;
	else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y))	ACTION_RIGHT;

	else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y-1)) {	// UPRIGHT
		if (rand() % 2)	ACTION_UP;
		else 			ACTION_RIGHT;
	} else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y+1)) {	// DOWNRIGHT
		if (rand() % 2) ACTION_DOWN;
		else			ACTION_RIGHT;
	} else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y-1)) {	// UPLEFT
		if (rand() % 2) ACTION_UP;
		else			ACTION_LEFT;
	} else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y+1)) { 	// DOWNLEFT
		if (rand() % 2)	ACTION_DOWN;
		else			ACTION_LEFT;
	} else {
		ACTION_RANDOM;
	}

	return 0;
}

void _runSALeft(Agent** aG, Window** window) {
	if ((*aG)->x > 0) {
		(*aG)->oldx = (*aG)->x;
		(*aG)->x--;
	}
}

void _runSARight(Agent** aG, Window** window) {
	if ((*aG)->x < ((*window)->fieldColumns)-1) {
		(*aG)->oldx = (*aG)->x;
		(*aG)->x++;
	};
}

void _runSAUp(Agent** aG, Window** window) {
	if ((*aG)->y > 0) {
		(*aG)->oldy = (*aG)->y;
		(*aG)->y--;
	};
}

void _runSADown(Agent** aG, Window** window) {
	if ((*aG)->y < (*window)->fieldRows-1) {
		(*aG)->oldy = (*aG)->y;
		(*aG)->y++;
	} 
}

void _runSARandom(Agent** aG, Window** window) {
	int direcao = rand() % 4;
	switch (direcao)
	{
	case 0:
		ACTION_UP;
		return;
	case 1:
		ACTION_LEFT;
		return;
	case 2:
		ACTION_DOWN;
		return;
	default:
		ACTION_RIGHT;
		return;
	}
}

void _holdSA(Agent** aG, Trash** trList, Window** window) {
	// Segura o item que está em aG->x e aG->y
	if (!(*aG) || !trList) return;
	(*aG)->hold = containsTrashin(*trList, (*aG)->x, (*aG)->y);
	removeTrashfromList((*aG)->hold, trList);
}

void _releaseSA(Agent** aG, Trash** trList, Window** window) {
	// Solta o item que tá segurando e aumenta os pontos
	if (!(*aG)) return;
	if ((*aG)->hold) {
		(*aG)->score += (*aG)->hold->value;
		destroyTrashList(&((*aG)->hold));
		(*aG)->hold = NULL;
	}
}

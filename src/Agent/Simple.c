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
	temp->name  = SIMPLE_AGENT_NAME;

	// Guarda o estado atual do agente no pacote reservado
	temp->reserved = (void*)SA_SEARCH_NORMAL; // Algoritmo inicia pelo pesquisa normal
	return temp;
}

uint16_t _runSA(Agent** aG, Trash** trList, Window** window) {
	// Executa o algoritmo do Agente Reativo Simples
	if (!aG || !(*aG) || !window || (!*window)) return 0;
	TrashLocation* nextTrash = NULL; // Guarda o próximo lixo que o agente vai pegar / O primeiro que está no campo de visão dele
	static bool horMoviment = true;		// Quando cancelar o movimento horizontal e partir para o movimento vertical 
	static int16_t targetX = -1, targetY = -1;	// Usados para saber para onde ir 

	// Verifica qual ação fazer
	switch ((SimpleAgentStatus)((*aG)->reserved)) {
		case SA_SEARCH_NORMAL:	// Pesquisa normal - Vai de 1,1 até a lixeira pulando de 3 em 3 e em zigue-zague

			// Verifica se tem alguém nas redondezas
			nextTrash = checkForTrashInFOV(aG, trList);
			
			if (nextTrash) { // Achou alguém - Vai até ele
				if (nextTrash->x == (*aG)->x && nextTrash->y == (*aG)->y) { // Estou em cima dele: Pegar
					ACTION_HOLD;
					(*aG)->reserved = (void*)SA_HOLD;
					break;
				}
				followLocalization(aG, window, nextTrash->x, nextTrash->y);
				break;
			}

			// Se está em 0,0, vai para 1,1
			if ((*aG)->x == 0 && (*aG)->y == 0) {
				targetX = 1;
				targetY = 1;
			}

			// Se está na última linha, vai até a lixeira
			if ((*aG)->x == (*window)->fieldColumns - 1) {
				targetX = (*window)->fieldColumns-1;
				targetY = (*window)->fieldRows-1;
				horMoviment = false;
			}



			// Verifica se tem algum lugar para ir
			if (((*aG)->x != targetX || (*aG)->y != targetY) && (targetX != -1 && targetY != -1) ) {
				// Vai até o local
				followLocalization(aG, window, targetX, targetY);
				break;
			}


			// Verifica se está na lixeira. Se sim, estado = busca reversa
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) {
				targetX = -1;
				targetY = -1;
				horMoviment = true;
				(*aG)->reserved = (void*)SA_SEARCH_REVERSE;
				break;
			}

			// Se estiver no início da busca, vai para (1,(*window)->fieldRows-2)
			if ((*aG)->y == 1 && horMoviment) {
				targetX = (*aG)->x;
				targetY = (*window)->fieldRows-2;
				horMoviment = false;
				break;
			}

			// Se estiver em (x, (*window)->fieldRows-2), vai para (x+3, (*window)->fieldRows-2)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment == false) {
				targetX = (*aG)->x+3;
				targetY = (*window)->fieldRows-2;
				horMoviment = true;
				break;
			}

			// Verifica se está em (targetX, (*window)fieldRows-2). Se sim, vai até ((*aG)->x, 1)
			if ((*aG)->x == targetX && (*aG)->y == (*window)->fieldColumns-2 && horMoviment) {
				targetX = (*aG)->x;
				targetY = 1;
				horMoviment = false;
				break;
			}

			// Se está em (targetX, 1) e o movimento horizontal está desativado, vai para (x+3, 1)
			if ((*aG)->y == 1 && horMoviment == false) {
				targetX = (*aG)->x+3;
				targetY = 1;
				horMoviment = true;
				break;
			}

			break;
		case SA_SEARCH_REVERSE: // Vai da lixeira até 1,1 pulando de 3 em 3 e em zigue-zague

			// Verifica se tem alguém nas redondezas
			nextTrash = checkForTrashInFOV(aG, trList);
			
			if (nextTrash) { // Achou alguém - Vai até ele
				if (nextTrash->x == (*aG)->x && nextTrash->y == (*aG)->y) { // Estou em cima dele: Pegar
					ACTION_HOLD;
					(*aG)->reserved = (void*)SA_HOLD;
					break;
				}
				followLocalization(aG, window, nextTrash->x, nextTrash->y);
				break;
			}

			// Se está em lixeira, vai para lixeira-1,lixeira-1
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) {
				targetX = (*window)->fieldColumns-2;
				targetY = (*window)->fieldRows-2;
			}

			// Se está na primeira linha, vai até a 0,0
			if ((*aG)->x == 0) {
				targetX = 0;
				targetY = 0;
				horMoviment = false;
			}


			// Verifica se tem algum lugar para ir
			if (((*aG)->x != targetX || (*aG)->y != targetY) && (targetX != -1 && targetY != -1) ) {
				// Vai até o local
				followLocalization(aG, window, targetX, targetY);
				break;
			}

			// Verifica se está em (0,0). Se sim, estado = busca normal
			if ((*aG)->x == 0 && (*aG)->y == 0) {
				horMoviment = true;
				(*aG)->reserved = (void*)SA_SEARCH_NORMAL;
				break;
			}

			// Se estiver no início da busca, vai para (X,1)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment) {
				targetX = (*aG)->x;
				targetY = 1;
				horMoviment = false;
				break;
			}


			// Se estiver em (x, (*window)->fieldRows-2), vai para (x+3, (*window)->fieldRows-2)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment == false) {
				targetX = (*aG)->x-3;
				targetY = (*window)->fieldRows-2;
				horMoviment = true;
				break;
			}

			// Verifica se está em (targetX, 1). Se sim, vai até ((*aG)->x, (*window)->fieldRow-2)
			if ((*aG)->x == targetX && (*aG)->y == 1 && horMoviment) {
				targetX = (*aG)->x;
				targetY = (*window)->fieldRows-2;
				horMoviment = false;
				break;
			}

			// Se está em (targetX, 1) e o movimento horizontal está desativado, vai para (x+3, 1)
			if ((*aG)->y == 1 && horMoviment == false) {
				targetX = (*aG)->x-3;
				targetY = 1;
				horMoviment = true;
				break;
			}




			break;
		case SA_HOLD: // Segue até a lixeira e solta lá
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) {
				ACTION_RELEASE;
				// Volta para a pesquisa reversa
				(*aG)->reserved = (void*)SA_SEARCH_REVERSE;
				break;
			}
			// Caso não, vá até a lixeira
			followLocalization(aG, window, (*window)->fieldColumns-1, (*window)->fieldRows-1);
			break;
	}



	// Se está segurando um lixo = anda até a lixeira(ou tenta)
	// if ((*aG)->hold) {
		// // Se está na lixeira, solta
		// if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) 	ACTION_RELEASE;

		// // Se não, Descobre qual o próximo ponto para ir
		// int cur_x = ((*aG)->x);
		// if (cur_x == (*aG)->x) {
		// 	if ((*aG)->x < (*window)->fieldColumns-1) 	cur_x = (*aG)->x+1;
		// 	else				cur_x = (*aG)->x-1;
		// }

		// float nextY = findYinLine(cur_x, (*aG)->x, (*aG)->y, (*window)->fieldColumns-1, (*window)->fieldRows-1);
		// // Vai para o próximo ponto
		// if 		((*aG)->y < (int)nextY) ACTION_DOWN;
		// else if ((*aG)->y > (int)nextY) ACTION_UP;
		// else {
		// 	if ((*aG)->x < (*window)->fieldColumns-1) 	ACTION_RIGHT;
		// 	else				ACTION_LEFT;
		// }

	// 	return 1;
	// }

	// Se não, verifica onde tá a próxima lixeira

	// if 		(containsTrashin(*trList, (*aG)->x, (*aG)->y)) 		
	// else if (containsTrashin(*trList, (*aG)->x, (*aG)->y-1))	ACTION_UP;
	// else if (containsTrashin(*trList, (*aG)->x, (*aG)->y+1))	ACTION_DOWN;
	// else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y))	ACTION_LEFT;
	// else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y))	ACTION_RIGHT;

	// else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y-1)) {	// UPRIGHT
	// 	if (rand() % 2)	ACTION_UP;
	// 	else 			ACTION_RIGHT;
	// } else if (containsTrashin(*trList, (*aG)->x+1, (*aG)->y+1)) {	// DOWNRIGHT
	// 	if (rand() % 2) ACTION_DOWN;
	// 	else			ACTION_RIGHT;
	// } else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y-1)) {	// UPLEFT
	// 	if (rand() % 2) ACTION_UP;
	// 	else			ACTION_LEFT;
	// } else if (containsTrashin(*trList, (*aG)->x-1, (*aG)->y+1)) { 	// DOWNLEFT
	// 	if (rand() % 2)	ACTION_DOWN;
	// 	else			ACTION_LEFT;
	// } else {
	// 	ACTION_RANDOM;
	// }

	return 0;
}

void followLocalization(Agent** aG, Window** window, uint16_t x, uint16_t y) {
	// Acha a próxima ação que o agente deve fazer para chegar em x,y
	if(!aG || !(*aG) || !window || !(*window)) return;

	uint16_t nextX; // Guarda o próximo X que o agente deve andar
	float nextY;	//

	// Descobre a direção horizontal que o agente deve andar
	if ((*aG)->x < x) 	nextX = (*aG)->x+1;
	else				nextX = (*aG)->x-1;

	// Descobre a direção vertical que o agente deve andar
	nextY = findYinLine(nextX, (*aG)->x, (*aG)->y, x, y);
	
	// Vai para o próximo ponto
	// Primeiro segue o Y. Após isso, segue o X
	if 		((*aG)->y < (int)nextY) ACTION_DOWN;
	else if ((*aG)->y > (int)nextY) ACTION_UP;
	else {
		// Segue o X quando ag->Y == nextY
		if ((*aG)->x < nextX) 	ACTION_RIGHT;
		else				ACTION_LEFT;
	}
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

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

#define ACTION_HOLD		_agentHold(aG, trList, window)
#define ACTION_RELEASE	_agentRelease(aG, trList, window)
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
	temp->status = AGENT_STATUS_SEARCH_NORMAL;
	return temp;
}

uint16_t _runSA(Agent** aG, Trash** trList, Window** window) {
	// Executa o algoritmo do Agente Reativo Simples
	if (!aG || !(*aG) || !window || (!*window)) return 0;
	TrashLocation* nextTrash = NULL; // Guarda o próximo lixo que o agente vai pegar / O primeiro que está no campo de visão dele
	static bool horMoviment = true;		// Quando cancelar o movimento horizontal e partir para o movimento vertical 

	// Verifica qual ação fazer
	switch ((AgentStatus)((*aG)->status)) {
		case AGENT_STATUS_SEARCH_NORMAL:	// Pesquisa normal - Vai de 1,1 até a lixeira pulando de 3 em 3 e em zigue-zague

			// Verifica se tem alguém nas redondezas
			nextTrash = checkForTrashInFOV(aG, trList);
			
			if (nextTrash) { // Achou alguém - Vai até ele
				if (nextTrash->x == (*aG)->x && nextTrash->y == (*aG)->y) { // Estou em cima dele: Pegar
					ACTION_HOLD;
					(*aG)->status = AGENT_STATUS_HOLD;
					break;
				}
				followLocalization(aG, window, nextTrash->x, nextTrash->y);
				break;
			}

			// Se está em 0,0, vai para 1,1
			if ((*aG)->x == 0 && (*aG)->y == 0) {
				(*aG)->targetX = 1;
				(*aG)->targetY = 1;
			}

			// Se está na última linha, vai até a lixeira
			if ((*aG)->x == (*window)->fieldColumns - 1) {
				(*aG)->targetX = (*window)->fieldColumns-1;
				(*aG)->targetY = (*window)->fieldRows-1;
				horMoviment = false;
			}



			// Verifica se tem algum lugar para ir
			if (((*aG)->x != (*aG)->targetX || (*aG)->y != (*aG)->targetY) && ((*aG)->targetX != -1 && (*aG)->targetY != -1) ) {
				// Vai até o local
				followLocalization(aG, window, (*aG)->targetX, (*aG)->targetY);
				break;
			}


			// Verifica se está na lixeira. Se sim, estado = busca reversa
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) {
				(*aG)->targetX = -1;
				(*aG)->targetY = -1;
				horMoviment = true;
				(*aG)->status = AGENT_STATUS_SEARCH_REVERSE;
				break;
			}

			// Se estiver no início da busca, vai para (1,(*window)->fieldRows-2)
			if ((*aG)->y == 1 && horMoviment) {
				(*aG)->targetX = (*aG)->x;
				(*aG)->targetY = (*window)->fieldRows-2;
				horMoviment = false;
				break;
			}

			// Se estiver em (x, (*window)->fieldRows-2), vai para (x+3, (*window)->fieldRows-2)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment == false) {
				(*aG)->targetX = (*aG)->x+3;
				(*aG)->targetY = (*window)->fieldRows-2;
				horMoviment = true;
				break;
			}

			// Verifica se está em ((*aG)->targetX, (*window)fieldRows-2). Se sim, vai até ((*aG)->x, 1)
			if ((*aG)->x == (*aG)->targetX && (*aG)->y == (*window)->fieldRows-2 && horMoviment) {
				(*aG)->targetX = (*aG)->x;
				(*aG)->targetY = 1;
				horMoviment = false;
				break;
			}

			// Se está em ((*aG)->targetX, 1) e o movimento horizontal está desativado, vai para (x+3, 1)
			if ((*aG)->y == 1 && horMoviment == false) {
				(*aG)->targetX = (*aG)->x+3;
				(*aG)->targetY = 1;
				horMoviment = true;
				break;
			}

			break;
		case AGENT_STATUS_SEARCH_REVERSE: // Vai da lixeira até 1,1 pulando de 3 em 3 e em zigue-zague

			// Verifica se tem alguém nas redondezas
			nextTrash = checkForTrashInFOV(aG, trList);
			
			if (nextTrash) { // Achou alguém - Vai até ele
				if (nextTrash->x == (*aG)->x && nextTrash->y == (*aG)->y) { // Estou em cima dele: Pegar
					ACTION_HOLD;
					(*aG)->status = AGENT_STATUS_HOLD;
					break;
				}
				followLocalization(aG, window, nextTrash->x, nextTrash->y);
				break;
			}

			// Se está em lixeira e o alvo é a lixeira, vai para lixeira-1,lixeira-1
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1 && (*aG)->targetX == (*window)->fieldColumns-1 && (*aG)->targetY == (*window)->fieldRows-1) {
				(*aG)->targetX = (*window)->fieldColumns-2;
				(*aG)->targetY = (*window)->fieldRows-2;
			}

			// Se está na primeira linha, vai até a 0,0
			if ((*aG)->x == 0) {
				(*aG)->targetX = 0;
				(*aG)->targetY = 0;
				horMoviment = false;
			}


			// Verifica se tem algum lugar para ir
			if (((*aG)->x != (*aG)->targetX || (*aG)->y != (*aG)->targetY) && ((*aG)->targetX != -1 && (*aG)->targetY != -1) ) {
				// Vai até o local
				followLocalization(aG, window, (*aG)->targetX, (*aG)->targetY);
				break;
			}

			// Verifica se está em (0,0). Se sim, estado = busca normal
			if ((*aG)->x == 0 && (*aG)->y == 0) {
				horMoviment = true;
				(*aG)->status = AGENT_STATUS_SEARCH_NORMAL;
				break;
			}

			// Se estiver no início da busca, vai para (X,1)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment) {
				(*aG)->targetX = (*aG)->x;
				(*aG)->targetY = 1;
				horMoviment = false;
				break;
			}


			// Se estiver em (x, (*window)->fieldRows-2), vai para (x+3, (*window)->fieldRows-2)
			if ((*aG)->y == (*window)->fieldRows-2 && horMoviment == false) {
				(*aG)->targetX = (*aG)->x-3;
				(*aG)->targetY = (*window)->fieldRows-2;
				horMoviment = true;
				break;
			}

			// Verifica se está em ((*aG)->targetX, 1). Se sim, vai até ((*aG)->x, (*window)->fieldRow-2)
			if ((*aG)->x == (*aG)->targetX && (*aG)->y == 1 && horMoviment) {
				(*aG)->targetX = (*aG)->x;
				(*aG)->targetY = (*window)->fieldRows-2;
				horMoviment = false;
				break;
			}

			// Se está em ((*aG)->targetX, 1) e o movimento horizontal está desativado, vai para (x+3, 1)
			if ((*aG)->y == 1 && horMoviment == false) {
				(*aG)->targetX = (*aG)->x-3;
				(*aG)->targetY = 1;
				horMoviment = true;
				break;
			}




			break;
		case AGENT_STATUS_HOLD: // Segue até a lixeira e solta lá
			if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1) {
				ACTION_RELEASE;
				// Volta para a pesquisa reversa
				(*aG)->targetX = (*window)->fieldColumns-1;
				(*aG)->targetY = (*window)->fieldRows-1;
				(*aG)->status = AGENT_STATUS_SEARCH_REVERSE;
				break;
			}
			// Caso não, vá até a lixeira
			followLocalization(aG, window, (*window)->fieldColumns-1, (*window)->fieldRows-1);
			break;
	}
	return 0;
}
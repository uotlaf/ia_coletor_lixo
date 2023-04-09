/*
 * Objective.c - Agente baseado em objetivos
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#include "Agent/Objective.h"
#include <stdio.h>		// sprintf()
#include <Terminal.h> 	// drawObject()
#include <stdlib.h>		// malloc()
#include <UTF_char.h>	// PERSON_ICON
#include <unistd.h>
#include <Ansi.h>		// ANSI
#include <Lixo.h>		// trash*
#include <math.h>       // sqrt

/*
 * Agente baseado em objetivos
 * Procura o próximo -> vai até ele -> pega -> lixeira
 */

Agent* newObjectiveAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore) {
	Agent* temp = newSimpleAgent(initialX, initialY, initialScore);
	temp->run 	= (*_runOA);
	temp->name  = OBJECTIVE_AGENT_NAME;
	return temp;
}

Trash* objectiveSearchNextTrash(Agent** aG, Trash** trList) {
    // Retorna o lixo mais próximo do agente
    if (!aG || !(*aG) || !trList || !(*trList)) return NULL;
    Trash* ret = NULL;
    Trash* act = (*trList);
    int16_t oldDist = 0;
    int16_t newDist;

    // Calcula a distância
    while (act) {
        newDist = sqrt(pow((*aG)->x - act->x, 2) + pow((*aG)->y - act->y, 2));

        if (newDist < oldDist || oldDist == 0) {
            ret = act;
            oldDist = newDist;
        }
        act = act->next;
    }
    return ret;
}

uint16_t _runOA(Agent** aG, Trash** trList, Window** window) {
	// Executa o algoritmo do Agente baseado em objetivo
	// Primeiro verifica se está segurando algo e tem algum lixo próximo
	Trash* nextTrash;
	
    if (((*aG)->targetX == -1 && (*aG)->targetY == -1) || ((*aG)->x == 0 && (*aG)->y == 0) || ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1)) {
        // Se não sabe pra onde ir, procura quem tá próximo
        nextTrash = objectiveSearchNextTrash(aG, trList);
        if (nextTrash) {
            (*aG)->targetX = nextTrash->x;
            (*aG)->targetY = nextTrash->y;
        }
    }
	_runSA(aG, trList, window);
	return 0;
}
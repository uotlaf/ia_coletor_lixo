/*
 * Utility.c - Agente baseado em utilidade
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#include "Agent/Utility.h"
#include <stdio.h>		// sprintf()
#include <Terminal.h> 	// drawObject()
#include <stdlib.h>		// malloc()
#include <UTF_char.h>	// PERSON_ICON
#include <unistd.h>
#include <Ansi.h>		// ANSI
#include <Lixo.h>		// trash*
#include <math.h>       // sqrt

/*
 * Agente baseado em utilidade
 * Procura o próximo -> vai até ele -> pega -> lixeira
 */

Agent* newUtilityAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore) {
	Agent* temp = newSimpleAgent(initialX, initialY, initialScore);
	temp->run 	= (*_runUA);
	temp->name  = UTILITY_AGENT_NAME;
	return temp;
}

Trash* utilitySearchNextTrash(Agent** aG, Trash** trList) {
    // Retorna o lixo com maior valor e mais próximo do agente 
    if (!aG || !(*aG) || !trList || !(*trList)) return NULL;
    Trash* ret = NULL;
    Trash* act = (*trList);
    int16_t oldDist = 0, oldValue = 0;
    int16_t newDist;

    // Calcula a distância
    while (act) {
        newDist = sqrt(pow((*aG)->x - act->x, 2) + pow((*aG)->y - act->y, 2));

        // Menor distância e maior valor
        if ((newDist < oldDist && oldValue <= act->value) || oldDist == 0) {
            ret = act;
            oldDist = newDist;
            oldValue = act->value;
        }
        act = act->next;
    }
    return ret;
}

uint16_t _runUA(Agent** aG, Trash** trList, Window** window) {
	// Executa o algoritmo do Agente baseado em utilidade
	// Primeiro verifica se está segurando algo e tem algum lixo próximo
	Trash* nextTrash;
	
    if (((*aG)->targetX == -1 && (*aG)->targetY == -1) || ((*aG)->x == 0 && (*aG)->y == 0) || ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1)) {
        // Se não sabe pra onde ir, procura quem tá próximo
        nextTrash = utilitySearchNextTrash(aG, trList);
        if (nextTrash) {
            (*aG)->targetX = nextTrash->x;
            (*aG)->targetY = nextTrash->y;
        }
    }
	_runSA(aG, trList, window);
	return 0;
}
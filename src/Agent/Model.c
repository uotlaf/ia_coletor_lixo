/*
 * Model.c - Agente baseado em modelo
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */


#include "Agent/Model.h"
#include <stdio.h>		// sprintf()
#include <Terminal.h> 	// drawObject()
#include <stdlib.h>		// malloc()
#include <UTF_char.h>	// PERSON_ICON
#include <unistd.h>
#include <Ansi.h>		// ANSI
#include <Lixo.h>		// trash*

/*
 * Agente baseado em modelo
 *
 * Agente--------
 * 				|
 * 				|
 * --------------
 * |
 * |
 * --------------Lixeira
 */


Agent* newModelAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore) {
	Agent* temp = newSimpleAgent(initialX, initialY, initialScore);
	temp->run 	= (*_runMA);
	temp->name  = MODEL_AGENT_NAME;

	// Guarda o estado atual do agente no pacote reservado
	ModelAgentReserved* reserved = (ModelAgentReserved*)malloc(sizeof(ModelAgentReserved));
	if (!reserved) {
		temp->destroy(&(temp));
		return NULL;
	}
	reserved->foundedTrash = NULL;
	reserved->status = AGENT_STATUS_SEARCH_NORMAL;

	temp->reserved = (void*)reserved; // Algoritmo inicia pelo pesquisa normal
	return temp;
}

uint16_t _runMA(Agent** aG, Trash** trList, Window** window) {
	// Executa o algoritmo do Agente baseado em modelos
	// Primeiro verifica se está segurando algo e tem algum lixo próximo
	TrashLocation* nextTrash;
	nextTrash = checkForTrashInFOV(aG, trList);
	ModelAgentReserved** modelExtend = (ModelAgentReserved**)&((*aG)->reserved);
	
	if (nextTrash && (*aG)->holdItem && !((*modelExtend)->foundedTrash)) {
		// Existe um lixo próximo, o agente está segurando algo e a lista de encontrados não existe
		// Bota na lista de encontrados
		(*modelExtend)->foundedTrash = copyTrashToList(nextTrash->founded, (*modelExtend)->foundedTrash);
	}
	else if (nextTrash && (*aG)->holdItem && (*modelExtend)->foundedTrash && !checkTrashinList(nextTrash->founded, (*modelExtend)->foundedTrash)) {
		// Existe um lixo próximo, o agente está segurando algo e ele não está na lista de encontrados
		// Bota na lista de encontrados
		(*modelExtend)->foundedTrash = copyTrashToList(nextTrash->founded, (*modelExtend)->foundedTrash);
	}
	else if ((*aG)->x == (*window)->fieldColumns-1 && (*aG)->y == (*window)->fieldRows-1 && !(*aG)->holdItem && (*modelExtend)->foundedTrash){
		// Está na lixeira, não está mais com nenhum item e tem um item na lista de encontrados
		// Vai até a localização dele
		(*aG)->targetX = (*modelExtend)->foundedTrash->x;
		(*aG)->targetY = (*modelExtend)->foundedTrash->y;
	}
	else if (checkTrashinList((*aG)->holdItem, (*modelExtend)->foundedTrash)) {
		// Verifica se o ítem que o agente está segurando está na lista de encontrados. Se sim, remove ele da lista
		removeTrashfromList((*aG)->holdItem, &((*modelExtend)->foundedTrash));
	}
	_runSA(aG, trList, window);

	return 0;
}
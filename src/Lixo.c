/*
 * Lixo.c
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#include <Lixo.h>
#include <stdio.h> 		// sprintf
#include <stdlib.h> 	// malloc
#include <Terminal.h> 	// drawObject()
#include <Ansi.h> 		// ANSI codes

#ifdef COLOR_OUTPUT
#include <Color.h>
#else
#include <UTF_char.h>
#define TRASH_ICON "*"
#endif

Trash* newTrash(Window** window, uint8_t value) {
	// Gera um lixo com coordenada aleatória
	Trash* retorno = (Trash*)malloc(sizeof(Trash));
	if (!retorno) return NULL;

	retorno->x 		= rand() % ((*window)->fieldColumns	-1); // Mapa inicia em 0
	retorno->y 		= rand() % ((*window)->fieldRows	-1);
	retorno->value = value;
	retorno->next 	= NULL;
	return retorno;
}

Trash* genTrashList(int64_t seed, Window** window, uint8_t trashOrg, uint8_t trashRec) {
	// Gera de 1 a X lixos baseados em TRASH_QUANT

	// Bota seed no RNG
	srand(seed);

	Trash* sLList = NULL;
	Trash* temp;

	// Gera os lixos orgânicos
	for (int i = 0; i < trashOrg; i++) {
		temp = newTrash(window, 1);

		// verifica se um lixo com a mesma coordenada já está na lista
		if (!checkTrashinList(temp, sLList)) {
			// Se não, adiciona o lixo na lista
			sLList = addTrashinList(temp, sLList);
		} else {
			// Pó jogar fora
			free(temp);
		}
	}

	// Gera os lixos recicláveis
	for (int i = 0; i < trashRec; i++) {
		Trash* temp = newTrash(window, 5);

		// verifica se um lixo com a mesma coordenada já está na lista
		if (!checkTrashinList(temp, sLList)) {
			// Se não, adiciona o lixo na lista
			sLList = addTrashinList(temp, sLList);
		} else {
			// Pó jogar fora
			free(temp);
		}
	}

	return sLList;
}

bool checkTrashinList(Trash* tr, Trash* list) {
	// Verifica se tr está na lista ordenada list
	if (!tr || !list) {
		return false;
	}
	Trash* pos = list;
	while (pos != NULL) {
		if (pos->x < tr->x && pos->y < tr->y ) {	// Caso menor que
			pos = pos->next;
		} else if (pos->x == tr->x && pos->y == tr->y) { // Igual
			return true;
		} else {	// Maior que
			pos = pos->next;
		}
	}

	// Caso ninguém seja encontrado
	return false;
}

Trash* addTrashinList(Trash* tr, Trash* list) {
	// Bota o tr ordenado na list
	// tr não pode estar em list
	// No final retorna a nova lista

	if (!tr) {
		return false;
	}

	/* Casos:
	 * x menor e y menor = passa
	 * x maior e y menor = passa
	 * x igual e y menor = passa
	 *
	 * x menor e y maior = não passa
	 * x maior e y maior = não passa
	 * x igual e y maior = não passa
	 *
	 * x menor e y igual = passa
	 * x maior e y igual = não passa
	 * x igual e y igual = não passa
	 *
	 * Y menor || x menor e y igual
	 */
	// Procura o último da lista menor que o list
	Trash* cur = NULL;
	Trash* next = list;
	// pos->next->x < tr->x && pos->next->y < tr->y || pos->next->x >
	while (next && (next->y < tr->y || (next->x < tr->x && next->y == tr->y )) ) {
			cur = next;
			next = cur->next;
	}

	// cur é o escolhido
	// verifica se o cur == NULL. Se sim, adicionamos no início da fila
	if (!cur) {
		tr->next = next;
		return tr;
	}

	// se não, adiciona entre o escolhido e o next

	if (cur->next) {
		tr->next = cur->next;
	}
	cur->next = tr;
	return list;
}

void removeTrashfromList(Trash* tr, Trash** list) {
	// Retira o lixo da lista e retorna ela
	if (!tr || !list) return;

	Trash* act = NULL;
	Trash* next = *list;

	// Se o cabeçalho for o escolhido
	if (tr->x == (*list)->x && tr->y == (*list)->y) {
		*list = (*list)->next;
		tr->next = NULL;
		return;
	}

	while (next != NULL && (tr->x != next->x || tr->y != next->y)) {
			act = next;
			next = next->next;
	}
	// No final temos o anterior em act e nosso escolhido em next
	// ou o último em act e null em next

	if (next == NULL) {
		return; // Nenhum item foi encontrado
	}

	// Junta a lista
	act->next = next->next;

	// Limpa nosso lixo e retorna
	next->next = NULL;
}

bool destroyTrashList(Trash** tr) {
	if (!tr || !(*tr)) {
		return false;
	}
	Trash* act = (*tr);

	while (act != NULL) {
		Trash* next = act->next;
		free(act);
		act = next;
	}

	(*tr) = NULL;

	return true;
}

void drawTrash(Trash** list, Window** window) {
	Trash* pos = (*list);
	char trashColor[20];

	updateConsole("Lixo: Desenhando", window);

	while (pos) {
		uint8_t color = pos->value * (255/MAX_VALUE);
		sprintf(trashColor, ANSI_COLOR_FOREGROUND, color, 255, color );
		
		drawObject(TRASH_ICON, trashColor, (*window)->colors.boxBackground, pos->x, pos->y, window);
		pos = pos->next;
	}
}

void drawBin(Window** window) {
	drawObject(UTF_BIN, ANSI_COLOR_RESET, (*window)->colors.boxBackground, (*window)->fieldColumns-2, (*window)->fieldRows-1, window);
}

Trash* containsTrashin(Trash* trList, uint16_t x, uint16_t y) {
	if (!trList) return NULL;
	Trash* pos = trList;

	while(pos) {
		if (pos->x == x && pos->y == y) {
			return pos;
		}
		pos = pos->next;
	}
	return NULL;
}
/*
 * Lixo.h
 *
 *  Created on: 31 de mar. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_LIXO_H_
#define INCLUDE_LIXO_H_
#include <Terminal.h>	// Window



typedef struct __trash__ {
	int16_t x, y;
	uint8_t value;
	struct __trash__* next;
} Trash;

#define BIN_X FIELD_WIDTH-1
#define BIN_Y FIELD_HEIGHT - 1

Trash* newTrash(Window** window, uint8_t value);

Trash* genTrashList(int64_t seed, Window** window, uint8_t trashOrg, uint8_t trashRec);

bool checkTrashinList(Trash* tr, Trash* list);

Trash* addTrashinList(Trash* tr, Trash* list);

void removeTrashfromList(Trash* tr, Trash** list);

bool destroyTrashList(Trash** tr);

void drawTrash(Trash** list, Window** window);

Trash* containsTrashin(Trash* tr, uint16_t x, uint16_t y);

void drawBin(Window** window);

#endif /* INCLUDE_LIXO_H_ */

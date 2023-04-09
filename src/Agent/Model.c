/*
 * Model.c
 *
 *  Created on: 2 de abr. de 2023
 *      Author: uotlaf
 */


#include "Agent/Model.h"
#include <stdio.h> // NULL

Agent* newModelAgent() {
	Agent* temp = (Agent*)malloc(sizeof(Agent));
	temp->x		= 0;
	temp->y		= 0;
	temp->hold	= NULL;
	temp->score = 0;
	temp->run	= (*_runMA);
	temp->draw	= (*_drawMA);
}

int _runMA(Agent* aG, trash** trList) {

}


void _drawMA(Agent* aG, trash** trList) {

}

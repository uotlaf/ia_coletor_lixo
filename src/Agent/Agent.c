/*
 * Agent.c
 *
 *  Created on: 2 de abr. de 2023
 *      Author: uotlaf
 */

#include <Agent/Agent.h>
#include <stdio.h> // sprintf
#include <Ansi.h>
#include <UTF_char.h>
#include <Terminal.h> // drawObject

Agent* newAgent(uint16_t initialX, uint16_t initialY, uint16_t initialScore) {
	Agent* temp = (Agent*)malloc(sizeof(Agent));
	temp->x 	= 0;
	temp->y 	= 0;
	temp->score = 0;
	temp->oldx	= 0;
	temp->oldy	= 0;
	temp->draw 	= (*_drawAgent);
	temp->drawFOV = (*_drawFOV);
	temp->clean = (*_clean);
	temp->destroy = (*_destroy);
	temp->hold 	= NULL;
	temp->name  = DEFAULT_AGENT_NAME;
	return temp;
}

void _drawAgent(Agent** aG, Window** window) {
	drawObject(PERSON_ICON, ANSI_COLOR_NONE, (*window)->colors.fovBackground, (*aG)->x, (*aG)->y, window);
}

void _drawFOV(Agent** aG, Window** window) {
	// Desenha o campo de visão do agente
	// CimaEsquerda
	if ((*aG)->x-1 >= 0 && (*aG)->y-1 >= 0) {
		drawObject(FOV_UPLEFT	, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x-1	, (*aG)->y-1, window);
	}
	// Cima
	if ((*aG)->y-1 >= 0) {
		drawObject(FOV_UP		, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x	, (*aG)->y-1, window);
	}
	// CimaDireita
	if ((*aG)->x+1 < (*window)->fieldColumns && (*aG)->y-1 >= 0) {
		drawObject(FOV_UPRIGHT	, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x+1	, (*aG)->y-1, window);
	}

	// Esquerda
	if ((*aG)->x-1 >= 0) {
		drawObject(FOV_LEFT		, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x-1  , (*aG)->y	 , window);
	}
	// Direita
	if ((*aG)->x+1 < (*window)->fieldColumns) {
		drawObject(FOV_RIGHT	, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x+1  , (*aG)->y	 , window);
	}
	// BaixoEsquerda
	if ((*aG)->x-1 >= 0 && (*aG)->y+1 < (*window)->fieldRows) {
		drawObject(FOV_DOWNLEFT	, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x-1  , (*aG)->y+1, window);
	}
	// Baixo
	if ((*aG)->y+1 < (*window)->fieldRows) {
		drawObject(FOV_DOWN		, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x	, (*aG)->y+1, window);
	}
	// BaixoDireita
	if ((*aG)->x+1 < (*window)->fieldColumns && (*aG)->y+1 < (*window)->fieldRows) {
		drawObject(FOV_DOWNRIGHT, (*window)->colors.fovForeground, (*window)->colors.fovBackground, (*aG)->x+1	, (*aG)->y+1, window);
	}
}

void _clean(Agent** aG, Window** window) {
	// Limpa a área do agente

		// CimaEsquerda
	if ((*aG)->x-1 >= 0 && (*aG)->y-1 >= 0) {
		drawObject(" "	, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x-1			, (*aG)->y-1, window);
	}
	// Cima
	if ((*aG)->y-1 >= 0) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x		, (*aG)->y-1, window);
	}
	// CimaDireita
	if ((*aG)->x+1 < (*window)->fieldColumns && (*aG)->y-1 >= 0) {
		drawObject(" "	, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x+1			, (*aG)->y-1, window);
	}

	// Esquerda
	if ((*aG)->x-1 >= 0) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x-1  	, (*aG)->y	 , window);
	}
	// Meio
	if ((*aG)->x-1 >= 0) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x  		, (*aG)->y	 , window);
	}

	// Direita
	if ((*aG)->x+1 < (*window)->fieldColumns) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x+1  	, (*aG)->y	 , window);
	}
	// BaixoEsquerda
	if ((*aG)->x-1 >= 0 && (*aG)->y+1 < (*window)->fieldRows) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x-1  	, (*aG)->y+1, window);
	}
	// Baixo
	if ((*aG)->y+1 < (*window)->fieldRows) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x		, (*aG)->y+1, window);
	}
	// BaixoDireita
	if ((*aG)->x+1 < (*window)->fieldColumns && (*aG)->y+1 < (*window)->fieldRows) {
		drawObject(" "		, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*aG)->x+1		, (*aG)->y+1, window);
	}

}

float findYinLine(int x, int Ax, int Ay, int Bx, int By) {
	/* Baseado na equação geral da reta entre o ponto A e B, acha o valor de Y para o qual x=x
	 * Argumentos:
	 * x = Ponto X para qual retornar Y
	 * (Ax, Ay) = Ponto A da reta
	 * (Bx, By) = Ponto B da reta
	 */

	float num, den, m, n;

	num = (float)(Ay - By);
	den = (float)(Ax - Bx);

	// Se o denominador é 0, a linha é perfeitamente reta, retornar By
	// Se o numerador é 0, a linha é perfeitamente reta. retornar x
	if (den == 0) 		return (float)By;

	m = num / den;
	n = -(m * Ax) + Ay;

	if (n == 0) {
		return ((float)Ay / (float)Ax) * (float)x;
	}
	return m * (float)x + n;
}

void _destroy(Agent** aG) {
	// Libera a memória usada pelo agente
	if (!aG || !(*aG)) return;
	free((*aG));
	(*aG) = NULL;
}
/*
 * Ansi.h - Contém os códigos ANSI usados para manipular o terminal
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_ANSI_H_
#define INCLUDE_ANSI_H_

#define ANSI_PREFIX_HEX "\x1b["
#define ANSI_PREFIX_OCT "\033["
#define ANSI_PREFIX ANSI_PREFIX_OCT

// Especiais para posição
#define ANSI_SAVE_CURSOR_POSITION ANSI_PREFIX "s"
#define ANSI_LOAD_CURSOR_POSITION ANSI_PREFIX "u"

// Especiais para limpeza
#define ANSI_CLEAR_TERMINAL		ANSI_PREFIX "2J"
#define ANSI_CLEAR_LEFT			ANSI_PREFIX "1J"
#define ANSI_CLEAR_RIGHT		ANSI_PREFIX "0J"


// Movimentação - Argumentos são sempre strings
#define ANSI_MOVE_CURSOR_START				ANSI_PREFIX "0;0f"
#define ANSI_HIDE_CURSOR					ANSI_PREFIX "?25l"
#define ANSI_SHOW_CURSOR					ANSI_PREFIX "?25h"

// ANSI_MOVE_CURSOR(rows, columns)
#define ANSI_MOVE_CURSOR					ANSI_PREFIX "%d;%df"

#define ANSI_MOVE_CURSOR_UP					ANSI_PREFIX "A"
#define ANSI_MOVE_CURSOR_DOWN				ANSI_PREFIX "B"
#define ANSI_MOVE_CURSOR_LEFT 				ANSI_PREFIX "D"
#define ANSI_MOVE_CURSOR_RIGHT 				ANSI_PREFIX "C"

// ANSI_MOVE_CURSOR_DIRECTION_COL(columns)
#define ANSI_MOVE_CURSOR_UP_COL 			ANSI_PREFIX "%dA"
#define ANSI_MOVE_CURSOR_DOWN_COL 			ANSI_PREFIX "%dB"
#define ANSI_MOVE_CURSOR_LEFT_COL 			ANSI_PREFIX "%dD"
#define ANSI_MOVE_CURSOR_RIGHT_COL 			ANSI_PREFIX "%dC"

// Cores (r, g, b)
#define ANSI_COLOR_FOREGROUND 				ANSI_PREFIX "38;2;%d;%d;%dm"
#define ANSI_COLOR_BACKGROUND 				ANSI_PREFIX "48;2;%d;%d;%dm"
#define ANSI_COLOR_RESET					ANSI_PREFIX "0m"
#define ANSI_COLOR_NONE						""
#define ANSI_COLOR_GREEN					ANSI_PREFIX "38;5;34m"

// Especiais para texto
#define ANSI_TEXT_BLINK                     ANSI_PREFIX "5m"
#define ANSI_TEXT_NOBLINK                   ANSI_PREFIX "25m"

#endif /* INCLUDE_ANSI_H_ */

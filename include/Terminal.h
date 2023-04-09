/*
 * Terminal.h - Utilitários do terminal
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_TERMINAL_H_
#define INCLUDE_TERMINAL_H_

// includes
#include <sys/ioctl.h> 	// winsize
#include <stdint.h>		// integer types
#include <stdbool.h>	// Booleans
#include <sys/time.h>	// timeval
#include <pthread.h>	// mutex
#include <Time.h>		// Cronometer

// default
#define STEPS_SIZE 	4 // Define quantos caracteres vão ser escritos na área de passos
#define RANK_SIZE 	2 // Quantos chars vão ser escritos na área de rank
#define FPS_SIZE	3 // Quantos chars vão ser escritos na área de fps
#define TPS_SIZE	3
#define TIME_SIZE   4

// Variables
extern bool receivedSIGWINCH; // Guarda para recuperar o estado da janela no próximo loop
extern struct termios raw, canonical; // Guarda o estado do terminal no modo raw e no modo canonico

typedef enum __WindowState__ {
	WINSTATE_MENU, WINSTATE_OPTIONS, WINSTATE_HELP, WINSTATE_EXEC, WINSTATE_DONE, WINSTATE_CLEAN
} WindowState;


typedef struct __consoleText__ {
	char* text;
	struct __consoleText__* next;
} consoleText;

typedef struct __console__ {
	consoleText *textStart, *textEnd;	// LL com o texto que vai ser desenhado
	uint16_t row, columns;				// Tamanho do console
	uint16_t historySize, curSize; 		// Guarda o tamanho máximo e atual do histórico
	consoleText *lastDraw;				// Guarda o último texto que foi desenhado. Não desenha se textStart == lastDraw
} Console;

typedef struct __colors__ {
	char *fontForeground, *fontBackground;
	char *boxForeground,  *boxBackground;
	char *consoleForeground, *consoleBackground, *consoleText;
	char *fovForeground, *fovBackground;
	char *shadowBackground;
} Colors;


typedef struct __window__ {
	// Terminal
	uint16_t 		termRows, termColumns;	// Tamanho do terminal
	uint16_t 		cursorX, cursorY; 		// Posição do cursor do mouse no terminal
	// Field
	uint16_t 		fieldRows, fieldColumns;// Tamanho do campo. A janela vai ser desenhada fora dele
	// Locks
	bool 			renderLock, renderFlip;	// renderFlip é usado para controlar os frames jogados fora no motor de render. true = desenhar
	// Console
	bool 			enableConsole;			// if true processa o console
	Console 		console;				// guarda o console
	// Header
	char* 			title;					// Título da janela, desenhado em cima do frame
	// Footer
	uint16_t 		rank, steps;			// Valores do rodapé da página
	uint64_t 		execTimeUs;				// Tempo guardado no rodapé da página
	uint8_t			hold;					// Valor do item que o agente segura
	// Time and framing
	uint16_t		fpsLimit, fps;			// Guarda o limite e o fps atual
	uint16_t		tpsLimit, tps;			// Guarda o limite e o tps atual
	Cronometer 		frameCount, tpsCount;	// Guarda a data do último frame e do último tick
	bool			pause;					// Pausa a execução do algoritmo e desenha um pause na tela
	// Colors
	Colors 			colors;					// Mapa de cores que vamos utilizar
	// Threads
	bool			inputWorker;			// Define se a thread de entrada deve continuar ou não
	bool			drawWorker;				// Define se a thread de desenho deve continuar ou não 
	bool			updateWorker;			// Define se a thread de atualização deve continuar ou não
	// Mutexes
	pthread_mutex_t WriteLock;				// Trava a modificação de itens na janela
	pthread_mutex_t UpdateLock;				// Trava a thread de atualização, incluindo a execução do agente
	pthread_mutex_t DrawLock;				// Trava a thread de desenho, incluindo do agente
	WindowState 	state;					// Estado da janela
	pthread_mutex_t UpdateStart;			// inicia travado e destrava quando o algoritmo começar
} Window;


// Create
Window* newWindow(	uint16_t fieldRows, uint16_t fieldCol,
					char* title, uint16_t fpsLimit);

// Update
void updateWindowSize(Window** window);
void updateTitle(char* title, Window** window);

void updateConsole(char* text, Window** window);
void updateWarning(Window** window);

// Draw
void drawWarning(Window** window, uint16_t minX, uint16_t minY);
void drawObject(char* object, char* foregroundColor, char* backgroundColor, int16_t x, int16_t y, Window** window);

void drawConsoleFrame(Window** window);
void drawConsole(Window** window);
void drawConsoleFloor(Window** window);

void drawFrame(Window** window);
void drawFloor(Window** window);
void drawRank (uint16_t rank, Window** window);
void drawPause(Window** window);
void drawSteps(Window** window);
void drawHold (Window** window);
void drawTime (Window** window);
void drawFPS(Window** window);
void drawTPS(Window** window);


// Especiais do terminal
void enableRawMode();
void disableRawMode();
void handleSIGWINCH();

// Utils
float getTimeinMs(struct timeval start, struct timeval end);
float getTimeinUs(struct timeval start, struct timeval end);

#endif /* INCLUDE_TERMINAL_H_ */

/*
 * Terminal.c
 *
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#include <Terminal.h>
#include <Lixo.h> 		// drawBin()
#include <stdio.h> 		// printf()
#include <stdlib.h>		// malloc()
#include <Ansi.h> 		// ANSI codes
#include <unistd.h> 	// STDOUT_FILENO
#include <string.h> 	// string manipulation in updateTitle()
#include <time.h>		// LastFrame
#include <termios.h>	// Raw mode


#ifdef COLOR_OUTPUT
#include <Color.h>
#else
#include <UTF_char.h>
#define BOX_TOPLEFT 	UTF_TOPLEFT
#define BOX_TOP			UTF_TOP
#define BOX_TOPRIGHT	UTF_TOPRIGHT
#define BOX_LEFT		UTF_LEFT
#define BOX_RIGHT		UTF_RIGHT
#define BOX_BOTTOMLEFT	UTF_BOTTOMLEFT
#define BOX_BOTTOM		UTF_BOTTOM
#define BOX_BOTTOMRIGHT	UTF_BOTTOMRIGHT
#define BOX_COLOR       ANSI_PREFIX "38;2;46;156;197m"
#define FLOOR_COLOR     ANSI_PREFIX "48;2;13;26;25m"
#define FLOOR_SHADOW	ANSI_PREFIX "48;2;22;22;22m"
#define TITLE_COLOR		ANSI_COLOR_NONE

#define FONTFGCOLOR		ANSI_PREFIX "38;2;255;255;255m"
#define FONTBGCOLOR 	ANSI_COLOR_NONE

#define FLOORFGCOLOR	ANSI_PREFIX "38;2;46;156;197m"
#define FLOORBGCOLOR	ANSI_PREFIX "48;2;13;26;25m"

#define SHADOWBGCOLOR	ANSI_PREFIX "48;2;22;22;22m"

#define CONSOLEFGCOLOR	ANSI_PREFIX "38;2;19;207;24m"
#define CONSOLEBGCOLOR	ANSI_PREFIX "48;2;3;29;3m"
#define CONSOLETXTCOLOR ANSI_PREFIX	"38;2;19;207;24m"

#define FOVFGCOLOR		ANSI_PREFIX "38;2;17;35;33m"
#define FOVBGCOLOR		ANSI_PREFIX "48;2;20;40;38m"


#endif

bool receivedSIGWINCH = true;
struct termios raw, canonical;

// Create
Window* newWindow(	uint16_t fieldRows, uint16_t fieldCol,
					char* title, uint16_t fpsLimit) {
	/*
	 * Cria uma nova janela com os valores padrões
	*/
	Window* ret = (Window*)malloc(sizeof(Window));
	if (!ret) return NULL;

	// Atualiza as infos do terminal
	updateWindowSize(&ret);
	// Atualiza as infos do field
	ret->fieldRows 		= fieldRows;
	ret->fieldColumns 	= fieldCol;
	// Locks
	ret->renderLock = false;
	ret->renderFlip = true;
	// Console
	ret->enableConsole = ENABLE_CONSOLE;
	ret->console.textStart = NULL;
	ret->console.textEnd = NULL;
	ret->console.row = fieldRows;
	ret->console.columns = CONSOLE_W;
	ret->console.historySize = fieldRows;
	ret->console.curSize = 0;
	// Header
	ret->title = title;
	// Footer
	ret->rank = 0;
	ret->steps = 0;
	ret->sec = 0;
	ret->hold = 0;
	// Time and framing
	ret->fpsLimit = fpsLimit;
	ret->fps = 0;
	ret->tpsLimit = 0;
	ret->tps = 0;
	ret->pause = false;
	// Colors
	ret->colors.fontForeground 		= FONTFGCOLOR;
	ret->colors.fontBackground 		= FONTBGCOLOR;
	ret->colors.boxForeground		= FLOORFGCOLOR;
	ret->colors.boxBackground		= FLOORBGCOLOR;
	ret->colors.shadowBackground	= SHADOWBGCOLOR;
	ret->colors.consoleText			= CONSOLETXTCOLOR;
	ret->colors.consoleForeground	= CONSOLEFGCOLOR;
	ret->colors.consoleBackground	= CONSOLEBGCOLOR;
	ret->colors.fovForeground		= FOVFGCOLOR;
	ret->colors.fovBackground		= FOVBGCOLOR;

	// Threads
	ret->inputWorker = true;
	ret->drawWorker = true;
	ret->updateWorker = true;
	// Others
	ret->WriteLock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	ret->UpdateLock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	ret->DrawLock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	ret->UpdateStart = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	ret->state	= WINSTATE_MENU;
	return ret;
}


// Update
void updateWindowSize(Window** window) {
	/* 
	 * Copia as informações do terminal para a estrutura window
	*/
	if (!window || !(*window)) return;
	struct winsize tempWinSize; // POSIX: Guarda infos sobre o terminal

	// Receive window size from terminal
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &tempWinSize);

	// Store in window struct
	(*window)->termRows 	= tempWinSize.ws_row;
	(*window)->termColumns 	= tempWinSize.ws_col;
	(*window)->cursorX		= tempWinSize.ws_xpixel;
	(*window)->cursorY		= tempWinSize.ws_ypixel;
}

void updateTitle(char* title, Window** window) {
	/*
	 * Bota o endereço de title para window->title
	*/
	if (!title || !window || !(*window)) return;
	
	(*window)->title = title;
}

void updateConsole(char* text, Window** window) {
	/* 
	 * Adiciona text no console e aumenta o tamanho
	*/
	consoleText *act, *temp;
	// Verificações iniciais
	if (!text || !window || !(*window) || !(*window)->enableConsole) return;


	// Constrói o novo consoleText;
	act = (consoleText*)malloc(sizeof(consoleText));
	if (!act) return;

	// Copia o text para o consoleText
	act->text = (char*)malloc((*window)->console.columns + 1);
	if (!act->text) return;
	strncpy(act->text, text, (*window)->console.columns);


	// Verifica se ainda dá pra botar algo no histórico
	
	if (!(((*window)->console.curSize) < (*window)->console.historySize)) {
		// Console já está cheio; Apague um do fim e bote um no início
		// procura o penúltimo colocado
		temp = (*window)->console.textStart;
		if (temp->next) {
			while (temp->next->next) {
				temp = temp->next;
			}
		}

		// Temos o penúltimo colocado. Agora podemos remover o último colocado
		free(temp->next);
		temp->next = NULL;
		(*window)->console.curSize--;

		// Agora botamos o nosso no início da fila
	}
	// Ainda dá pra botar algo
	
	// Bota o novo texto no início da lista
	temp = (*window)->console.textStart;
	(*window)->console.textStart = act;
	act->next = temp;

	// Atualiza o tamanho do histórico
	(*window)->console.curSize++;
}

void updateWarning(Window** window) {
	if (!window || !(*window)) return;
	
	/* Tamanho mínimo:
	 * X = fieldColumns + (2*borda) + sombra || fieldColumns + (2*borda) + sombra + consoleWidth + (2*borda) + sombra
	 * Y = fieldHeight + (2*borda) + sombra  + centralização
	*/
	int minX, minY;


	if ((*window)->enableConsole) {
		minX = (*window)->fieldColumns + 3 + (*window)->console.columns + 3;
	} else {
		minX = (*window)->fieldColumns + 4;
	}
	minY = (*window)->fieldRows + 3;

	// RenderLock está ativado: Verifica se pode desativar
	if ((*window)->renderLock) {
		updateWindowSize(window);
		if (minX < ((*window)->termColumns + 1)  && minY <= (*window)->termRows) {
			(*window)->renderLock = false;
			receivedSIGWINCH = true;
		} else {
			drawWarning(window, minX, minY);
		}
	} else {
		// RenderLock está desativado: Verifica se deve ativar
		if (minX > ((*window)->termColumns)  || minY > (*window)->termRows) { // X + 1 = centralizar
			(*window)->renderLock = true;
			drawWarning(window, minX, minY);
		}
	}
}


// Draw
void drawObject(char* object, char* foregroundColor, char* backgroundColor, int16_t x, int16_t y, Window** window) {
 	/* Desenha um objeto dentro da área do campo
 	 * ┌────────┐
 	 * │🤖	   	 │
 	 * │	*	│
 	 * │*	  🗑 │
 	 * └────────┘
 	 *
 	 */

 	/*	Cálculo da área do campo:
 	 * X: ((Largura do terminal / 2) - ( Largura do campo / 2)) - (Largura do console / 2) 	+ Offset
 	 * Y: ((Altura do terminal / 2)  - ( Altura do campo / 2)) 	+ 1 						+ Offset
 	 */

 	// (Largura do terminal / 2) - ( Largura do campo / 2)
 	// (terminfo.ws_col / 2) - (FIELD_WIDTH / 2)

 	// Testa	
 	if ((*window)->renderLock || !(*window)->renderFlip) return;

 	// Move o cursor para a área desejada
	if ((*window)->enableConsole) {
	printf( ANSI_MOVE_CURSOR,
			((*window)->termRows	/ 2) - ((*window)->fieldRows 	 / 2) + 1 + y,
			((*window)->termColumns / 2) - ((*window)->fieldColumns		)	  + x);
	} else {
 	printf(	ANSI_MOVE_CURSOR,
 			((*window)->termRows 	/ 2) - ((*window)->fieldRows 	 / 2) + 1 + y,
 			((*window)->termColumns / 2) - ((*window)->fieldColumns  / 2)	   + x);
	}
 	// Desenha o objeto com a cor selecionada
 	printf("%s%s%s", foregroundColor, backgroundColor, object);
 	// Reinicia a cor
 	printf(ANSI_COLOR_RESET);

#ifdef INSTANT_DRAW
	fflush(stdout);
#endif
}

void drawFrame(Window** window) {
	/*	Desenha um objeto fora do campo(margem de 2 caracteres)
 	 *    
 	 *  ┌Titulo────────────┐
 	 *  │	   	 		   │
 	 *  │		 		   │
 	 *  h	  	 		   │
 	 *  o	  	 		   │
 	 *  └passos/tempo/rank─┘
 	 *a
 	 */
 	if ((*window)->renderLock || !(*window)->renderFlip) return;
	uint8_t titleLocation, titleLen;
	char title[((*window)->fieldColumns)-1];

 	// De cima para baixo
 	// Desenha o título primeiro
	if (strlen((*window)->title) < (*window)->fieldColumns-2) {
		titleLen = strlen((*window)->title);
		titleLocation = ((*window)->fieldColumns/2)-(titleLen/2);
		drawObject((*window)->title, (*window)->colors.fontForeground, (*window)->colors.boxBackground, titleLocation, -1, window);
	} else {
		strncpy(title, (*window)->title, (*window)->fieldColumns-2);
		title[(*window)->fieldColumns-2] = 0;
		titleLen = strlen(title);
		titleLocation = ((*window)->fieldColumns/2)-(titleLen/2);
		drawObject(title, (*window)->colors.fontForeground, (*window)->colors.boxBackground, titleLocation, -1, window);
	}

 	// Parte de cima
 	drawObject(BOX_TOPLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, -1, window);

	// Desenha ao redor do texto
 	for (int x = 1; x < titleLocation; x++) {
 		drawObject(BOX_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
 	}

	for (int x = titleLocation+titleLen; x < (*window)->fieldColumns; x++) {
		drawObject(BOX_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
	} 

 	drawObject(BOX_TOPRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, -1, window);

 	// Meio do frame
 	for (int y = 0; y < (*window)->fieldRows; y++) {
 		drawObject(BOX_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, y, window);
 		drawObject(BOX_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, y, window);
 	}

 	if ((*window)->hold) {
 		drawHold(window);
 	}

 	// Parte de baixo
 	drawObject(BOX_BOTTOMLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, (*window)->fieldRows, window);
 	for (int x = STEPS_SIZE; x < ((*window)->fieldColumns - (RANK_SIZE + 1 + FPS_SIZE)); x++) {
 		drawObject(BOX_BOTTOM, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, (*window)->fieldRows, window);
 	}

	// Barra entre o contador de fps e os pontos
	drawObject(BOX_BOTTOM, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - RANK_SIZE - 1, (*window)->fieldRows, window);
	// Lateral do terminal

	drawObject(BOX_BOTTOMRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, (*window)->fieldRows, window); 	

#ifdef DRAW_SHADOW
	// Desenha a sombra da janela
	
	// Só desenha a sombra da direita se o console estiver desabilitado
	if (!((*window)->enableConsole)) {
		for (int y = 0; y < (*window)->fieldRows+1; y++) {
			drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.shadowBackground, (*window)->fieldColumns+1, y, window);
		}
	}
	for (int x = 0; x < (*window)->fieldColumns+2; x++) {
		drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.shadowBackground, x, (*window)->fieldRows+1, window);
	}
#endif
}

void drawPause(Window** window) {
	// Desenha o Botão de pause
	if (!window || !(*window)) return;

	if ((*window)->pause) {
		drawObject(UTF_PAUSE, ANSI_TEXT_NOBLINK, (*window)->colors.boxBackground, 0, -1, window);
	} else {
		drawObject(UTF_PLAY, ANSI_TEXT_BLINK, (*window)->colors.boxBackground, 0, -1, window);
	}
}

void drawFloor(Window** window) {
	if ((*window)->renderLock || !(*window)->renderFlip) return;

	for (int y = 0; y < (*window)->fieldRows; y++) {
		for (int x = 0; x < (*window)->fieldColumns; x++) {
			drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.boxBackground, x, y, window);
		}
	}
}

void drawRank(uint16_t rank, Window** window) {
	if (!window || !(*window)) return;

 	// Desenha o rank em cima do frame
 	char text[sizeof(rank)*(RANK_SIZE+1)];
 	sprintf(text, "%02d", rank);
 	drawObject(text, ANSI_COLOR_RESET, FLOOR_COLOR, (*window)->fieldColumns-2, (*window)->fieldRows, window);
}

void drawSteps(Window** window) {
	if (!window || !(*window)) return;
	
	// Desenha a quantidade de passos que o agente fez
 	char text[sizeof((*window)->steps)*(STEPS_SIZE+1)];
 	sprintf(text, "%05d", (*window)->steps);
 	drawObject(text, ANSI_COLOR_RESET, FLOOR_COLOR, 0, (*window)->fieldRows, window);
}

void drawHold(Window** window) {
	if (!window || !(*window) || !(*window)->hold) return;
	// Desenha uma mão e o valor toda vez que o agente está segurando algo
	// Conversão int->char
	char c = (*window)->hold+'0';

 	drawObject(UTF_HAND, ANSI_COLOR_NONE, FLOOR_COLOR, -1, (*window)->fieldRows-2, window);
 	drawObject(&c, ANSI_COLOR_NONE, FLOOR_COLOR, -1, (*window)->fieldRows-1, window);
}

void drawTime(float ms, Window** window) {
	if (!window || !(*window)) return;

	// Desenha o tempo passado desde o início da execução do agente
 	char buf[400];
 	sprintf(buf, "%.2fs", ms/1000);
 	drawObject(buf, ANSI_COLOR_NONE, ANSI_COLOR_NONE, (*window)->fieldColumns/2-(strlen(buf)/2), (*window)->fieldRows, window);
}

void drawConsoleFrame(Window** window) {
	if (!window || !(*window) || !((*window)->renderFlip) || ((*window)->renderLock) || !((*window)->enableConsole)) return;
	// Desenha a borda do console

	/*	Desenha um objeto fora do campo(margem de 2 caracteres)
 	 *    
 	 *  ┌Console───────────┐
 	 *  │	   	 		   │
 	 *  |		 		   │
 	 *  |	  	 		   │
 	 *  |	  	 		   │
 	 *  └──────────────────┘
 	 *
 	 */
 	if ((*window)->renderLock || !(*window)->renderFlip) return;

	// Adicicionar (*window)->fieldColumns para todo X desenhado

 	// De cima para baixo
 	// Desenha o título primeiro
	char* title = "Console";
	uint8_t titleLocation = ((*window)->console.columns/2)-(strlen(title)/2);
 	drawObject(title, (*window)->colors.consoleText, (*window)->colors.consoleBackground, titleLocation+(*window)->fieldColumns+1, -1, window);

 	// Parte de cima
 	drawObject(BOX_TOPLEFT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns+1, -1, window);

	// Desenha ao redor do texto
 	for (int x = 1; x < titleLocation; x++) {
 		drawObject(BOX_TOP, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x+((*window)->fieldColumns)+1, -1, window);
 	}
	for (int x = titleLocation+strlen(title); x < (*window)->console.columns+1; x++) {
		drawObject(BOX_TOP, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x+(*window)->fieldColumns+1, -1, window);
	} 

 	drawObject(BOX_TOPRIGHT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns+(*window)->console.columns+2, -1, window);

 	// Meio do frame
 	for (int y = 0; y < (*window)->fieldRows; y++) {
 		drawObject(BOX_LEFT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns+1, y, window);
 		drawObject(BOX_RIGHT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns+(*window)->console.columns+2, y, window);
 	}


 	// Parte de baixo
 	drawObject(BOX_BOTTOMLEFT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, ((*window)->fieldColumns)+1, (*window)->fieldRows, window);
 	for (int x = 1; x < (*window)->console.columns+1; x++) {
 		drawObject(BOX_BOTTOM, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x+(*window)->fieldColumns+1, (*window)->fieldRows, window);
 	}
 	drawObject(BOX_BOTTOMRIGHT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns+(*window)->console.columns+2, (*window)->fieldRows, window);

#ifdef DRAW_SHADOW
	// Desenha a sombra da janela
	for (int y = 0; y < (*window)->fieldRows+2; y++) {
		drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.shadowBackground, (*window)->fieldColumns+1+(*window)->console.columns+2, y, window);
	}
	for (int x = 0; x < (*window)->console.columns+1; x++) {
		drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.shadowBackground, x+(*window)->fieldColumns+2, (*window)->fieldRows+1, window);
	}
#endif
}

void drawConsoleFloor(Window** window) {
	if (!window || !(*window) || !((*window)->renderFlip) || ((*window)->renderLock) || !((*window)->enableConsole)) return;

	for (int y = 0; y < (*window)->console.row; y++) {
		for (int x = 1; x < (*window)->console.columns+1; x++) {
			drawObject(" ", ANSI_COLOR_NONE, (*window)->colors.consoleBackground, x+(*window)->fieldColumns + 1, y, window);
		}
	}
}

void drawConsole(Window** window) {
	if (!window || !(*window) || !((*window)->renderFlip) || ((*window)->renderLock) || !((*window)->enableConsole)) return;
	// Verifica se o console atual já foi desenhado
	if (((*window)->console.lastDraw) == ((*window)->console.textStart)) return;


	// Desenha o console na tela
	consoleText *act = (*window)->console.textStart;
	int8_t curY = (*window)->fieldRows-1;


	for (int i = 0; i < (*window)->console.curSize; i++) {
		if (act) {
			drawObject(act->text, (*window)->colors.consoleText, (*window)->colors.consoleBackground, (*window)->fieldColumns+2, curY, window);
			act = act->next;
			curY--;
		}
	}
	
	// Guarda a última string que foi desenhada
	(*window)->console.lastDraw = (*window)->console.textStart;
}

void drawWarning(Window** window, uint16_t minX, uint16_t minY) {
	if (!window || !(*window)) return;
	/* Desenha a janela de aviso para aumentar o tamanho do term
	 * Esta janela é desenhada direto com printf, 
	 * já que o renderLock está travando a renderização do resto dos objetos
	 * Versão maior	| Versão menor
	 *  ╔═══════╗	|
 	 *  ║✕💻🗔↘→║	|    ↘→✕💻
 	 *  ║↓02║→02║	|
 	 *  ╚═══════╝	|
	*/


	uint8_t warningRows = 4;
	uint8_t warningColumns = 9;
	int16_t minRows =  minY - (*window)->termRows;
	int16_t minColumns = minX - (*window)->termColumns;

	if (minRows < 0) 	 minRows = 0;
	if (minColumns < 0) minColumns = 0;

	// Limpa a tela antes de tudo
	printf(ANSI_CLEAR_TERMINAL);

	// Se necessário, imprime a versão menor
 	if ((*window)->termRows < 5 || (*window)->termColumns < 9) {
		printf(ANSI_MOVE_CURSOR, (*window)->termRows / 2, ((*window)->termColumns / 2) - 2 );
		printf("%s%s%s%s", UTF_ARROW_DR, UTF_ARROW_RIGHT, UTF_X, UTF_COMPUTER);
 		return;
 	}

	// Desenha a versão maior
	// Move para a posição necessária
	printf(ANSI_MOVE_CURSOR, ((*window)->termRows / 2) - (warningRows / 2), ((*window)->termColumns / 2 ) - (warningColumns / 2));

 	// Versão maior
 	printf(ANSI_MOVE_CURSOR_LEFT_COL, warningColumns / 2);
 	printf(ANSI_MOVE_CURSOR_UP_COL, warningRows / 2);

 	printf(ANSI_SAVE_CURSOR_POSITION); // Salva para uso futuro

 	// Desenha a primeira linha
 	printf(UTF_TOPLEFT);
 	for (int i = 0; i < warningColumns - 2; i++) {
 		printf(UTF_TOP);
 	}
 	printf(UTF_TOPRIGHT);

 	printf(ANSI_LOAD_CURSOR_POSITION); // Retorna para o início

 	// Segunda linha
 	printf(ANSI_MOVE_CURSOR_DOWN);
 	printf(ANSI_SAVE_CURSOR_POSITION);
 	printf("%s %s%s%s%s %s", UTF_LEFT, UTF_X, UTF_COMPUTER, UTF_WINDOW, UTF_ARROW_DR, UTF_RIGHT);
 	printf(ANSI_LOAD_CURSOR_POSITION);

 	// Linha com os números
 	printf(ANSI_MOVE_CURSOR_DOWN);
 	printf(ANSI_SAVE_CURSOR_POSITION);
 	printf("%s%s%02d%s%s%02d%s", UTF_LEFT, UTF_ARROW_DOWN, minRows, UTF_LEFT, UTF_ARROW_RIGHT, minColumns, UTF_RIGHT);
 	printf(ANSI_LOAD_CURSOR_POSITION);

 	// Última linha
 	printf(ANSI_MOVE_CURSOR_DOWN);
 	printf(ANSI_SAVE_CURSOR_POSITION);
 	printf(UTF_BOTTOMLEFT);
 	for (int i = 0; i < warningColumns - 2; i++) {
 		printf(UTF_BOTTOM);
 	}
 	printf(UTF_BOTTOMRIGHT);
 	printf(ANSI_LOAD_CURSOR_POSITION);
}

void drawFPS(Window** window) {
	if (!window || !(*window)) return;
	char buffer[10];
	// Desenha o contador de fps
	sprintf(buffer, "%05d", (*window)->fps);
	drawObject(buffer, (*window)->colors.consoleText, (*window)->colors.boxBackground, (*window)->fieldColumns - RANK_SIZE - 1 - FPS_SIZE, (*window)->fieldRows, window);
}

void drawTPS(Window** window) {
	if (!window || !(*window)) return;
	char buffer[10];
	// Desenha o contador de fps
	sprintf(buffer, "%05d", (*window)->tps);
	drawObject(buffer, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - RANK_SIZE - 1 - FPS_SIZE - 1 - TPS_SIZE, (*window)->fieldRows, window);
}
// Terminal specials
void enableRawMode() {
	printf(ANSI_HIDE_CURSOR);
	tcgetattr(STDIN_FILENO, &raw);
	atexit(disableRawMode);
	tcgetattr(STDIN_FILENO, &canonical);
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	
}

void disableRawMode() {
	printf(ANSI_SHOW_CURSOR);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &canonical);
}

void handleSIGWINCH() {
 	/* Recebe um SIGWINCH para redimensionar o terminal
 	 */
 	receivedSIGWINCH = true;
}

// Utils
float getTimeinMs(struct timeval start, struct timeval end) {
	return ((((long long)end.tv_sec)*1000)+(end.tv_usec/1000)) - ((((long long)start.tv_sec)*1000)+(start.tv_usec/1000));
}

float getTimeinUs(struct timeval start, struct timeval end) {
	return ((((long long)end.tv_sec)*1000)+(end.tv_usec)) - ((((long long)start.tv_sec)*1000)+(start.tv_usec));
}
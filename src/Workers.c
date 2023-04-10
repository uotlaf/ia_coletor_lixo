/*
 * workers.c - Threads
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#define _GNU_SOURCE			// warning: implicit declaration of function ‘pthread_setname_np’
#include <Workers.h>
#include <Terminal.h> 		// Window
#include <unistd.h>			// read
#include <Time.h>			// Cronometer
#include <pthread.h>		// pthread_setname_np
#include <stdio.h>			// sprintf
#include <Ansi.h>			// ANSI_TERMINAL_CLEAR
#include <math.h>			// pow()
#include <Menu.h>			// drawMainMenu
#include <ctype.h>			// iscntrl()
#include <Agent/Simple.h> 	// Simple Agent
#include <Agent/Model.h>	// Model Agent
#include <Agent/Objective.h>// Objective Agent
#include <Agent/Utility.h>	// Utility Agent

pthread_t inputThread;
pthread_t renderThread;
pthread_t updateThread;


void* inputWorker(void* workerPKG) {
	// Pega os itens do teclado e processa
	WorkerPKG* pkg = (WorkerPKG*)workerPKG;
	pthread_setname_np(inputThread, "Input Worker");
	if (!(pkg) || !(*pkg->window)) return NULL;
	// Recebe algum caractere do terminal
	char c; // Caractere pego do terminal
	Trash* tempt;
	Agent* agent;

	do {
		c = '\0';
		(void)!read(STDIN_FILENO, &c, 1);
		switch ((*pkg->window)->state) {

			case WINSTATE_MENU:
				// Só caracteres de controle e enter são permitidos
				// Caratere de controle tem 3 bytes: 27, [ e a direção[A, B, C e D]
				if (iscntrl(c)) {
					if (c == 27 ) {
						(void)!read(STDIN_FILENO, &c, 1);; // Pula [
						(void)!read(STDIN_FILENO, &c, 1);;
						switch (c) {
							case 'A': // Cima
								if (menuSelected > 0) {
									menuSelected--;
								}
								break;
							case 'B': // Baixo
								if (menuSelected < MAINMENU_ROW - 1) {
									menuSelected++;
								}
							break;
						}
					} else if (c == '\n') { // Enter apertado
						switch (menuSelected) {
							case 0:
								// Selecionado Agente simples
								// Prepara a janela para o agente
								(*pkg->window)->fieldRows 		= defaultOptions.fieldRows;
								(*pkg->window)->fieldColumns 	= defaultOptions.fieldColumns;
								(*pkg->window)->fpsLimit		= defaultOptions.fpsLimit;
								(*pkg->window)->tpsLimit		= defaultOptions.tpsLimit;
								(*pkg->window)->console.row		= defaultOptions.fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								(*pkg->window)->pause 			= false;
								(*pkg->window)->steps			= 0;
								(*pkg->window)->execTimeUs		= 0;

								
								// Prepara o lixo
								tempt = genTrashList(defaultOptions.seed, &(*pkg->window), defaultOptions.trashOrg, defaultOptions.trashRec);
								pkg->trash = &tempt;

								// Prepara o agente
								agent = newSimpleAgent(0, 0, 0);
								pkg->agent = &agent;

								// Destrava o updateWorker
								pthread_mutex_unlock(&((*pkg->window)->UpdateStart));

								// Atualiza o título da janela
								(*pkg->window)->title = agent->name;

								// Inicia o algoritmo
								(*pkg->window)->state = WINSTATE_EXEC;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
							case 1:
								// Selecionado Agente baseado em modelo
								// Prepara a janela para o agente
								(*pkg->window)->fieldRows 		= defaultOptions.fieldRows;
								(*pkg->window)->fieldColumns 	= defaultOptions.fieldColumns;
								(*pkg->window)->fpsLimit		= defaultOptions.fpsLimit;
								(*pkg->window)->tpsLimit		= defaultOptions.tpsLimit;
								(*pkg->window)->console.row		= defaultOptions.fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								(*pkg->window)->pause 			= false;
								(*pkg->window)->steps			= 0;
								(*pkg->window)->execTimeUs		= 0;
								
								// Prepara o lixo
								tempt = genTrashList(defaultOptions.seed, &(*pkg->window), defaultOptions.trashOrg, defaultOptions.trashRec);
								pkg->trash = &tempt;

								// Prepara o agente
								agent = newModelAgent(0, 0, 0);
								pkg->agent = &agent;

								// Destrava o updateWorker
								pthread_mutex_unlock(&((*pkg->window)->UpdateStart));

								// Atualiza o título da janela
								(*pkg->window)->title = agent->name;

								// Inicia o algoritmo
								(*pkg->window)->state = WINSTATE_EXEC;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
							case 2:
								// Selecionado Agente Baseado em Objetivos
								// Prepara a janela
								(*pkg->window)->fieldRows 		= defaultOptions.fieldRows;
								(*pkg->window)->fieldColumns 	= defaultOptions.fieldColumns;
								(*pkg->window)->fpsLimit		= defaultOptions.fpsLimit;
								(*pkg->window)->tpsLimit		= defaultOptions.tpsLimit;
								(*pkg->window)->console.row		= defaultOptions.fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								(*pkg->window)->pause 			= false;
								(*pkg->window)->steps			= 0;
								(*pkg->window)->execTimeUs		= 0;
								
								// Prepara o lixo
								tempt = genTrashList(defaultOptions.seed, &(*pkg->window), defaultOptions.trashOrg, defaultOptions.trashRec);
								pkg->trash = &tempt;

								// Prepara o agente
								agent = newObjectiveAgent(0, 0, 0);
								pkg->agent = &agent;

								// Destrava o updateWorker
								pthread_mutex_unlock(&((*pkg->window)->UpdateStart));

								// Atualiza o título da janela
								(*pkg->window)->title = agent->name;

								// Inicia o algoritmo
								(*pkg->window)->state = WINSTATE_EXEC;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
							case 3:
								// Selecionado Agente baseado em Utilidades
								// Prepara a janela
								(*pkg->window)->fieldRows 		= defaultOptions.fieldRows;
								(*pkg->window)->fieldColumns 	= defaultOptions.fieldColumns;
								(*pkg->window)->fpsLimit		= defaultOptions.fpsLimit;
								(*pkg->window)->tpsLimit		= defaultOptions.tpsLimit;
								(*pkg->window)->console.row		= defaultOptions.fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								(*pkg->window)->pause 			= false;
								(*pkg->window)->steps			= 0;
								(*pkg->window)->execTimeUs		= 0;
								
								// Prepara o lixo
								tempt = genTrashList(defaultOptions.seed, &(*pkg->window), defaultOptions.trashOrg, defaultOptions.trashRec);
								pkg->trash = &tempt;

								// Prepara o agente
								agent = newUtilityAgent(0, 0, 0);
								pkg->agent = &agent;

								// Destrava o updateWorker
								pthread_mutex_unlock(&((*pkg->window)->UpdateStart));

								// Atualiza o título da janela
								(*pkg->window)->title = agent->name;

								// Inicia o algoritmo
								(*pkg->window)->state = WINSTATE_EXEC;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
								break;
							case 4:
								// Selecionado Configurações
								(*pkg->window)->state = WINSTATE_OPTIONS;
								// Configura a janela para as configs
								(*pkg->window)->fieldRows = SETTINGSMENU_ROW;
								(*pkg->window)->fieldColumns = SETTINGSMENU_COL;
								(*pkg->window)->console.row		= (*pkg->window)->fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
							case 5:
								// Selecionado Atalhos
								(*pkg->window)->state = WINSTATE_HELP;
								// Configuração da janela
								(*pkg->window)->fieldRows = HELPMENU_ROW;
								(*pkg->window)->fieldColumns = HELPMENU_COL;
								(*pkg->window)->console.row		= (*pkg->window)->fieldRows;
								(*pkg->window)->console.historySize = (*pkg->window)->console.row;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
						}
					}
				} else if (c == 'c'){ // Desabilitar / habilitar console
					pthread_mutex_lock(&((*pkg->window)->WriteLock));
					if ((*pkg->window)->enableConsole) 	updateConsole("Console: ^C. desabilitando", &(*pkg->window));
					else 								updateConsole("Console: ^C. habilitando", &(*pkg->window));
					
					
					(*pkg->window)->enableConsole = !((*pkg->window)->enableConsole);
					pthread_mutex_unlock(&((*pkg->window)->WriteLock));
					
					// Redesenha toda a janela
					receivedSIGWINCH = true;
				}	
				
			break;

			case WINSTATE_OPTIONS:
				// Só caracteres de controle e enter são permitidos
				// Caratere de controle tem 3 bytes: 27, [ e a direção[A, B, C e D]
				if (iscntrl(c)) {
					if (c == 27 ) {
						(void)!read(STDIN_FILENO, &c, 1);; // Pula [
						(void)!read(STDIN_FILENO, &c, 1);;
						switch (c) {
							case 'A': // Cima
								if (menuSelected > 0) {
									menuSelected--;
								}
								break;
							case 'B': // Baixo
								if (menuSelected < SETTINGSMENU_ROW - 1) {
									menuSelected++;
								}
								break;
							case 'C': // Direita
								switch (menuSelected) {
									case 0: // Colunas do campo
										if (defaultOptions.fieldColumns < ((*pkg->window)->termColumns) - 4) {
											defaultOptions.fieldColumns++;
										}
										break;
									case 1: // Linhas do campo
										if (defaultOptions.fieldRows < ((*pkg->window)->termRows - 4)) {
											defaultOptions.fieldRows++;
										}
										break;
									case 2: // FPS
										if (defaultOptions.fpsLimit < 999) {
											defaultOptions.fpsLimit++;
										}
										break;
									case 3: // TPS
										if (defaultOptions.tpsLimit < 999) {
											defaultOptions.tpsLimit++;
										}
										break;
									case 4: // SEED
										break;
									case 5: // Lixo reciclável
										if (defaultOptions.trashRec < 100) {
											defaultOptions.trashRec++;
										}
										break;
									case 6: // Lixo orgânico
										if (defaultOptions.trashOrg < 100) {
											defaultOptions.trashOrg++;
										}
										break;
								}
								break;
							case 'D': // Esquerda
								switch (menuSelected) {
									case 0: // Colunas do campo
										if (defaultOptions.fieldColumns > 5) {
											defaultOptions.fieldColumns--;
										}
										break;
									case 1: // Linhas do campo
										if (defaultOptions.fieldRows > 5) {
											defaultOptions.fieldRows--;
										}
										break;
									case 2: // FPS
										if (defaultOptions.fpsLimit > 0) {
											defaultOptions.fpsLimit--;
										}
										break;
									case 3: // TPS
										if (defaultOptions.tpsLimit > 0) {
											defaultOptions.tpsLimit--;
										}
										break;
									case 4: // SEED
										break;
									case 5: // Lixo reciclável
										if (defaultOptions.trashRec > 1) {
											defaultOptions.trashRec--;
										}
										break;
									case 6: // Lixo orgânico
										if (defaultOptions.trashOrg > 1) {
											defaultOptions.trashOrg--;
										}
										break;
								}
								break;
							case '\033': // ESC
								(*pkg->window)->state = WINSTATE_MENU;
								// Configura a janela para as configs
								(*pkg->window)->fieldRows = MAINMENU_ROW;
								(*pkg->window)->fieldColumns = MAINMENU_COL;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
						}
					}
				} else if (c == 'c'){ // Desabilitar / habilitar console
					pthread_mutex_lock(&((*pkg->window)->WriteLock));
					if ((*pkg->window)->enableConsole) 	updateConsole("Console: ^C. desabilitando", &(*pkg->window));
					else 								updateConsole("Console: ^C. habilitando", &(*pkg->window));
					
					
					(*pkg->window)->enableConsole = !((*pkg->window)->enableConsole);
					pthread_mutex_unlock(&((*pkg->window)->WriteLock));
					
					// Redesenha toda a janela
					receivedSIGWINCH = true;
				}	
			break;
			case WINSTATE_HELP:
				// Só caracteres de controle e enter são permitidos
				// Caratere de controle tem 3 bytes: 27, [ e a direção[A, B, C e D]
				if (iscntrl(c)) {
					if (c == 27 ) {
						(void)!read(STDIN_FILENO, &c, 1);; // Pula [
						(void)!read(STDIN_FILENO, &c, 1);;
						switch (c) {
							case '\033': // ESC
								(*pkg->window)->state = WINSTATE_MENU;
								// Configura a janela para as configs
								(*pkg->window)->fieldRows = MAINMENU_ROW;
								(*pkg->window)->fieldColumns = MAINMENU_COL;
								receivedSIGWINCH = true;
								menuSelected = 0;
								break;
						}
					}
				}
				else if (c == 'c'){ // Desabilitar / habilitar console
					pthread_mutex_lock(&((*pkg->window)->WriteLock));
					if ((*pkg->window)->enableConsole) 	updateConsole("Console: ^C. desabilitando", &(*pkg->window));
					else 								updateConsole("Console: ^C. habilitando", &(*pkg->window));
					
					
					(*pkg->window)->enableConsole = !((*pkg->window)->enableConsole);
					pthread_mutex_unlock(&((*pkg->window)->WriteLock));
					
					// Redesenha toda a janela
					receivedSIGWINCH = true;
				}	
			break;
			// Execução do algoritmo
			case WINSTATE_EXEC:
				if (iscntrl(c)) {
					if (c == 27 ) {
						(void)!read(STDIN_FILENO, &c, 1);; // Pula [
						(void)!read(STDIN_FILENO, &c, 1);;
						switch (c) {
							case '\033': // ESC
								// Volta para o menu principal
								(*pkg->window)->state = WINSTATE_CLEAN;
								// Configura a janela para as configs
								receivedSIGWINCH = true;
								break;
						}
					}
				} else {
					switch (c) {
						case 'c': // Desabilitar / habilitar console
							pthread_mutex_lock(&((*pkg->window)->WriteLock));
							if ((*pkg->window)->enableConsole) 	updateConsole("Console: ^C. desabilitando", &(*pkg->window));
							else 								updateConsole("Console: ^C. habilitando", &(*pkg->window));
							
							
							(*pkg->window)->enableConsole = !((*pkg->window)->enableConsole);
							pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							
							// Redesenha toda a janela
							receivedSIGWINCH = true;
							break;
						case '+' : // Aumentar o framerate só se for menor que o tamanho do inteiro não assinado de 16 bits
							if ((*pkg->window)->fpsLimit < FPS_MAX_LIMIT) {
								pthread_mutex_lock(&((*pkg->window)->WriteLock));
								(*pkg->window)->fpsLimit++;
								pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							}
							break;
						case '-' : // Diminuir o framerate
							if ((*pkg->window)->fpsLimit > 1) {
								pthread_mutex_lock(&((*pkg->window)->WriteLock));
								(*pkg->window)->fpsLimit--;
								pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							}
							break;
						case 'w' : // Aumentar o tps só se for menor que 999
							if ((*pkg->window)->tpsLimit < 999) {
								pthread_mutex_lock(&((*pkg->window)->WriteLock));
								(*pkg->window)->tpsLimit++;
								pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							}
							break;
						case 'q' : // Diminuir TPS
							if ((*pkg->window)->tpsLimit > 1) {
								pthread_mutex_lock(&((*pkg->window)->WriteLock));
								(*pkg->window)->tpsLimit--;
								pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							}
							break;
						case 'f': // Habilita/desabilita o limite de framerate
							pthread_mutex_lock(&((*pkg->window)->WriteLock));
							if ((*pkg->window)->fpsLimit) { // Se está ativado, desativa
								(*pkg->window)->fpsLimit = 0;
							} else { // Ativa com o valor padrão
								(*pkg->window)->fpsLimit = defaultOptions.fpsLimit;
							}
							pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							break;
						case 't': // Habilita/desabilita o limite de tickrate
							pthread_mutex_lock(&((*pkg->window)->WriteLock));
							pthread_mutex_lock(&((*pkg->window))->UpdateStart);
							if ((*pkg->window)->tpsLimit) { // Se está ativado, desativa
								(*pkg->window)->tpsLimit = 0;
							} else { // Ativa com o valor padrão
								(*pkg->window)->tpsLimit = defaultOptions.tpsLimit;
							}
							pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							pthread_mutex_unlock(&((*pkg->window))->UpdateStart);
							break;
						case ' ': // Pausa/continua o agente
							pthread_mutex_lock(&((*pkg->window)->WriteLock));
							(*pkg->window)->pause = !((*pkg->window)->pause);
							pthread_mutex_unlock(&((*pkg->window)->WriteLock));
							break;
					}
				}
			case WINSTATE_DONE:
				break;
			case WINSTATE_CLEAN:
				// Limpeza do algoritmo para voltar ao menu

				// Primeiro trava o updateWorker
				pthread_mutex_lock(&((*pkg->window)->UpdateStart));
				// Limpa primeiro a janela
				(*pkg->window)->fieldRows 		= MAINMENU_ROW;
				(*pkg->window)->fieldColumns 	= MAINMENU_COL;
				(*pkg->window)->fpsLimit		= defaultOptions.fpsLimit;
				(*pkg->window)->tpsLimit		= defaultOptions.tpsLimit;
				(*pkg->window)->console.row		= (*pkg->window)->fieldRows;
				(*pkg->window)->console.historySize = (*pkg->window)->console.row;
				(*pkg->window)->title 			= TEXT_MENU_TITLE;


				// Limpa o algoritmo
				if (*pkg->agent) {
					(*pkg->agent)->destroy(pkg->agent);
				}

				// Limpeza dos lixos
				if (*pkg->trash) {
					destroyTrashList(pkg->trash);
				}

				// Continua para desenhar o menu principal
				receivedSIGWINCH = true;
				menuSelected = 0; 
				(*pkg->window)->state = WINSTATE_MENU;
				break;
		}
		usleep(10000);
	} while ((*pkg->window)->inputWorker);
	return NULL;
}

void* drawWorker(void* workerPKG) {
	// Desenha as coisas no terminal em determinado fps
	// TODO: Implementar double buffer?
	WorkerPKG* pkg = (WorkerPKG*)workerPKG;
	pthread_setname_np(renderThread, "Render Worker");
	if (!pkg || !(pkg->window)) return NULL;

	// Em uSec
	int64_t frametimeLimit;
	int64_t frametimeAct;
	int64_t sleepValue;

	// Buffer para guardar o console
	char buffer[(*pkg->window)->console.columns];
	
	
	do {
		// Recalcula o limite de fps a cada quadro
		frametimeLimit = (1.0f/(*pkg->window)->fpsLimit)*1000000;
		// Pega o tempo do início deste quadro
		startCronometer(&(*pkg->window)->frameCount);

		switch ((*pkg->window)->state) {

			case WINSTATE_MENU:
				// Desenha o menu
				if (receivedSIGWINCH) {
					// Redesenha toda a janela
					printf(ANSI_CLEAR_TERMINAL);
					updateWindowSize(&(*pkg->window));
					updateWarning(&(*pkg->window));
					drawConsoleFloor(&(*pkg->window));
					drawConsoleFrame(pkg->window);
					receivedSIGWINCH = false;
				}

				// Menu é desenhado a cada quadro
				drawConsole(pkg->window);
				drawMainMenu(&(*pkg->window));
			break;

			case WINSTATE_OPTIONS:
				// Menu de opções
				if (receivedSIGWINCH) {
					// Redesenha toda a janela
					printf(ANSI_CLEAR_TERMINAL);
					updateWindowSize(&(*pkg->window));
					updateWarning(&(*pkg->window));

					drawConsoleFloor(&(*pkg->window));
					drawConsoleFrame(pkg->window);
					receivedSIGWINCH = false;
				}
				drawConsole(pkg->window);
				drawSettingsMenu(&(*pkg->window));
			break;
			case WINSTATE_HELP:
				// Menu de ajuda/atalhos
				if (receivedSIGWINCH) {
					// Redesenha toda a janela
					printf(ANSI_CLEAR_TERMINAL);
					updateWindowSize(&(*pkg->window));
					updateWarning(&(*pkg->window));

					drawConsoleFloor(&(*pkg->window));
					drawConsoleFrame(pkg->window);
					receivedSIGWINCH = false;
				}
				drawConsole(pkg->window);
				drawHelpMenu(&(*pkg->window));
			break;

			// Execução do algoritmo
			case WINSTATE_EXEC:
				// Limpa a tela e redesenha o chão
				if (receivedSIGWINCH) {
					sprintf(buffer, "Novo terminal %d,%d", (*pkg->window)->termColumns, (*pkg->window)->termRows);
					updateConsole(buffer, &(*pkg->window));

					// Redesenha toda a janela
					printf(ANSI_CLEAR_TERMINAL);
					updateWindowSize(&(*pkg->window));
					updateWarning(&(*pkg->window));

					// Draw
					drawFloor(&(*pkg->window));
					drawConsoleFloor(&(*pkg->window));
					drawConsoleFrame(pkg->window);
					drawFloor(&(*pkg->window));
					// Depois as bordas
					drawFrame(&(*pkg->window));

					receivedSIGWINCH = false;
				}
				
				// Trava a edição da janela enquanto estamos desenhando o mapa
				pthread_mutex_lock(&((*pkg->window))->WriteLock);

				// Desenha primeiro as coisas de dentro
				drawConsole(pkg->window);
				

				// Desenha o chão para evitar smearing
				drawFloor(&(*pkg->window));
				
				// Trava o agente antes de desenhar o campo de visão
				pthread_mutex_lock(&((*pkg->window))->UpdateLock);
				if ((*pkg->agent) != NULL) {
					(*pkg->agent)->drawFOV(pkg->agent, pkg->window);
				}
				pthread_mutex_unlock(&((*pkg->window))->UpdateLock);

				drawBin(pkg->window);
				drawTrash(&(*pkg->trash), &(*pkg->window));

				// Desenha as coisas na borda do campo
				drawFPS(pkg->window);
				drawTPS(pkg->window);
				drawHold(pkg->window);
				drawSteps(pkg->window);
				drawPause(pkg->window);
				drawTime(pkg->window);
				drawRank((*pkg->agent)->score, pkg->window);

				pthread_mutex_lock(&((*pkg->window))->UpdateLock);
				if ((*pkg->agent) != NULL) {
					(*pkg->agent)->draw(pkg->agent, pkg->window);
				}
				pthread_mutex_unlock(&((*pkg->window))->UpdateLock);
				
				break;
			case WINSTATE_DONE:
			case WINSTATE_CLEAN:
				break;
		}




		// Desenha tudo no terminal
		/* Calcula o tempo para dormir
		* Cálculo: 1/FPS - (actFrame - lastFrame)
		*	1/FPS = Tempo para cada quadro em ms
		*	(actFrame - lastFrame) = tempo gasto por este quadro em ms
		*/
		
		// Verifica o quanto precisa dormir para o próximo quadro
		stopCronometer(&(*pkg->window)->frameCount);
		frametimeAct = cronToInt((&((*pkg->window)->frameCount)));

		if (frametimeLimit < 0) frametimeLimit = 0;
		sleepValue = frametimeLimit - frametimeAct;
		// Destrava para continuar processando
		pthread_mutex_unlock(&(*pkg->window)->WriteLock);

		fflush(stdout);
		
		
		// Dorme os X ms
		if ((*pkg->window)->fpsLimit && (sleepValue) > 0) {
			usleep(sleepValue);
		}

		// Após dormir, recalcula o fps
		stopCronometer(&(*pkg->window)->frameCount);
		frametimeAct = cronToInt((&((*pkg->window)->frameCount)));
		
		// Guarda o framerate atual em window->fps
		// frametimeAct deve estar em sec
		// +1 = A conta sempre dá no limite do quadro anterior
		(*pkg->window)->fps = (1.0f / ((float)frametimeAct/1000000.0f)) + 1;

	} while((*pkg->window)->drawWorker);
	return NULL;
}

void* updateWorker(void* workerPKG) {
	// Processa o agente
	WorkerPKG* pkg = (WorkerPKG*)workerPKG;

	if (!workerPKG) return NULL;
	pthread_setname_np(updateThread, "Update Worker");

	// Em uSec
	int64_t tickTimeLimit;
	int64_t tickTimeAct;
	int64_t sleepValue;
	do {
		pthread_mutex_lock(&((*pkg->window))->UpdateStart);
		// Recalcula o limite de tps a cada quadro
		tickTimeLimit = (1.0f/(*pkg->window)->tpsLimit)*1000000;
		

		// Pega o tempo do início deste quadro
		startCronometer(&(*pkg->window)->tpsCount);


		switch ((*pkg->window)->state) {

			// Execução do algoritmo
			case WINSTATE_EXEC:
				// Só continua se pausar
				if (!((*pkg->window)->pause)) {
					pthread_mutex_lock(&((*pkg->window))->UpdateLock);

					// Se não tem mais nenhum lixo, pausa o programa
					if (!(*pkg->trash) && !(*pkg->agent)->holdItem) {
						(*pkg->window)->pause = true;
					} else {
						(*pkg->agent)->run(pkg->agent, &(*pkg->trash), &(*pkg->window));
						(*pkg->window)->steps++;
					}
					pthread_mutex_unlock(&((*pkg->window))->UpdateLock);
				}
				break;
			case WINSTATE_MENU:
			case WINSTATE_OPTIONS:
			case WINSTATE_DONE:
			case WINSTATE_HELP:
			case WINSTATE_CLEAN:
				break;
		}	
		// Verifica o quanto precisa dormir para o próximo tick
		stopCronometer(&(*pkg->window)->tpsCount);
		tickTimeAct = cronToInt((&((*pkg->window)->tpsCount)));
		sleepValue = tickTimeLimit - tickTimeAct;
		pthread_mutex_unlock(&((*pkg->window))->UpdateStart);

		// Dorme os X ms
		if ((*pkg->window)->tpsLimit && (sleepValue) > 0) {
			usleep(sleepValue);
		}

		// Após dormir, recalcula o fps
		stopCronometer(&(*pkg->window)->tpsCount);
		tickTimeAct = cronToInt((&((*pkg->window)->tpsCount)));
		// Guarda o framerate atual em window->fps
		// frametimeAct deve estar em sec
		// +1 = A conta sempre dá no limite do quadro anterior
		(*pkg->window)->tps = (1.0f / ((float)tickTimeAct/1000000.0f)) + 1;
		// Aumenta o execTimeUs da janela
		if (!(*pkg->window)->pause) {
			(*pkg->window)->execTimeUs += tickTimeAct;
		}

	} while ((*pkg->window)->updateWorker);
	return NULL;
}
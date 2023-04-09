//struct timeval start, act;
//struct timeval lastframe, actframe;

//extern unsigned short int dontRender;


#include <stdio.h>			// printf for SIGINT
#include <stdlib.h>			// exit()
#include <Ansi.h>			// ANSI chars
#include <Terminal.h> 		// Terminal utils - Window and draw
#include <Lixo.h>			// Lixo
#include <Agent/Simple.h>	// Agente simples
#include <signal.h>			// Sinais
#include <pthread.h>		// Threads
#include <Workers.h>		// Workers
#include <Menu.h>			// Main menu

extern bool receivedSIGWINCH;

void processSIGINT() {
	// Mostra o cursor em caso de emergência(SIGINT, SIGKILL, etc);
	printf(ANSI_SHOW_CURSOR);

	printf("Saindo\n");
	disableRawMode();
	exit(0);
}

int main() {
	enableRawMode();
	// Inicia as opções padrão
	defaultMenuOptions(&defaultOptions);


	// Gera o básico
	Window *menuwindow = newWindow(MAINMENU_ROW, MAINMENU_COL, DEFAULT_TITLE, FPS);

	// Trava o updateWorker
	pthread_mutex_lock(&(menuwindow->UpdateStart));

	// Sinais
	signal(SIGWINCH, handleSIGWINCH);
	signal(SIGINT, processSIGINT); // Mostra o cursor se o user apertar ^[C

	// Primeira leva: Desenha tudo antes de ir pro mainloop
	updateTitle("MainMenu", &menuwindow);

	// Spawna as threads
	WorkerPKG workerPKG;
	workerPKG.window = &menuwindow;
	workerPKG.agent  = NULL;
	workerPKG.trash  = NULL;



	pthread_create(&renderThread, NULL, drawWorker, (void*)(&workerPKG));
	pthread_create(&inputThread, NULL, inputWorker, (void*)(&workerPKG));
	pthread_create(&updateThread, NULL, updateWorker, (void*)(&workerPKG));

	// Pega de volta as threads
	pthread_join(renderThread, NULL);
	pthread_join(inputThread, NULL);
	pthread_join(updateThread, NULL);

}
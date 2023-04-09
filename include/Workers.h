/*
 * Workers.h - Threads utilizadas pelo loop principal
 *  Created on: 1 de abr. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_WORKERS_H_
#define INCLUDE_WORKERS_H_

#include <Terminal.h>       // Window
#include <Agent/Agent.h>    // Agent
#include <Lixo.h>           // Trash

typedef struct __WorkerPKG__ {
    Window** window;
    Agent**  agent;
    Trash**  trash;
} WorkerPKG;

// FPS
#define FPS_MAX_LIMIT 9999

// Threads
extern pthread_t inputThread;
extern pthread_t renderThread;
extern pthread_t updateThread;

void* inputWorker(void* workerPKG);
void* drawWorker(void* workerPKG);
void* updateWorker(void* workerPKG);

#endif
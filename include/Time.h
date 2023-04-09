/*
 * tempo.h
 *
 *  Created on: 16 de jun. de 2022
 *      Author: uotlaf
 */

#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_
#include "sys/time.h"
#include <stdlib.h>
#include <stdint.h>

// Controlador de tempo de cada parte do programa
typedef struct __Cronometer__
{
	struct timeval start, final; // Timeval que vai ser convertido pra long
} Cronometer;


// Conversões
#define timevalToLong(timev) (timev.tv_sec*1000000 + timev.tv_usec)
#define cronToInt(cron)     (timevalToLong(cron->final) - timevalToLong(cron->start))
int64_t cronToSec(Cronometer* cron);
int64_t cronToMSec(Cronometer* cron);
int64_t cronToUSec(Cronometer* cron);



Cronometer* newCronometer();
void startCronometer(Cronometer* cron);
void stopCronometer(Cronometer* cron);

#endif /* INCLUDE_TEMPO_H_ */
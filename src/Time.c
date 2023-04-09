/*
 * tempo.c
 *
 *  Created on: 16 de jun. de 2022
 *      Author: uotlaf
 */


#include <Time.h>

Cronometer* newCronometro() {
	Cronometer* cron = (Cronometer*)malloc(sizeof(Cronometer));
	if (cron == NULL) {
		return NULL;
	}
	return cron;
}

void startCronometer(Cronometer* cron) {
    if (!cron) return;
    gettimeofday(&cron->start, NULL); // Inicia o cronômetro
}

void stopCronometer(Cronometer* cron) {
	if (cron == NULL) {
		return;
	}
	gettimeofday(&cron->final, NULL);
}

int64_t cronToSec(Cronometer* cron) {
	return cronToInt(cron)/1000000;
}

int64_t cronToMSec(Cronometer* cron) {
	return (cronToInt(cron)%1000000/1000);
}

int64_t cronToUSec(Cronometer* cron) {
    // Retorna somente os Usec
	if (cronToInt(cron) < 1000) {
		return cronToInt(cron);
	}
	return cronToInt(cron)-(cronToMSec(cron)*1000)-(cronToSec(cron)*1000000);
}
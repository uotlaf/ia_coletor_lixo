/*
 * Menu.h
 *
 *  Created on: 31 de mar. de 2023
 *      Author: uotlaf
 */

#ifndef INCLUDE_MENU_H_
#define INCLUDE_MENU_H_
#include <stdint.h>
#include <Terminal.h>

#define MAINMENU_ROW 6
#define MAINMENU_COL 40

#define SETTINGSMENU_ROW 7
#define SETTINGSMENU_COL 40

#define HELPMENU_ROW 8
#define HELPMENU_COL 40

#define DEFAULT_TITLE       "Default Title"
#define TEXT_MENU_TITLE     "Menu principal"
#define TEXT_MENU_SA        "Iniciar com Reativo Simples"
#define TEXT_MENU_MA        "Iniciar com Baseado em Modelos"
#define TEXT_MENU_OA        "Iniciar com Baseado em Objetivos"
#define TEXT_MENU_UA        "Iniciar com Baseado em Utilidades"
#define TEXT_MENU_SETTINGS  "Configuracoes"
#define TEXT_MENU_SHORTCUTS "Atalhos do teclado"

#define TEXT_OPTIONS_TITLE          "Configuracoes"
#define TEXT_OPTIONS_AGENT_WIDTH    "Largura do campo do agente"
#define TEXT_OPTIONS_AGENT_HEIGHT   "Altura do campo do agente"
#define TEXT_OPTIONS_FPS            "Framerate maximo"
#define TEXT_OPTIONS_TPS            "Tickrate do agente"
#define TEXT_OPTIONS_SEED           "Semente de geracao de lixo"
#define TEXT_OPTIONS_TRASH_REC      "Quantidade de lixo reciclavel"
#define TEXT_OPTIONS_TRASH_ORG      "Quantidade de lixo organico"
#define TEXT_ESC                    "Aperte ESC para voltar"

#define TEXT_HELP_TITLE             "Atalhos"
#define TEXT_HELP_PAUSE_AGENT       "Pausar/Continuar agente"
#define TEXT_HELP_PAUSE_AGENT_BUT   "<space>"
#define TEXT_HELP_FPS_LIMIT         "Hab/Des limite de FPS"
#define TEXT_HELP_FPS_LIMIT_BUT     "<f>"
#define TEXT_HELP_TPS_LIMIT         "Hab/Des limite de Ticks"
#define TEXT_HELP_TPS_LIMIT_BUT     "<t>"
#define TEXT_HELP_ADD_FPS           "Aumentar limite de FPS/TPS"
#define TEXT_HELP_ADD_FPS_BUT       "<+/w>"
#define TEXT_HELP_SUB_FPS           "Diminuir limite de FPS/TPS"
#define TEXT_HELP_SUB_FPS_BUT       "<-/q>"
#define TEXT_HELP_TOG_CONSOLE       "Habilitar/Desabilitar Console"
#define TEXT_HELP_TOG_CONSOLE_BUT   "<c>"
#define TEXT_HELP_FPS               "FPS = Quadros renderizados/s"
#define TEXT_HELP_TPS               "TPS = Loops do agente/s"


typedef struct __menuOptions__ {
    uint16_t fieldRows, fieldColumns;   // Tamanho do campo do agente
    uint16_t fpsLimit, tpsLimit;        // Velocidade de desenho e de atualização do agente
    int64_t seed;                       // Semente para gerar os lixos
    uint8_t  trashRec, trashOrg;        // Quantidade de lixo reciclável e orgânico
} MenuOptions;

extern MenuOptions defaultOptions;
extern uint8_t menuSelected;

void defaultMenuOptions(MenuOptions* options);

// Draw
void drawMainMenu(Window** window);
void drawMainMenuSelected(Window** window);
void drawSettingsMenu(Window** window);
void drawSettingsMenuSelected(Window** window);
void drawHelpMenu(Window** window);
void drawHelpMenuSelected(Window** window);

#endif
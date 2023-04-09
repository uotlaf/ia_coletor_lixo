/*
 * Menu.c
 *
 *  Created on: 7 de abr. de 2023
 *      Author: uotlaf
 */


#include <Menu.h>
#include <Ansi.h>       // ANSI_COLOR_NONE
#include <UTF_char.h>   // UTF chars
#include <stdio.h>      // sprintf
#include <string.h>     // strlen()

uint8_t menuSelected = 0;
MenuOptions defaultOptions;


void defaultMenuOptions(MenuOptions* options) {
    options->fieldColumns = FIELD_WIDTH;
    options->fieldRows    = FIELD_HEIGHT;
    options->fpsLimit     = FPS;
    options->seed         = SEED;
    options->tpsLimit     = TPS;
    options->trashOrg     = TRASH_QUANT_ORG;
    options->trashRec     = TRASH_QUANT_REC;
}

// Draw
void drawMainMenu(Window** window) {
    // Desenha o menu principal com a seleção atual
    /*
     * ╔══════════Menu Principal════════════╗
     * ║ Iniciar com Reativo Simples        ║
     * ║ Iniciar com Baseado em Modelos     ║
     * ║ Iniciar com Baseado em Objetivos   ║
     * ║ Iniciar com Baseado em Utilidades  ║
     * ║ Configurações                      ║
     * ║ Atalhos                            ║
     * ╚════════════════════════════════════╝
     *
    */
    if (!window || !(*window)) return;

    uint8_t titleX = ((*window)->fieldColumns/2) - (strlen(TEXT_MENU_TITLE)/2);

    // Reinicia para a cor padrão do terminal
    drawObject(ANSI_COLOR_RESET, ANSI_COLOR_RESET, ANSI_COLOR_RESET, 0, 0, window);

    // Título
    drawObject(TEXT_MENU_TITLE, (*window)->colors.fontForeground, (*window)->colors.boxBackground, titleX, -1, window);

    // Primeiro o topo
    drawObject(UTF_TOPLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, -1, window);
    for (int x = 0; x < titleX; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    for (int x = titleX + strlen(TEXT_MENU_TITLE); x < (*window)->fieldColumns; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    drawObject(UTF_TOPRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, -1, window);
    
    // Primeira opção - Reativo Simples
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 0, window);
    drawObject(TEXT_MENU_SA, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 0, window);
    for (int x = strlen(TEXT_MENU_SA); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 0, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 0, window);

    // Segunda opção - Baseado em Modelos
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 1, window);
    drawObject(TEXT_MENU_MA, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 1, window);
    for (int x = strlen(TEXT_MENU_MA); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 1, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 1, window);

    // Terceira opção - Baseado em Objetivos
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 2, window);
    drawObject(TEXT_MENU_OA, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 2, window);
    for (int x = strlen(TEXT_MENU_OA); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 2, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 2, window);

    // Quarta opção - Baseado em Utilidades
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 3, window);
    drawObject(TEXT_MENU_UA, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 3, window);
    for (int x = strlen(TEXT_MENU_UA); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 3, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 3, window);

    // Quinta opção - Configurações
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 4, window);
    drawObject(TEXT_MENU_SETTINGS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 4, window);
    for (int x = strlen(TEXT_MENU_SETTINGS); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 4, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 4, window);

    // Sexta opção - Atalhos
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 5, window);
    drawObject(TEXT_MENU_SHORTCUTS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 5, window);
    for (int x = strlen(TEXT_MENU_SHORTCUTS); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 5, window);
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 5, window);


    // Parte de baixo
    drawObject(UTF_BOTTOMLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, (*window)->fieldRows, window);
    for (int x = 0; x < MAINMENU_COL; x++) {
        drawObject(UTF_BOTTOM, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, (*window)->fieldRows, window);
    }
    drawObject(UTF_BOTTOMRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, (*window)->fieldRows, window);

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

    drawMainMenuSelected(window);
};

void drawMainMenuSelected(Window** window) {
    if (!window || !(*window)) return;

    // Primeira opção
    switch(menuSelected) {
        case 0:
            drawObject(TEXT_MENU_SA, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 0, window);
            for (int x = strlen(TEXT_MENU_SA); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 0, window);
            }   
        break;
        case 1:
            drawObject(TEXT_MENU_MA, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 1, window);
            for (int x = strlen(TEXT_MENU_MA); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 1, window);
            }
        break;
        case 2:
            drawObject(TEXT_MENU_OA, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 2, window);
            for (int x = strlen(TEXT_MENU_OA); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 2, window);
            }
        break;
        case 3:
            drawObject(TEXT_MENU_UA, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 3, window);
            for (int x = strlen(TEXT_MENU_UA); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 3, window);
            }
        break;
        case 4:
            drawObject(TEXT_MENU_SETTINGS, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 4, window);
            for (int x = strlen(TEXT_MENU_SETTINGS); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 4, window);
        }
        break;
        case 5:
            drawObject(TEXT_MENU_SHORTCUTS, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 5, window);
            for (int x = strlen(TEXT_MENU_SHORTCUTS); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 5, window);
        }
        break;
    }

}

void drawSettingsMenu(Window** window) {
    // Desenha o menu de configuração com a seleção atual
    /* 
     * ╔═══════════Configuracoes════════════╗
     * ║ Largura do campo do agente    <020>║
     * ║ Altura do campo do agente     <020>║
     * ║ Framerate de desenho          <060>║
     * ║ Tickrate do agente            <010>║
     * ║ Semente de geração de lixo    <...>║
     * ║ Quantidade de lixo reciclavel <005>║
     * ║ Quantidade de lixo organico   <010>║
     * ╚Esc para voltar═════════════════════╝
     * 
    */
    if (!window || !(*window)) return;
    char buf[6];

    uint8_t titleX = ((*window)->fieldColumns/2) - (strlen(TEXT_OPTIONS_TITLE)/2);

    // Reinicia para a cor padrão do terminal
    drawObject(ANSI_COLOR_RESET, ANSI_COLOR_RESET, ANSI_COLOR_RESET, 0, 0, window);

    // Título
    drawObject(TEXT_OPTIONS_TITLE, (*window)->colors.fontForeground, (*window)->colors.boxBackground, titleX, -1, window);

    // Primeiro o topo
    drawObject(UTF_TOPLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, -1, window);
    for (int x = 0; x < titleX; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    for (int x = titleX + strlen(TEXT_OPTIONS_TITLE); x < (*window)->fieldColumns; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    drawObject(UTF_TOPRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, -1, window);
    
    // Primeira opção - Largura do campo do agente
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 0, window);
    drawObject(TEXT_OPTIONS_AGENT_WIDTH, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 0, window);
    for (int x = strlen(TEXT_OPTIONS_AGENT_WIDTH); x < (*window)->fieldColumns - 4; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 0, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 0, window);
    sprintf(buf, "%03d", defaultOptions.fieldColumns);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 0, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 0, window);

    // Segunda opção - Altura do campo do agente
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 1, window);
    drawObject(TEXT_OPTIONS_AGENT_HEIGHT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 1, window);
    for (int x = strlen(TEXT_OPTIONS_AGENT_HEIGHT); x < (*window)->fieldColumns - 5; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 1, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 1, window);
    sprintf(buf, "%03d", defaultOptions.fieldRows);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 1, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 1, window);

    // Terceira opção - FPS do desenho
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 2, window);
    drawObject(TEXT_OPTIONS_FPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 2, window);
    for (int x = strlen(TEXT_OPTIONS_FPS); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 2, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 2, window);
    sprintf(buf, "%03d", defaultOptions.fpsLimit);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 2, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 2, window);

    // Quarta opção - Tickrate do agente
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 3, window);
    drawObject(TEXT_OPTIONS_TPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 3, window);
    for (int x = strlen(TEXT_OPTIONS_TPS); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 3, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 3, window);
    sprintf(buf, "%03d", defaultOptions.tpsLimit);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 3, window);
    
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 3, window);

    // Quinta opção - Semente de geração de lixo
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 4, window);
    drawObject(TEXT_OPTIONS_SEED, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 4, window);
    for (int x = strlen(TEXT_OPTIONS_SEED); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 4, window);
    }
    drawObject("<...>", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 4, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 4, window);

    // Sexta opção - Quantidade de lixo reciclável
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 5, window);
    drawObject(TEXT_OPTIONS_TRASH_REC, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 5, window);
    for (int x = strlen(TEXT_OPTIONS_TRASH_REC); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 5, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 5, window);
    sprintf(buf, "%03d", defaultOptions.trashRec);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 5, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 5, window);

    // Sétima opção - Quantidade de lixo orgânico
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 6, window);
    drawObject(TEXT_OPTIONS_TRASH_ORG, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 6, window);
    for (int x = strlen(TEXT_OPTIONS_TRASH_ORG); x < (*window)->fieldColumns; x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 6, window);
    }
    drawObject("<   >", (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 5, 6, window);
    sprintf(buf, "%03d", defaultOptions.trashOrg);
    drawObject(buf, (*window)->colors.fontForeground, (*window)->colors.boxBackground, (*window)->fieldColumns - 4, 6, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 6, window);



    // Parte de baixo
    drawObject(UTF_BOTTOMLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, (*window)->fieldRows, window);
    for (int x = (sizeof(TEXT_ESC) - 1); x < MAINMENU_COL; x++) {
        drawObject(UTF_BOTTOM, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, (*window)->fieldRows, window);
    }
    drawObject(UTF_BOTTOMRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, (*window)->fieldRows, window);
    
    // Texto de baixo
    drawObject(TEXT_ESC, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, (*window)->fieldRows, window);

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
    drawSettingsMenuSelected(window);
};

void drawSettingsMenuSelected(Window** window) {
    if (!window || !(*window)) return;
    char buf[6];

    // Primeira opção
    switch(menuSelected) {
        case 0: // Largura
            drawObject(TEXT_OPTIONS_AGENT_WIDTH, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 0, window);
            for (int x = strlen(TEXT_OPTIONS_AGENT_WIDTH); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 0, window);
            }   
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 0, window);
            sprintf(buf, "%03d", defaultOptions.fieldColumns);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 0, window);
        break;
        case 1: // Altura
            drawObject(TEXT_OPTIONS_AGENT_HEIGHT, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 1, window);
            for (int x = strlen(TEXT_OPTIONS_AGENT_HEIGHT); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 1, window);
            }
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 1, window);
            sprintf(buf, "%03d", defaultOptions.fieldRows);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 1, window);
        break;
        case 2: // Framerate
            drawObject(TEXT_OPTIONS_FPS, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 2, window);
            for (int x = strlen(TEXT_OPTIONS_FPS); x < MAINMENU_COL; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 2, window);
            }
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 2, window);
            sprintf(buf, "%03d", defaultOptions.fpsLimit);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 2, window);
        break;
        case 3: // Tickrate
            drawObject(TEXT_OPTIONS_TPS, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 3, window);
            for (int x = strlen(TEXT_OPTIONS_TPS); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 3, window);
            }
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 3, window);
            sprintf(buf, "%03d", defaultOptions.tpsLimit);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 3, window);
        break;
        case 4: // Semente
            drawObject(TEXT_OPTIONS_SEED, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 4, window);
            for (int x = strlen(TEXT_OPTIONS_SEED); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 4, window);
        }
            drawObject("<...>", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 4, window);
        break;
        case 5: // Quantidade de lixo reciclável
            drawObject(TEXT_OPTIONS_TRASH_REC, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 5, window);
            for (int x = strlen(TEXT_OPTIONS_TRASH_REC); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 5, window);
        }
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 5, window);
            sprintf(buf, "%03d", defaultOptions.trashRec);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 5, window);
        break;
        case 6: // Quantidade de lixo orgânico
            drawObject(TEXT_OPTIONS_TRASH_ORG, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, 0, 6, window);
            for (int x = strlen(TEXT_OPTIONS_TRASH_ORG); x < (*window)->fieldColumns; x++) {
                drawObject(" ", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, x, 6, window);
        }
            drawObject("<   >", (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 5, 6, window);
            sprintf(buf, "%03d", defaultOptions.trashOrg);
            drawObject(buf, (*window)->colors.consoleForeground, (*window)->colors.consoleBackground, (*window)->fieldColumns - 4, 6, window);
        break;
    }

}

void drawHelpMenu(Window** window) {
        // Desenha o menu de configuração com a seleção atual
    /* 
     * ╔════════════════Ajuda═══════════════╗
     * ║ Pausar/Continuar agente     <space>║
     * ║ Hab/Des limite de FPS         <f>  ║
     * ║ Hab/Des limite de Ticks       <t>  ║
     * ║ Aumentar limite de FPS        <+>  ║
     * ║ Diminuir limite de FPS        <->  ║
     * ║ Habilitar/Desabilitar Console <c>  ║
     * ║ FPS = Quadros renderizados/s       ║
     * ║ TPS = Loops do agente/s            ║
     * ╚Esc para voltar═════════════════════╝
     * 
    */
    if (!window || !(*window)) return;

    uint8_t titleX = ((*window)->fieldColumns/2) - (strlen(TEXT_HELP_TITLE)/2);

    // Reinicia para a cor padrão do terminal
    drawObject(ANSI_COLOR_RESET, ANSI_COLOR_RESET, ANSI_COLOR_RESET, 0, 0, window);

    // Título
    drawObject(TEXT_HELP_TITLE, (*window)->colors.fontForeground, (*window)->colors.boxBackground, titleX, -1, window);

    // Primeiro o topo
    drawObject(UTF_TOPLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, -1, window);
    for (int x = 0; x < titleX; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    for (int x = titleX + strlen(TEXT_HELP_TITLE); x < (*window)->fieldColumns; x++) {
        drawObject(UTF_TOP, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, -1, window);
    }
    drawObject(UTF_TOPRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, -1, window);
    
    // Primeira opção - Pausar/Continuar agente
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 0, window);
    drawObject(TEXT_HELP_PAUSE_AGENT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 0, window);
    for (int x = strlen(TEXT_HELP_PAUSE_AGENT); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_PAUSE_AGENT_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 0, window);    
    }
    drawObject(TEXT_HELP_PAUSE_AGENT_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_PAUSE_AGENT_BUT), 0, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 0, window);

    // Segunda opção - Hab/Des limite de FPS
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 1, window);
    drawObject(TEXT_HELP_FPS_LIMIT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 1, window);
    for (int x = strlen(TEXT_HELP_FPS_LIMIT); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_FPS_LIMIT_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 1, window);    
    }
    drawObject(TEXT_HELP_FPS_LIMIT_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_FPS_LIMIT_BUT), 1, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 1, window);

    // Terceira opção - Hab/Des limite de Ticks
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 2, window);
    drawObject(TEXT_HELP_TPS_LIMIT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 2, window);
    for (int x = strlen(TEXT_HELP_TPS_LIMIT); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_TPS_LIMIT_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 2, window);    
    }
    drawObject(TEXT_HELP_TPS_LIMIT_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_TPS_LIMIT_BUT), 2, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 2, window);

    // Quarta opção - Aumentar limite de FPS
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 3, window);
    drawObject(TEXT_HELP_ADD_FPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 3, window);
    for (int x = strlen(TEXT_HELP_ADD_FPS); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_ADD_FPS_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 3, window);    
    }
    drawObject(TEXT_HELP_ADD_FPS_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_ADD_FPS_BUT), 3, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 3, window);

    // Quinta opção - Diminuir limite de FPS
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 4, window);
    drawObject(TEXT_HELP_SUB_FPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 4, window);
    for (int x = strlen(TEXT_HELP_SUB_FPS); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_SUB_FPS_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 4, window);    
    }
    drawObject(TEXT_HELP_SUB_FPS_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_SUB_FPS_BUT), 4, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 4, window);

    // Sexta opção - Habilitar/Desabilitar Console
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 5, window);
    drawObject(TEXT_HELP_TOG_CONSOLE, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 5, window);
    for (int x = strlen(TEXT_HELP_TOG_CONSOLE); x < ((*window)->fieldColumns)-strlen(TEXT_HELP_TOG_CONSOLE_BUT); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 5, window);    
    }
    drawObject(TEXT_HELP_TOG_CONSOLE_BUT, (*window)->colors.fontForeground, (*window)->colors.boxBackground, ((*window)->fieldColumns)-strlen(TEXT_HELP_TOG_CONSOLE_BUT), 5, window);
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 5, window);

    // Sétima opção - FPS
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 6, window);
    drawObject(TEXT_HELP_FPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 6, window);
    for (int x = strlen(TEXT_HELP_FPS); x < ((*window)->fieldColumns); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 6, window);    
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 6, window);

    // Oitava opção - FPS
    drawObject(UTF_LEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, 7, window);
    drawObject(TEXT_HELP_TPS, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, 7, window);
    for (int x = strlen(TEXT_HELP_TPS); x < ((*window)->fieldColumns); x++) {
        drawObject(" ", (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, 7, window);    
    }
    drawObject(UTF_RIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, 7, window);

    // Parte de baixo
    drawObject(UTF_BOTTOMLEFT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, -1, (*window)->fieldRows, window);
    for (int x = (sizeof(TEXT_ESC) - 1); x < MAINMENU_COL; x++) {
        drawObject(UTF_BOTTOM, (*window)->colors.boxForeground, (*window)->colors.boxBackground, x, (*window)->fieldRows, window);
    }
    drawObject(UTF_BOTTOMRIGHT, (*window)->colors.boxForeground, (*window)->colors.boxBackground, (*window)->fieldColumns, (*window)->fieldRows, window);
    
    // Texto de baixo
    drawObject(TEXT_ESC, (*window)->colors.fontForeground, (*window)->colors.boxBackground, 0, (*window)->fieldRows, window);

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
    drawHelpMenuSelected(window);
}

void drawHelpMenuSelected(Window** window) {

}
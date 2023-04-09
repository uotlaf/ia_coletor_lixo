# Configuração do programa
AGENTE = 0
AGENTE_TPS = 60
CAMPO_ALTURA = 20
CAMPO_LARGURA = 20
QUANT_LIXO_REC = 5
QUANT_LIXO_ORG = 10
VALOR_MAXIMO = 5
SEED = 1
FPS = 60
INSTANT_DRAW = 1   	# Desenha os objetos de cada vez, em vez de esperar o quadro terminar
ENABLE_CONSOLE = 0 	# Liga o console por padrão
CONSOLE_W = 40	 	# Largura do console. A altura = altura do campo
DRAW_SHADOW = 1

PROGRAM_NAME = Agente

ifeq ($(PLATFORM),windows64_gcc)
CC=x86_64-w64-mingw32-gcc
CFLAGS += -DWINDOWS64 -static
endif


# Início do makefile
PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SOURCE   = $(PROJECT_ROOT)/src
INCLUDE  = $(PROJECT_ROOT)include
OBJECTS  = \
	$(SOURCE)/main.c \
	$(SOURCE)/Terminal.c \
	$(SOURCE)/Lixo.c \
	$(SOURCE)/Agent/Agent.c \
	$(SOURCE)/Agent/Simple.c \
	$(SOURCE)/Workers.c \
	$(SOURCE)/Time.c \
	$(SOURCE)/Menu.c
BUILD_DIR= ./

# Bibliotecas ligadas ao programa
LIBS = 

ifneq ($(DRAW_SHADOW), )
CFLAGS += -DDRAW_SHADOW
endif

ifneq ($(LAG_uS), )
CFLAGS += -DLAG_uS=$(LAG_uS)
endif

ifneq ($(INSTANT_DRAW), )
	CFLAGS += -DINSTANT_DRAW
endif

# Flags para o pré-processador
CFLAGS  += $(LIBS) \
	-DAGENTE=$(AGENTE) \
	-DFIELD_HEIGHT=$(CAMPO_ALTURA) \
	-DFIELD_WIDTH=$(CAMPO_LARGURA) \
	-DTRASH_QUANT_REC=$(QUANT_LIXO_REC) \
	-DTRASH_QUANT_ORG=$(QUANT_LIXO_ORG) \
	-DMAX_VALUE=$(VALOR_MAXIMO) \
	-DENABLE_CONSOLE=$(ENABLE_CONSOLE) \
	-DCONSOLE_W=$(CONSOLE_W) \
	-DFPS=$(FPS) \
	-DTPS=$(AGENTE_TPS) \
	-DDRAW_CONSOLE=$(DESENHAR_CONSOLE) \
	-DSEED=$(SEED)


ifeq ($(BUILD_MODE),debug)
	CFLAGS += -fdiagnostics-color=always -Wall -pg -g -fsanitize=bounds -fsanitize-undefined-trap-on-error -fstack-protector 
else
	CFLAGS += -O3 \
		-Wall \
		-fsanitize=bounds \
		-fsanitize-undefined-trap-on-error \
		-fstack-protector \
		-D_FORTIFY_SOURCE=2 \
		-fgraphite-identity \
		-floop-nest-optimize \
		-fdevirtualize-at-ltrans \
		-fipa-pta \
		-fno-semantic-interposition \
		-flto=13
endif

all: program # link

program:
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(PROGRAM_NAME) -I $(INCLUDE) $(CFLAGS) $(OBJECTS)

	
clean:
	rm -fr $(BUILD_DIR)/$(PROGRAM_NAME) build/*

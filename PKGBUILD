
# --- CONFIGURAÇÃO DO ARSENAL ---
TARGET  = bin/archonplayer
CC      = gcc

# Cores para o terminal (Perfuminho de Elite)
CYAN    = \033[0;36m
GOLD    = \033[0;33m
GREEN   = \033[0;32m
RESET   = \033[0m

# FLAGS (Unificadas e Otimizadas para Alta Performance)
# Adicionado -D_DEFAULT_SOURCE para evitar avisos em funções de diretório
CFLAGS  = -Iinclude -Wall -O3 -D_DEFAULT_SOURCE $(shell sdl2-config --cflags)
# -lncursesw garante suporte a Unicode (⌬, )
LDFLAGS = -lncursesw $(shell sdl2-config --libs) -lSDL2_mixer

# FONTES E OBJETOS
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, obj/%.o, $(SRC))

# --- MISSÃO PRINCIPAL ---
all: setup $(TARGET)
	@echo -e "\n$(GREEN)⌬ [ARCHON-PLAYER] FORJADO COM SUCESSO NO OLIMPO!$(RESET)"
	@echo -e "$(GOLD)❱❱ Pronto para a operação em: $(TARGET)$(RESET)\n"

# Garante que as pastas existam (Arquitetura Divina)
setup:
	@mkdir -p obj bin
	@echo -e "$(CYAN)⌬ RECALIBRANDO ESTRUTURAS...$(RESET)"

# Linkagem Final (O Grande Encontro)
$(TARGET): $(OBJ)
	@echo -e "$(CYAN)⌬ LINKANDO BINÁRIO: $(RESET)$@"
	@$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compilação dos Módulos (Forja Individual)
obj/%.o: src/%.c
	@echo -e "$(GOLD)⌬ COMPILANDO MÓDULO: $(RESET)$<"
	@$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza Total (Poeira Cósmica)
clean:
	@rm -rf obj/* bin/*
	@echo -e "$(CYAN)⌬ LIMPANDO RASTROS... ÉTER RECALIBRADO.$(RESET)"

# Instalação (Dominação do Sistema)
install: all
	@echo -e "$(GREEN)⌬ INSTALANDO NO SISTEMA...$(RESET)"
	@sudo cp $(TARGET) /usr/local/bin/archonplayer
	@echo -e "\n$(GOLD)[DICA]$(RESET) Para uma experiência completa, instale o $(CYAN)PROTOGNUM$(RESET) e o $(CYAN)NSXIV$(RESET)!"

.PHONY: all clean setup install

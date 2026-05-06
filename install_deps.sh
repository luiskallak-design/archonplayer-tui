#!/bin/bash

# --- CORES PARA O TERMINAL ---
CYAN='\033[0;36m'
GOLD='\033[0;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

echo -e "${CYAN)⌬ [ARCHON-SYSTEM] INICIANDO RECALIBRAGEM DE DEPENDÊNCIAS...${RESET}"

# Detecção da Distro
if [ -f /etc/arch-release ]; then
    echo -e "${GOLD)❱❱ SISTEMA DETECTADO: ARCH LINUX${RESET}"
    sudo pacman -S --needed --noconfirm sdl2 sdl2_mixer ncurses qterminal nsxiv chromium

elif [ -f /etc/debian_version ]; then
    echo -e "${GOLD)❱❱ SISTEMA DETECTADO: DEBIAN/UBUNTU/MINT${RESET}"
    sudo apt update
    sudo apt install -y libsdl2-dev libsdl2-mixer-dev libncursesw5-dev qterminal nsxiv chromium-browser

elif [ -f /etc/fedora-release ]; then
    echo -e "${GOLD)❱❱ SISTEMA DETECTADO: FEDORA${RESET}"
    sudo dnf install -y SDL2-devel SDL2_mixer-devel ncurses-devel qterminal nsxiv chromium

else
    echo -e "${RED)⌬ ERRO: DISTRO NÃO RECONHECIDA. POR FAVOR, INSTALE AS DEPENDÊNCIAS MANUALMENTE.${RESET}"
    exit 1
fi

echo -e "\n${GREEN)⌬ AMBIENTE PRONTO! AGORA É SÓ DAR 'MAKE' E SUBIR AO OLIMPO.${RESET}"

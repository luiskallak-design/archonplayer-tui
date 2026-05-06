# 🎧 ArchonPlayer-TUI v2.0

O **ArchonPlayer** é um sistema de entretenimento para terminal focado em estética cyberpunk e performance extrema. Além de áudio, agora integra visualização de imagens e edição de código sem interromper o fluxo sonoro.

## ✨ Diferenciais
*   **Visual:** Espectro de áudio dinâmico em tempo real (60 FPS).
*   **Navegação:** Explorador de arquivos inteligente com auto-next.
*   **Multimídia:** Suporte nativo para imagens (**nsxiv**) e textos (**nano**).
*   **Multitask:** Edição de arquivos em janelas independentes (**qterminal**).
*   **Personalização:** Temas **Emerald**, **Blood** e **Cyber** (Atalho C).

## 📸 Temas Disponíveis


| **Archon Blood** | **Archon Cyber** | **Archon Emerald** |
| :---: | :---: | :---: |
| ![Red](archonred.png) | ![Blue](archonblue.png) | ![Green](archongreen.png) |

## ⌨️ Comandos de Elite (Atalhos)


| Tecla | Ação | Descrição |
| :--- | :--- | :--- |
| `ENTER` | **Executar** | Abre pastas, toca áudio ou dispara nsxiv/nano |
| `ESPAÇO` | **Pause** | Interrompe/Retoma o sinal de áudio |
| `H` | **Home** | Salto dimensional direto para a Home |
| `Z` | **Portal** | Abre o navegador Chromium no DuckDuckGo |
| `C` | **Theme** | Recalibra as cores do sistema (3 esquemas) |
| `Q` | **Exit** | Encerra a operação e limpa o buffer |

## 🚀 Ritual de Instalação (Universal)

O ArchonPlayer possui um sistema de detecção automática para preparar sua forja, independente da sua distribuição Linux.

```bash
# 1. Acesse a forja
git clone https://github.com
cd archonplayer-tui

# 2. Recalibrar Dependências (O Pulo do Gato)
# Suporta: Arch, Debian, Ubuntu, Mint e Fedora
chmod +x install_deps.sh
./install_deps.sh

# 3. Compile (Nível Sênior)
make

# 4. Inicie a operação
./bin/archonplayer
```

---

## 📦 Componentes do Arsenal
Para a operação plena dos módulos multimídia, o script instalará:
* **Chromium** (Módulo Portal)
* **qterminal** (Módulo de Edição Independente)
* **nsxiv** (Módulo de Imagem)
* **Engines:** `SDL2`, `SDL2_mixer` e `ncursesw`

*(Opcional) Instalação Global:* `sudo make install`

## ⚖️ Licença
Este projeto está sob a licença **MIT**.

*Desenvolvido  por luiskallak-design*



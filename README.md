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

## 📦 Requisitos do Sistema

Para a operação plena dos módulos multimídia, certifique-se de ter:
* **Chromium** (Módulo Portal)
* **qterminal** (Módulo de Edição Independente)
* **nsxiv** (Módulo de Imagem)
* **Bibliotecas:** `SDL2`, `SDL2_mixer` e `ncursesw`

## 🚀 Forjando o Binário (Ritual do Olimpo)

```bash
# 1. Acesse a forja
cd archonplayer-tui

# 2. Compile (Nível Sênior)
make

# 3. Inicie a operação
./bin/archonplayer
```

*(Opcional) Instalação Global:* `sudo make install`

## ⚖️ Licença
Este projeto está sob a licença **MIT**.

*Desenvolvido por luiskallak-design*


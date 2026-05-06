#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* --- ESTRUTURAS E GLOBAIS --- */
typedef struct { char name[256]; int is_dir; } BrowseItem;
int spectrum_data[256], peak_data[256]; 
int offset = 0, selection = 0, playing_idx = -1, esquema_player = 0; 
double current_prog = 0; 
char current_path[1024];

/* --- UTILITÁRIOS SÊNIOR --- */
int check_ext(const char *name, const char *ext) {
    const char *dot = strrchr(name, '.');
    if (!dot) return 0;
    return (strcasecmp(dot, ext) == 0);
}

void spawn_external(const char *cmd, const char *arg, int novo_terminal) {
    if (fork() == 0) {
        setsid();
        int dn = open("/dev/null", O_RDWR);
        dup2(dn, 0); dup2(dn, 1); dup2(dn, 2); close(dn);
        
        if (novo_terminal) {
            // Abre o nano dentro do qterminal em uma nova janela
            execlp("qterminal", "qterminal", "-e", "nano", arg, (char *)NULL);
        } else {
            // Abre o nsxiv ou outro app gráfico diretamente
            execlp(cmd, cmd, arg, (char *)NULL);
        }
        _exit(0);
    }
}

const char* detect_gui_browser() {
    if (access("/usr/bin/chromium", X_OK) == 0) return "chromium";
    if (access("/usr/bin/brave", X_OK) == 0) return "brave";
    return "xdg-open";
}

void audio_callback(void *udata, Uint8 *stream, int len) {
    if (!stream || len == 0) return;
    for (int i = 0; i < 256 && i < len; i++) {
        int val = abs((int8_t)stream[i]) * 2.2;
        spectrum_data[i] = (spectrum_data[i] * 0.4) + (val * 0.6);
    }
}

void init_player_colors() {
    use_default_colors(); 
    if (esquema_player == 0) { // EMERALD
        init_pair(1, COLOR_GREEN, -1);   init_pair(2, COLOR_YELLOW, -1);
        init_pair(3, COLOR_CYAN, -1);    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    } else if (esquema_player == 1) { // BLOOD
        init_pair(1, COLOR_RED, -1);     init_pair(2, COLOR_YELLOW, -1);
        init_pair(3, COLOR_WHITE, -1);   init_pair(5, COLOR_BLACK, COLOR_RED);
    } else { // CYBER
        init_pair(1, COLOR_CYAN, -1);    init_pair(2, COLOR_MAGENTA, -1);
        init_pair(3, COLOR_WHITE, -1);   init_pair(5, COLOR_BLACK, COLOR_CYAN);
    }
    init_pair(4, COLOR_WHITE, -1); 
}

int scan_folder(BrowseItem *items_ptr) {
    DIR *dir = opendir("."); if (!dir) return 0;
    struct dirent *entry; int count = 0;
    while ((entry = readdir(dir)) != NULL && count < 100) {
        if (entry->d_name[0] == '.' && strcmp(entry->d_name, "..") != 0) continue;
        snprintf(items_ptr[count].name, 256, "%s", entry->d_name);
        items_ptr[count].is_dir = (entry->d_type == DT_DIR);
        count++;
    }
    closedir(dir); return count;
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    const char *gui_browser = detect_gui_browser();
    SDL_Init(SDL_INIT_AUDIO); Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);
    
    initscr(); start_color(); init_player_colors(); 
    noecho(); curs_set(0); keypad(stdscr, TRUE); 
    timeout(16); 
    
    BrowseItem items[100];
    int total_files = scan_folder(items);
    Mix_Music *music = NULL;

    if (argc > 1) {
        music = Mix_LoadMUS(argv[1]);
        if (music) { Mix_PlayMusic(music, 0); Mix_SetPostMix(audio_callback, NULL); playing_idx = 0; }
    }

    while(1) {
        int max_y, max_x; 
        getmaxyx(stdscr, max_y, max_x);
        getcwd(current_path, sizeof(current_path));
        
        int max_visible = max_y - 16; 
        if (max_visible < 1) max_visible = 1;
        int horizon_y = max_y - 7; 

        // --- LÓGICA DE AUTO-NEXT ---
        if (playing_idx != -1 && Mix_PlayingMusic() == 0 && Mix_PausedMusic() == 0) {
            int proximo = (playing_idx + 1) % total_files;
            int seguranca = 0;
            while (items[proximo].is_dir && seguranca < total_files) {
                proximo = (proximo + 1) % total_files;
                seguranca++;
            }
            if (!items[proximo].is_dir) {
                if (music) { Mix_HaltMusic(); Mix_FreeMusic(music); music = NULL; }
                music = Mix_LoadMUS(items[proximo].name);
                if (music) {
                    Mix_PlayMusic(music, 0);
                    Mix_SetPostMix(audio_callback, NULL);
                    playing_idx = proximo;
                    selection = proximo; 
                }
            }
        }

        if (selection >= offset + max_visible) offset = selection - max_visible + 1;
        if (selection < offset) offset = selection;

        erase(); 

        // [Header]
        attron(COLOR_PAIR(3) | A_BOLD); 
        mvprintw(0, 1, " ⌬ ARCHON_SYSTEM ❯ %s ", current_path);
        attroff(A_BOLD);

        // [Lista de Arquivos]
        for (int i = 0; i < max_visible && (i + offset) < total_files; i++) {
            int idx = i + offset;
            int is_playing = (idx == playing_idx);
            int pair = (idx == selection) ? 5 : (items[idx].is_dir ? 3 : 1);
            attron(COLOR_PAIR(pair));
            if (idx == selection) attron(A_BOLD);
            mvprintw(i + 2, 1, " %s %-60.60s ", (is_playing ? "⌬" : (items[idx].is_dir ? "" : " ")), items[idx].name);
            attroff(A_BOLD | COLOR_PAIR(pair));
        }

        // [Espectro e Progresso]
        if (music) {
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(1, max_x - 36, "[ %02d:%02d ❯❯ PROGRESS ❯❯ %02d:%02d ]", 
                     (int)Mix_GetMusicPosition(music)/60, (int)Mix_GetMusicPosition(music)%60, 
                     (int)Mix_MusicDuration(music)/60, (int)Mix_MusicDuration(music)%60);
            attroff(A_BOLD);

            current_prog = (Mix_MusicDuration(music) > 0) ? Mix_GetMusicPosition(music) / Mix_MusicDuration(music) : 0;
            int horizon_fill = (int)(current_prog * (max_x - 4));
            
            for (int i = 1; i < max_x - 1; i++) {
                int d_idx = (i * 256) / max_x;
                int h = spectrum_data[d_idx % 256] / 18; if (h > 9) h = 9; 
                int is_passed = (i <= horizon_fill);
                attrset(0); 
                for(int k = 1; k <= 10; k++) { mvaddch(horizon_y - k, i, ' '); mvaddch(horizon_y + k, i, ' '); }
                for (int j = 1; j <= h; j++) {
                    int col = is_passed ? ((j < 4) ? 1 : 2) : 1;
                    attron(COLOR_PAIR(col) | (is_passed ? A_BOLD : A_DIM));
                    mvaddch(horizon_y - j, i, ACS_VLINE); mvaddch(horizon_y + j, i, ACS_VLINE);
                    attroff(A_BOLD | A_DIM);
                }
                attron(COLOR_PAIR(is_passed ? 2 : 1) | (is_passed ? A_BOLD : A_DIM));
                mvaddch(horizon_y, i, (i == horizon_fill) ? 'O' : ACS_HLINE);
                attroff(A_BOLD | A_DIM);
            }
        }

        // [Decoding Info]
        if (playing_idx != -1) {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(max_y - 2, 1, " ⌬ DECODING_SIGNAL: ");
            attroff(A_BOLD | COLOR_PAIR(3));
            attron(COLOR_PAIR(1)); printw("%-s", items[playing_idx].name); attroff(COLOR_PAIR(1));
            clrtoeol();
        }

        // [Rodapé]
        int p = 0;
        attron(COLOR_PAIR(2) | A_BOLD); mvprintw(max_y - 1, p, " Q "); p += 3;
        attron(COLOR_PAIR(5)); mvprintw(max_y - 1, p, " EXIT "); p += 8;
        attron(COLOR_PAIR(2) | A_BOLD); mvprintw(max_y - 1, p, " H "); p += 3;
        attron(COLOR_PAIR(5)); mvprintw(max_y - 1, p, " HOME "); p += 8;
        attron(COLOR_PAIR(2) | A_BOLD); mvprintw(max_y - 1, p, " Z "); p += 3;
        attron(COLOR_PAIR(5)); mvprintw(max_y - 1, p, " PORTAL "); p += 10;
        attron(COLOR_PAIR(2) | A_BOLD); mvprintw(max_y - 1, p, " C "); p += 3;
        attron(COLOR_PAIR(5)); mvprintw(max_y - 1, p, " THEME "); p += 9;
        attron(COLOR_PAIR(2) | A_BOLD); mvprintw(max_y - 1, p, " ENTER "); p += 7;
        attron(COLOR_PAIR(5)); mvprintw(max_y - 1, p, " OPEN/PLAY "); p += 13;
        attron(COLOR_PAIR(5)); mvhline(max_y - 1, p, ' ', max_x - p); attroff(COLOR_PAIR(5));

        refresh();
        int ch = getch(); 

        // --- 4. CONTROLES ---
        if (ch == 'q' || ch == 'Q') break;
        if (ch == KEY_UP || ch == 'w' || ch == 'W') { if (selection > 0) selection--; }
        if (ch == KEY_DOWN || ch == 's' || ch == 'S') { if (selection < total_files - 1) selection++; }
        
        if (ch == 'h' || ch == 'H') { 
            chdir(getenv("HOME")); total_files = scan_folder(items); 
            selection = 0; offset = 0; playing_idx = -1; erase(); 
        }
        
        if (ch == 'c' || ch == 'C') { 
            esquema_player = (esquema_player + 1) % 3; init_player_colors(); erase(); 
        }

        if (ch == 'z' || ch == 'Z') {
             if (fork() == 0) {
                 setsid(); int dn = open("/dev/null", O_RDWR); dup2(dn, 0); dup2(dn, 1); dup2(dn, 2); close(dn);
                 char c_cmd[1024]; snprintf(c_cmd, 1024, "%s --app=https://duckduckgo.com", gui_browser);
                 execlp("sh", "sh", "-c", c_cmd, (char *)NULL); _exit(0);
             }
             clearok(stdscr, TRUE); refresh();
        }

        if (ch == 10 || ch == KEY_ENTER) { 
            if (items[selection].is_dir) {
                chdir(items[selection].name); total_files = scan_folder(items); selection = 0; offset = 0; playing_idx = -1;
            } else {
                const char *f = items[selection].name;
                if (check_ext(f, ".jpg") || check_ext(f, ".png") || check_ext(f, ".gif")) {
                    spawn_external("nsxiv", f, 0);
                } else if (check_ext(f, ".txt") || check_ext(f, ".c") || check_ext(f, ".h") || check_ext(f, ".sh") || check_ext(f, ".md")) {
                    spawn_external("nano", f, 1); 
                } else {
                    if (music) { Mix_HaltMusic(); Mix_FreeMusic(music); }
                    music = Mix_LoadMUS(f);
                    if (music) { Mix_PlayMusic(music, 0); Mix_SetPostMix(audio_callback, NULL); playing_idx = selection; }
                }
            }
        }

        if (ch == ' ') { 
            if (music) { if (Mix_PausedMusic()) Mix_ResumeMusic(); else Mix_PauseMusic(); } 
        }
    }

    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio(); SDL_Quit(); endwin();
    return 0;
}


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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// --- ESTRUTURAS E VARIÁVEIS GLOBAIS ---
typedef struct {
    char name[256];
    int is_dir;
} BrowseItem;

int spectrum_data[256];
int peak_data[256]; 
int offset = 0;
int esquema_player = 0; 
double current_prog = 0; 

void audio_callback(void *udata, Uint8 *stream, int len) {
    if (stream == NULL || len == 0) return;
    for (int i = 0; i < 256 && i < len; i++) {
        int val = abs((int8_t)stream[i]);
        spectrum_data[i] = (spectrum_data[i] * 0.7) + (val * 0.3);
    }
}

void init_player_colors() {
    start_color();
    use_default_colors(); 
    if (esquema_player == 0) { // EMERALD ARCHON
        init_pair(1, COLOR_GREEN, -1);   
        init_pair(2, COLOR_YELLOW, -1);  
        init_pair(3, COLOR_CYAN, -1);    
        init_pair(4, COLOR_MAGENTA, -1); 
    } else if (esquema_player == 1) { // BLOOD NODE
        init_pair(1, COLOR_RED, -1);   
        init_pair(2, COLOR_WHITE, -1);  
        init_pair(3, COLOR_YELLOW, -1);    
        init_pair(4, COLOR_RED, -1);    
    } else { // CYBER PUNK
        init_pair(1, COLOR_BLUE, -1);   
        init_pair(2, COLOR_CYAN, -1);  
        init_pair(3, COLOR_MAGENTA, -1);    
        init_pair(4, COLOR_WHITE, -1);    
    }
}

int scan_folder(BrowseItem *items) {
    DIR *dir = opendir(".");
    if (!dir) return 0;
    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL && count < 100) {
        if (entry->d_name[0] == '.' && strcmp(entry->d_name, "..") != 0) continue;
        strncpy(items[count].name, entry->d_name, 255);
        items[count].name[255] = '\0';
        items[count].is_dir = (entry->d_type == DT_DIR);
        count++;
    }
    closedir(dir);
    return count;
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    char *initial_file = (argc > 1) ? argv[1] : NULL;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) return 1;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) return 1;

    initscr(); 
    init_player_colors();
    noecho(); curs_set(0); 
    keypad(stdscr, TRUE); 
    nodelay(stdscr, TRUE); 
    
    BrowseItem items[100];
    int total_files = scan_folder(items);
    int selection = 0;
    int playing_idx = -1; 
    Mix_Music *music = NULL;
    char current_path[1024];

    if (initial_file) {
        music = Mix_LoadMUS(initial_file);
        if (music) {
            Mix_PlayMusic(music, 0);
            Mix_SetPostMix(audio_callback, NULL);
            playing_idx = 0; // Assume o primeiro se vier de fora
        }
    }

    while(1) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        getcwd(current_path, sizeof(current_path));
        
        // --- LOGICA DE AUTO-NEXT ---
        if (playing_idx != -1 && !Mix_PlayingMusic() && !Mix_PausedMusic()) {
            int next_idx = (playing_idx + 1) % total_files;
            int attempts = 0;
            // Pula diretórios para achar a próxima música
            while (items[next_idx].is_dir && attempts < total_files) {
                next_idx = (next_idx + 1) % total_files;
                attempts++;
            }
            if (!items[next_idx].is_dir) {
                if (music) Mix_FreeMusic(music);
                music = Mix_LoadMUS(items[next_idx].name);
                if (music) {
                    Mix_PlayMusic(music, 0);
                    Mix_SetPostMix(audio_callback, NULL);
                    playing_idx = next_idx;
                    selection = next_idx; 
                }
            }
        }

        int max_visible = max_y - 14;   
        int center_y = max_y - 6;      
        int horizon_y = center_y;

        if (selection >= offset + max_visible) offset = selection - max_visible + 1;
        if (selection < offset) offset = selection;

        erase(); 

        attron(COLOR_PAIR(3) | A_BOLD); 
        mvprintw(0, 1, " ⌬ ARCHON_SYSTEM ");
        attroff(A_BOLD);
        attron(COLOR_PAIR(1));
        printw("❯ %s ", current_path);
        attroff(COLOR_PAIR(1));

        for (int i = 0; i < max_visible && (i + offset) < total_files; i++) {
            int idx = i + offset;
            int pair = (idx == selection) ? 2 : (items[idx].is_dir ? 3 : 1);
            if (idx == selection) {
                attron(COLOR_PAIR(pair) | A_BOLD | A_REVERSE);
                mvprintw(i + 2, 1, " %s %-60.60s ", (items[idx].is_dir ? "" : "  "), items[idx].name);
                attroff(COLOR_PAIR(pair) | A_BOLD | A_REVERSE);
            } else {
                attron(COLOR_PAIR(pair));
                mvprintw(i + 2, 2, " %s %-60.60s ", (items[idx].is_dir ? "" : " "), items[idx].name);
                attroff(COLOR_PAIR(pair));
            }
        }

        if (music != NULL) {
            double total = Mix_MusicDuration(music);
            double curr = Mix_GetMusicPosition(music);
            current_prog = (total > 0) ? curr / total : 0;

            int tx = max_x - 32;
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(1, tx, "┏━━━━━━━━ MISSION_TIME ━━━━━━━━┓");
            mvprintw(2, tx, "┃  %02d:%02d ❯❯ PROGRESS ❯❯ %02d:%02d  ┃", (int)curr/60, (int)curr%60, (int)total/60, (int)total%60);
            mvprintw(3, tx, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
            attroff(COLOR_PAIR(2) | A_BOLD);
        }

        if (Mix_PlayingMusic() || Mix_PausedMusic()) {
            int horizon_fill = (int)(current_prog * (max_x - 2));
            for (int i = 0; i < max_x - 2; i++) {
                int d_idx = (i * 256) / max_x;
                int h = spectrum_data[d_idx] / 12; 
                if (h > 10) h = 10;
                
                if (h > peak_data[i]) peak_data[i] = h;
                else if (peak_data[i] > 0) peak_data[i]--;

                for (int j = 1; j <= h; j++) {
                    int col_grad = (j < 3) ? 1 : (j < 7 ? 2 : 4); // Gradiente Vertical
                    attron(COLOR_PAIR(col_grad));
                    mvaddch(horizon_y - j, i + 1, ACS_VLINE);
                    mvaddch(horizon_y + j, i + 1, ACS_VLINE);
                    attroff(COLOR_PAIR(col_grad));
                }

                attron(COLOR_PAIR(i < horizon_fill ? 2 : 3) | A_BOLD);
                mvaddch(horizon_y, i + 1, (i == horizon_fill) ? 'O' : ACS_HLINE);
                attroff(A_BOLD);

                if (peak_data[i] > 0) {
                    attron(COLOR_PAIR(4));
                    mvaddch(horizon_y - peak_data[i] - 1, i + 1, '.');
                    mvaddch(horizon_y + peak_data[i] + 1, i + 1, '.');
                    attroff(COLOR_PAIR(4));
                }
            }
            attron(COLOR_PAIR(3) | A_BOLD);
            if (playing_idx != -1) mvprintw(max_y - 2, 2, " ⌬ DECODING: %s ", items[playing_idx].name);
            attroff(A_BOLD);
        }

        attron(COLOR_PAIR(1) | A_REVERSE);
        mvprintw(max_y - 1, 0, " [Q] EXIT ");
        mvprintw(max_y - 1, 12, " [SPACE] PAUSE ");
        mvprintw(max_y - 1, 28, " [C] COLORS ");
        mvprintw(max_y - 1, 41, " [Z] ZEUS_RADAR ");
        mvhline(max_y - 1, 56, ' ', max_x - 56);
        attroff(A_REVERSE);

        refresh();
        
        int ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_UP && selection > 0) selection--;
        if (ch == KEY_DOWN && selection < total_files - 1) selection++;
        
        if (ch == 10) { 
            if (items[selection].is_dir) {
                chdir(items[selection].name);
                total_files = scan_folder(items);
                selection = 0; offset = 0;
            } else {
                if (music) Mix_FreeMusic(music);
                music = Mix_LoadMUS(items[selection].name);
                Mix_PlayMusic(music, 0); 
                Mix_SetPostMix(audio_callback, NULL);
                playing_idx = selection;
            }
        }

        if (ch == 'z' || ch == 'Z') {
             pid_t pid = fork();
             if (pid == 0) {
                 setsid();
                 int dn = open("/dev/null", O_WRONLY);
                 dup2(dn, 1); dup2(dn, 2);
               char *args[] = {"kitty", "w3m", "https://duckduckgo.com", NULL};
                 execvp("kitty", args);
                 _exit(1);
             }
        }

        if (ch == ' ') { 
            if (Mix_PausedMusic()) Mix_ResumeMusic(); else Mix_PauseMusic(); 
        }
        
        if (ch == 'c') { 
            esquema_player = (esquema_player + 1) % 3; 
            init_player_colors();
            erase(); // Força limpeza para aplicar novo esquema
        }

        usleep(15000); 
    }

    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio(); 
    SDL_Quit(); 
    endwin();
    return 0;
}

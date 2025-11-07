#ifndef __GAME
#define __GAME

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define FPS 30.0

#define WW 1000
#define HH 500

#define C_BLACK al_map_rgb(0, 0, 0)
#define C_WHITE al_map_rgb(255, 255, 255)

typedef struct game {
    // Configuração do Allegro
    ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* display;

    // Níveis
    //level *cur_level;
    //level *levels[];
    //environment *envs[];
} game;

game *game_create(/*level *first, level **levels*/);
int game_destroy(game *g);
int game_process(game *g);

#endif
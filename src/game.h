#ifndef __GAME
#define GAME

#define FPS 30.0

typedef struct game {
    // Configuração do Allegro
    ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* disp;

    // Níveis
    level *cur_level;
    level *levels[];
    //environment *envs[];
} game;

game *game_create(level *first, level **levels);
int *game_destroy(game *g);
int *game_process(game *g);

#endif
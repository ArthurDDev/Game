#ifndef __GAME
#define __GAME

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct env env;
typedef struct spriteProvider spriteProvider;

#define FPS 30.0

#define WW 1000
#define HH 500

#define C_BLACK al_map_rgb(0, 0, 0)
#define C_WHITE al_map_rgb(255, 255, 255)
#define C_RED al_map_rgb(255, 0, 0)

#define MASTER_ENV 0
#define PROCESS_ENV 1
#define ASYNC_PROCESS_ENV 2
#define RENDER_ENV 3

#define KEY_SEEN 1
#define KEY_DOWN 2

typedef struct game game;
struct game {
    // Configuração do Allegro
    ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* display;

    // Níveis
    //level *cur_level;
    //level *levels[];
    unsigned char keys[ALLEGRO_KEY_MAX];
    int n_envs;
    env **envs;
    spriteProvider *sprites;
};

game *game_create(/*level *first, level **levels*/);
int game_destroy(game *g);
int game_process(game *g);

int insert_env(game *g, env *e);

#endif
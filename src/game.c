#include "game.h"

#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "env.h"
#include "spriteProvider.h"

game *game_create()
{
    game *g = malloc(sizeof(game));
    if (!g)
        return NULL;

    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    g->timer = al_create_timer(1.0 / FPS);
    g->queue = al_create_event_queue();
    g->font = al_create_builtin_font();
    g->display = al_create_display(WW, HH);

	al_register_event_source(g->queue, al_get_keyboard_event_source());
	al_register_event_source(g->queue, al_get_display_event_source(g->display));
	al_register_event_source(g->queue, al_get_timer_event_source(g->timer));

    // Ambientes

    g->n_envs = 4;
    g->envs = malloc(sizeof(env *) * g->n_envs);

    g->envs[MASTER_ENV] = processEnv_create(MASTER_ENV);
    g->envs[PROCESS_ENV] = processEnv_create(MASTER_ENV);
    g->envs[ASYNC_PROCESS_ENV] = processEnv_create(MASTER_ENV);
    g->envs[RENDER_ENV] = processEnv_create(MASTER_ENV);

    // Sprites

    g->sprites = spriteProvider_create();

    // Input

    memset(g->keys, 0, sizeof(g->keys));

    return g;
}

int game_destroy(game *g)
{
    g->envs[MASTER_ENV]->compute(g->envs[MASTER_ENV]);
    
    for (int i = 0; i < g->n_envs; i++) {
        g->envs[i]->destroy(g->envs[i]);
    }

    spriteProvider_destroy(g->sprites);
    
    al_destroy_font(g->font);
	al_destroy_display(g->display);
	al_destroy_timer(g->timer);
	al_destroy_event_queue(g->queue);
    
    free(g);

    return 0;
}


int game_process(game *g)
{

    ALLEGRO_EVENT event;
    al_start_timer(g->timer);

    while (1) {
        al_wait_for_event(g->queue, &event);

        g->envs[ASYNC_PROCESS_ENV]->compute(g->envs[ASYNC_PROCESS_ENV]);

        switch(event.type) {

            case ALLEGRO_EVENT_TIMER:

                g->envs[PROCESS_ENV]->compute(g->envs[PROCESS_ENV]);
            
                al_clear_to_color(al_map_rgb(0, 0, 0));
                g->envs[RENDER_ENV]->compute(g->envs[RENDER_ENV]);
                al_flip_display();

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    g->keys[i] &= ~KEY_SEEN;
                
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                g->keys[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                g->keys[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: 
                return 0;
        }
    }

    return 1;
}

int insert_env(game *g, env *e)
{
    g->envs = realloc(g->envs, sizeof(env *) * (g->n_envs + 1));
    g->envs[g->n_envs] = e;
    g->n_envs += 1;

    return g->n_envs - 1;
}
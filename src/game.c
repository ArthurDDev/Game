#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "game.h"
#include "env.h"

game *game_create()
{
    game *g = malloc(sizeof(game));
    if (!g)
        return NULL;

    al_init();
    al_install_keyboard();
    al_init_image_addon();

    g->timer = al_create_timer(1.0 / FPS);
    g->queue = al_create_event_queue();
    g->font = al_create_builtin_font();
    g->display = al_create_display(WW, HH);

	al_register_event_source(g->queue, al_get_keyboard_event_source());
	al_register_event_source(g->queue, al_get_display_event_source(g->display));
	al_register_event_source(g->queue, al_get_timer_event_source(g->timer));

    // Ambientes

    g->n_envs = 3;
    g->envs = malloc(sizeof(char *) * g->n_envs);

    g->envs[PROCESS_ENV] = processEnv_create();
    g->envs[ASYNC_PROCESS_ENV] = processEnv_create();
    g->envs[RENDER_ENV] = processEnv_create();

    // Sprites

    g->sprites = spriteProvider_create();

    return g;
}

int game_destroy(game *g)
{
    al_destroy_font(g->font);
	al_destroy_display(g->display);
	al_destroy_timer(g->timer);
	al_destroy_event_queue(g->queue);

    g->envs[PROCESS_ENV]->destroy(g->envs[PROCESS_ENV]);
    g->envs[ASYNC_PROCESS_ENV]->destroy(g->envs[ASYNC_PROCESS_ENV]);
    g->envs[RENDER_ENV]->destroy(g->envs[RENDER_ENV]);

    spriteProvider_destroy(g->sprites);

    free(g);

    return 0;
}


int game_process(game *g)
{

    ALLEGRO_EVENT event;
    al_start_timer(g->timer);

    while (1) {
        al_wait_for_event(g->queue, &event);

        bool done = false;

        g->envs[ASYNC_PROCESS_ENV]->compute(g->envs[ASYNC_PROCESS_ENV]);

        switch(event.type) {

            case ALLEGRO_EVENT_TIMER:

                g->envs[PROCESS_ENV]->compute(g->envs[PROCESS_ENV]);
            
                al_clear_to_color(al_map_rgb(0, 0, 0));
                g->envs[RENDER_ENV]->compute(g->envs[RENDER_ENV]);
                al_flip_display();
                
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: 
                done = true;
                break;
        }

        if (done)
            break;
    }

    return 0;

}
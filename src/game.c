#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "env.h"

game *game_create()
{
    game *g = malloc(sizeof(game));
    if (!g)
        return NULL;

    al_init();
    al_install_keyboard();

    g->timer = al_create_timer(1.0 / FPS);
    g->queue = al_create_event_queue();
    g->font = al_create_builtin_font();
    g->display = al_create_display(WW, HH);

	al_register_event_source(g->queue, al_get_keyboard_event_source());
	al_register_event_source(g->queue, al_get_display_event_source(g->display));
	al_register_event_source(g->queue, al_get_timer_event_source(g->timer));

    return g;
}

int game_destroy(game *g)
{
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

    while (1) {
        al_wait_for_event(g->queue, &event);

        bool done = false;

        switch(event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE: 
                done = true;
                break;
        }

        if (done)
            break;
    }

    return 0;

}
#include "engine.h"

#include "background.h"

#define BACKGROUND_PARALLAX 0.2

int render_background(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    struct bddata *data = (struct bddata *)e->data;
    e->sprite->current_frame = 0;
    e->sprite->timer = 0;
    for (int i = 0; i < 4; i++) {
        camera_render(G, vec_mult(data->position, i * BACKGROUND_PARALLAX), e->sprite, 3.0, 3.0);
        camera_render(G, vec_mult(data->position, i * BACKGROUND_PARALLAX), e->sprite, 3.0, 3.0);
        e->sprite->current_frame = e->sprite->current_frame + 1;
    }

    return 0;
}

int process_background(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    struct bddata *data = (struct bddata *)e->data;
    //data->position.x = (int)(data->position.x + 1) % WW;

    data->position = vec_invert(G->camera->pos);

    return 0;
}

int init_background(game *G)
{
    entity *backdrop = entity_create(G);
    struct bddata *data = malloc(sizeof(struct bddata));
    backdrop->data = data;
    data->position = vec_create(0.0, 0.0);
    subscribe(G->envs[RENDER_ENV], backdrop, render_background);
    subscribe(G->envs[PROCESS_ENV], backdrop, process_background);

    backdrop->sprite = sprite_create(G, 4, vec_create(WW, HH), (const char *[]){
        "assets/background/back_0.png",
        "assets/background/back_1.png",
        "assets/background/back_2.png",
        "assets/background/back_3.png",
    }, SPR_TOP | SPR_LEFT);
    backdrop->sprite->delay = 10;

    return 0;
}
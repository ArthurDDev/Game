#include "engine.h"

#include "background.h"

#define BACKGROUND_PARALLAX -0.2
#define BACKGROUND_SCALE 4.0

int render_background(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    struct bddata *data = (struct bddata *)e->data;
    e->sprite->current_frame = 0;
    for (int l = 0; l < 4; l++) {
        for (int i = 0; i <= (WW / (e->sprite->size.x * BACKGROUND_SCALE)) + 1; i++) {
            for (int j = 0; j <= (HH / (e->sprite->size.y * BACKGROUND_SCALE)) + 1; j++) {
                e->sprite->current_frame = l;
                    e->sprite->timer = 0;

                render_sprite(e->sprite,
                    data->position.x + (i * e->sprite->size.x * BACKGROUND_SCALE) + (int)(G->camera->pos.x * BACKGROUND_PARALLAX * l) % WW,
                    data->position.y + (j * e->sprite->size.y * BACKGROUND_SCALE) + (int)(G->camera->pos.y * BACKGROUND_PARALLAX * l) % HH,
                    BACKGROUND_SCALE,
                    BACKGROUND_SCALE);
            }
        }
    }

    return 0;
}

int process_background(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    struct bddata *data = (struct bddata *)e->data;
    //data->position.x = (int)(data->position.x + 1) % WW;

    e->pos.x = (G->camera->pos.x);

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

    backdrop->sprite = sprite_create(G, 4, vec_create(272, 160), (const char *[]){
        "assets/background/back_0.png",
        "assets/background/back_1.png",
        "assets/background/back_2.png",
        "assets/background/back_3.png",
    }, SPR_TOP | SPR_LEFT);
    backdrop->sprite->delay = 10;
    backdrop->pos = vec_create(0.0, 0.0);

    return 0;
}
#include "platform.h"

#include <stdio.h>

#include "engine.h"
#include "level_1.h"

struct platformdata {
    int state;
    int timer;
};

int process_platform(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    
    struct platformdata *pdata = (struct platformdata *)e->data;
    if (pdata->timer == 0) {
        pdata->state = pdata->state ^ 0b1;
        pdata->timer = 45;
        if (pdata->state == 0)
            g->envs[COLLISION_ENV]->unsubscribe(e, g->envs[COLLISION_ENV]);
        else
            g->envs[COLLISION_ENV]->subscribe(e, g->envs[COLLISION_ENV], NULL);
    }
    pdata->timer --;

    return 0;
}

int render_platform(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct platformdata *pdata = (struct platformdata *)e->data;

    e->sprite->current_frame = pdata->state ^ 1;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int init_platform(game *g, vec pos, int start)
{
    if (!g)
        return 1;

    entity *platform = entity_create(g);
    struct platformdata *pdata = malloc(sizeof(struct platformdata));
    platform->data = pdata;

    platform->pos = pos;

    pdata->state = start;
    pdata->timer = 0;

    platform->sprite = sprite_create(g, 2, vec_create(16.0, 16.0), (const char *[]){
        "assets/platform/platform_1.png",
        "assets/platform/platform_2.png",
    }, SPR_TOP | SPR_LEFT);
    platform->sprite->delay = -1;

    hitbox_attatch(platform, 48, 48, HB_TOP | HB_LEFT);

    subscribe(g->envs[PROCESS_ENV], platform, process_platform);
    subscribe(g->envs[RENDER_ENV], platform, render_platform);

    return 0;
}

int destroy_platform(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    return 0;
}

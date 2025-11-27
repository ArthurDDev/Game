#include "trapdoor.h"

#include "engine.h"
#include "level_1.h"

#include <stdio.h>

struct trapdoordata {
    int timer;
    int active;
};

int trapdoor_render(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct trapdoordata *tdata = (struct trapdoordata *)e->data;

    if (tdata->timer == 0)
        e->sprite->current_frame = 0;
    else if (tdata->timer < 0)
        e->sprite->current_frame = 3;
    else if (tdata->timer < 15)
        e->sprite->current_frame = 1;
    else
        e->sprite->current_frame = 2;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int trapdoor_process(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct trapdoordata *tdata = (struct trapdoordata *)e->data;
    int deactivate = -1;

    if (tdata->timer == 0) {
        e->pos.y --;
        if (collides(e, g->player)) {
            tdata->timer = 30;
        }
        e->pos.y ++;
    }
    else {
        if (tdata->timer > 0) {
            if (tdata->timer == 1) deactivate = 1;
            tdata->timer --;
        }
        else if (tdata->timer < 0) {
            if (tdata->timer == -1) deactivate = 0;
            tdata->timer ++;
        }
    }

    if (deactivate != -1 && deactivate != tdata->active) {
        if (deactivate == 1) {
            tdata->timer = -30;
            g->envs[COLLISION_ENV]->unsubscribe(e, g->envs[COLLISION_ENV]);
        }
        else {
            g->envs[COLLISION_ENV]->subscribe(e, g->envs[COLLISION_ENV], NULL);
        }
        tdata->active = deactivate;
    }

    return 0;
}

int init_trapdoor(game *g, vec pos)
{
    if (!g)
        return 1;

    entity *trapdoor = entity_create(g);
    trapdoor->pos = pos;
    
    struct trapdoordata *tdata = malloc(sizeof(struct trapdoordata));
    trapdoor->data = tdata;

    trapdoor->sprite = sprite_create(g, 4, vec_create(16.0, 16.0), (const char *[]) {
        "assets/trapdoor/trapdoor1.png",
        "assets/trapdoor/trapdoor2.png",
        "assets/trapdoor/trapdoor3.png",
        "assets/trapdoor/trapdoor4.png",
    }, SPR_TOP | SPR_LEFT);
    trapdoor->sprite->delay = -1;

    tdata->timer = 0;
    tdata->active = 0;

    hitbox_attatch(trapdoor, 48.0, 48.0, HB_TOP | HB_LEFT);

    subscribe(g->envs[RENDER_ENV], trapdoor, trapdoor_render);
    subscribe(g->envs[PROCESS_ENV], trapdoor, trapdoor_process);

    subscribe(g->envs[COLLISION_ENV], trapdoor, NULL);

    return 0;
}

int destroy_trapdoor(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    return 0;
}

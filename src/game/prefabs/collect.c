#include "collect.h"

#include "engine.h"
#include "controller.h"

int process_collect(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct controllerData *cdata = (struct controllerData *)g->controller->data;

    if (collides(e, g->player)) {
        entity_destroy(e, g);
        if (cdata->life < cdata->maxLife)
            cdata->life ++;
    }

    return 0;
}

int render_collect(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int init_collect(game *g, vec pos)
{
    if (!g)
        return 1;

    entity *collect = entity_create(g);
    collect->sprite = sprite_create(g, 1, vec_create(16.0, 16.0), (const char *[]){
        "assets/heart/heart_0.png",
    }, SPR_TOP | SPR_LEFT);

    collect->pos = pos;

    hitbox_attatch(collect, 48.0, 48.0, HB_TOP | HB_LEFT);

    subscribe(g->envs[RENDER_ENV], collect, render_collect);
    subscribe(g->envs[PROCESS_ENV], collect, process_collect);

    return 0;
}

int destroy_collect(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    return 0;
}
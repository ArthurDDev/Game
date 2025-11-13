#include "engine.h"

struct shotData {
    vec vel;
    entity *player;
};

int shot_process(entity *e, game *g)
{
    if (!e)
        return -1;

    struct shotData *sdata = (struct shotData *)e->data;

    //e->pos = vec_add(e->pos, sdata->vel);
    e->pos = vec_add(e->pos, vec_create(15.0, 0.0));

    if (collides_env(e, g->envs[5]))
        entity_destroy(e, g);

    return 0;
}

int shot_render(entity *e, game *g)
{
    if (!e)
        return -1;

    vec newPos = position_to_camera(g, e->pos);
	newPos = vec_sub(newPos, e->hitbox->offset);
	al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, al_map_rgba(255, 0, 0, 0.01));

    return 0;
}

int init_shot(game *G, vec position, vec vel, entity *player)
{
    entity *shot = entity_create(G);
    struct shotData *sdata = malloc(sizeof(struct shotData));
    shot->data = sdata;

    shot->pos = position;
    sdata->player = player;
    sdata->vel = vel;

    hitbox_attatch(shot, 16, 16, 0);

    subscribe(G->envs[RENDER_ENV], shot, shot_render);
    subscribe(G->envs[PROCESS_ENV], shot, shot_process);

    return 1;
}

int destroy_shot(game *G, entity *shot)
{
    if (!G || !shot)
        return -1;

    entity_destroy(shot, G);
    return 0;
}
#include "patrol.h"

#include "engine.h"
#include "level_1.h"
#include "player.h"

#define PATROL_SPEED 5.0

struct patroldata {
    int dir;
};

int process_patrol(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct patroldata *pdata = (struct patroldata *)e->data;

    e->pos.x += PATROL_SPEED * pdata->dir;
    if (collides_env(e, g->envs[COLLISION_ENV])) {
        e->pos.x -= pdata->dir * PATROL_SPEED;
        pdata->dir *= -1;
    }

    if (collides(e, g->player))
        damage_player(g, g->player, 0);

    return 0;
}

int render_patrol(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int init_patrol(game *g, vec pos)
{
    if (!g)
        return 1;

    entity *patrol = entity_create(g);
    struct patroldata *pdata = malloc(sizeof(struct patroldata));

    patrol->data = pdata;
    pdata->dir = 1;
    patrol->pos = pos;

    patrol->sprite = sprite_create(g, 2, vec_create(16.0, 16.0), (const char *[]) {
        "assets/patrol/patrol_1.png",
        "assets/patrol/patrol_2.png",
    }, SPR_TOP | SPR_LEFT);

    hitbox_attatch(patrol, 48.0, 48.0, HB_TOP | HB_LEFT);

    subscribe(g->envs[PROCESS_ENV], patrol, process_patrol);
    subscribe(g->envs[RENDER_ENV], patrol, render_patrol);

    return 0;
}

int destroy_patrol(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    return 0;
}
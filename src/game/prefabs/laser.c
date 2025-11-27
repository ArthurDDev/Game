#include "laser.h"

#include "engine.h"
#include "level_1.h"
#include "player.h"

struct laserdata {
    int timer;
    sprite *preparesprite;
    sprite *lasersprite;
};

int render_laser(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct laserdata *ldata = (struct laserdata *)e->data;

    if (ldata->timer > 0 && ldata->timer < 30)
        camera_render(g, e->pos, ldata->preparesprite, 3.0, 3.0);
    else if (ldata->timer < 0)
        camera_render(g, e->pos, ldata->lasersprite, 3.0, 3.0);

    return 0;
}

int process_laser(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct laserdata *ldata = (struct laserdata *)e->data;

    if (ldata->timer > 0) {
        ldata->timer --;
        if (ldata->timer == 0) {
            ldata->timer = -60;
        }
    }
    else {
        ldata->timer ++;
        if (ldata->timer == 0) {
            ldata->timer = 120;
        }
        else if (collides(e, g->player)) {
            damage_player(g, g->player, 1);
        }
    }

    return 0;
}

int init_laser(game *g, vec pos)
{
    if (!g)
        return 1;

    entity *laser = entity_create(g);
    struct laserdata *ldata = malloc(sizeof(struct laserdata));
    laser->data = ldata;

    laser->pos = pos;

    ldata->timer = 0;

    ldata->lasersprite = sprite_create(g, 7, vec_create(16.0, 16.0), (const char *[]) {
        "assets/laser/laser_1.png",
        "assets/laser/laser_2.png",
        "assets/laser/laser_3.png",
        "assets/laser/laser_4.png",
        "assets/laser/laser_5.png",
        "assets/laser/laser_6.png",
        "assets/laser/laser_7.png",
    }, 0 | SPR_LEFT);
    ldata->lasersprite->delay = 1;

    ldata->preparesprite = sprite_create(g, 1, vec_create(16.0, 16.0), (const char *[]) {
        "assets/laser/laser_prepare.png",
    }, 0 | SPR_LEFT);

    laser->sprite = ldata->lasersprite;

    hitbox_attatch(laser, 48.0, 16.0, HB_LEFT);

    subscribe(g->envs[PROCESS_ENV], laser, process_laser);
    subscribe(g->envs[RENDER_ENV], laser, render_laser);

    laser->destroy = destroy_laser;

    return 0;
}

int destroy_laser(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    //struct laserdata *ldata = (struct laserdata *)e->data;

    // deletar os sprites

    e->sprite = NULL;

    return 0;
}
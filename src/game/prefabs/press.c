#include "press.h"

#include "engine.h"
#include "level_1.h"
#include "player.h"

#define HEAD_ACC 3;

struct pressdata {
    int active;
    int timer;
    entity *head;
};

struct headdata {
    int vel;
};

int process_head(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct headdata *data = (struct headdata *)e->data;

    if (collides(e, g->player)) {
        damage_player(g, g->player, 1);
        return 0;
    }

    if (collides_env(e, g->envs[COLLISION_ENV]))
        data->vel = 0;
    else
        data->vel += HEAD_ACC;

    e->pos.y += data->vel;

    return 0;
}

int render_head(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int process_press(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct pressdata *data = (struct pressdata *)e->data;

    if (!data->active) {
        if (data->timer < 60 * 1) {
            data->timer ++;
        }
        else if (collides(e, g->player)) {
            data->head = entity_create(g);
            data->head->pos = e->pos;
            struct headdata *hdata = malloc(sizeof(struct headdata));
            data->head->data = hdata;
            hdata->vel = 0;
            hitbox_attatch(data->head, 48 * 10, 48 * 1, HB_LEFT);
            data->active = 1;
            data->timer = 0;
            data->head->sprite = sprite_create(g, 1, vec_create(160, 144), (const char *[]){
                "assets/press/press_head.png"
            }, SPR_BOTTOM | SPR_LEFT);

            subscribe(g->envs[PROCESS_ENV], data->head, process_head);
            subscribe(g->envs[RENDER_ENV], data->head, render_head);
        }
    }
    else {
        if (data->timer < 60 * 1) {
            data->timer ++;
        }
        else {
            data->head = entity_destroy(data->head, g);
            data->active = 0;
            data->timer = 0;
        }
    }

    return 0;
}

int render_press(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    camera_render(g, e->pos, e->sprite, 3.0, 3.0);

    return 0;
}

int init_press(game *g, vec pos)
{
    if (!g)
        return 1;

    entity *press = entity_create(g);
    press->pos = pos;

    struct pressdata *prdata = malloc(sizeof(struct pressdata));
    press->data = prdata;

    prdata->active = 0;
    prdata->timer = 0;
    prdata->head = NULL;

    press->sprite = sprite_create(g, 1, vec_create(160, 144), (const char *[]){
        "assets/press/press_base.png"
    }, SPR_TOP | SPR_LEFT);

    hitbox_attatch(press, 48 * 10, 48 * 9, HB_LEFT | HB_TOP);

    subscribe(g->envs[RENDER_ENV], press, render_press);
    subscribe(g->envs[PROCESS_ENV], press, process_press);


    return 0;
}

int destroy_press(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    return 0;
}
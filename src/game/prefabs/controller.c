#include "controller.h"
#include "engine.h"

#include <allegro5/allegro_primitives.h>

int controller_render(entity *e, game *g)
{
    if (!e || !g)
        return 1;

    struct controllerData *cdata = (struct controllerData *)e->data;

    if (!cdata)
        return -1;
    
    for (int i = 0; i < cdata->maxLife; i++) {
        if (cdata->life > i)
            e->sprite->current_frame = 0;
        else
            e->sprite->current_frame = 1;

        render_sprite(e->sprite, 10.0 + i * 48.0, 10.0, 3.0, 3.0);
    }

    return 0;
}

int controller_process(entity *e, game *g)
{
    struct controllerData *cdata = (struct controllerData *)e->data;

    return 0;
}

int init_controller(game *g)
{
    if (!g || g->controller != NULL)
        return -1;

    entity *controller = entity_create(g);
    
    struct controllerData *cdata = malloc(sizeof(struct controllerData));
    controller->data = cdata;
    
    cdata->life = 2;
    cdata->maxLife = 4;
    
    subscribe(g->envs[UI_RENDER_ENV], controller, controller_render);
    subscribe(g->envs[PROCESS_ENV], controller, controller_process);

    g->envs[LEVEL_ENV]->unsubscribe(controller, g->envs[LEVEL_ENV]);

    controller->sprite = sprite_create(g, 2, vec_create(16.0, 16.0), (const char *[]){
        "assets/heart/heart_0.png",
        "assets/heart/heart_1.png"}
    , SPR_TOP | SPR_LEFT);
    controller->sprite->delay = -1;

    g->controller = controller;

    return 0;
}

int destroy_controller(game *g, entity *e)
{
    if (!g || !e)
        return -1;

    return 0;
}

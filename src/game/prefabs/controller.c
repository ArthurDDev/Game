#include "controller.h"
#include "engine.h"

#include <allegro5/allegro_primitives.h>

struct controllerData {
    int life;
};

int controller_render(entity *e, game *g)
{
    struct controllerData *cdata = (struct controllerData *)e->data;

    if (!cdata)
        return -1;
    
    al_draw_filled_rectangle(10, 10, 10 + (cdata->life * 1), 30, al_map_rgb(255, 0, 0));

    return 0;
}

int controller_process(entity *e, game *g)
{
    struct controllerData *cdata = (struct controllerData *)e->data;

    if (g->keys[ALLEGRO_KEY_UP]) {
        cdata->life += 1;
        if (cdata->life > 100)
            cdata->life = 100;
    }
    if (g->keys[ALLEGRO_KEY_DOWN]) {
        cdata->life -= 1;
        if (cdata->life < 0)
            cdata->life = 0;
    }

    return 0;
}

int init_controller(game *g)
{
    if (!g)
        return -1;

    entity *controller = entity_create(g);
    
    struct controllerData *cdata = malloc(sizeof(struct controllerData));
    controller->data = cdata;
    
    cdata->life = 100;
    
    subscribe(g->envs[RENDER_ENV], controller, controller_render);
    subscribe(g->envs[PROCESS_ENV], controller, controller_process);

    g->envs[LEVEL_ENV]->unsubscribe(controller, g->envs[LEVEL_ENV]);

    return 0;
}

int destroy_controller(game *g, entity *e)
{
    if (!g || !e)
        return -1;

    return 0;
}

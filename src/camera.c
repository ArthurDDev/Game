#include "camera.h"

#include <allegro5/allegro.h>

#include "entity.h"
#include "vec.h"
#include "game.h"
#include "env.h"

int camera_create(game *G)
{
    entity *camera = entity_create(G);
    camera->pos = vec_create(0.0, 0.0);

    G->camera = camera;
    // Depois atualizar para permitir escala

    return camera->id;
}

vec position_to_camera(game *G, vec pos)
{
    entity *camera = G->camera;
    return vec_sub(pos, camera->pos);
}

void camera_render(game *G, vec pos, ALLEGRO_BITMAP *bmp)
{
    vec newPos = position_to_camera(G, pos);
    al_draw_bitmap(bmp, newPos.x, newPos.y, 0);
}
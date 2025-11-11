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

    // Depois atualizar para permitir escala

    return camera->id;
}

vec position_to_camera(game *G, int cameraID, vec pos)
{
    entity *camera = G->envs[MASTER_ENV]->get(cameraID, G->envs[MASTER_ENV]);
    return vec_sub(pos, camera->pos);
}

void camera_render(game *G, int cameraID, vec pos, ALLEGRO_BITMAP *bmp)
{
    vec newPos = position_to_camera(G, cameraID, pos);
    al_draw_bitmap(bmp, newPos.x, newPos.y, 0);
}
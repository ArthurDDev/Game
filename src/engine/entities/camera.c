#include "camera.h"

#include <allegro5/allegro.h>

#include "entity.h"
#include "vec.h"
#include "game.h"
#include "env.h"
#include "sprite.h"

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

void camera_render(game *G, vec pos, sprite *spr, double scale_x, double scale_y)
{
    if (pos.x > G->camera->pos.x + WW || pos.x + spr->size.x * scale_x < G->camera->pos.x ||
        pos.y > G->camera->pos.y + HH || pos.y + spr->size.y * scale_y < G->camera->pos.y) {
        return;
    }

    vec newPos = position_to_camera(G, pos);
    render_sprite(spr, newPos.x, newPos.y, scale_x, scale_y);

}
#include "camera.h"

#include <allegro5/allegro.h>

#include "entity.h"
#include "vec.h"
#include "game.h"
#include "env.h"
#include "sprite.h"
#include "collision.h"

int camera_destroy(entity *e, game *g)
{
    if (!g || !e)
        return 1;

    g->camera = NULL;

    return 0;
}

int camera_create(game *G)
{
    entity *camera = entity_create(G);
    camera->pos = vec_create(0.0, 0.0);

    G->camera = camera;
    camera->destroy = camera_destroy;

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

void camera_render_hitbox(game *G, entity *e)
{
    if (!G || !e || !e->hitbox)
        return;

    vec pos = vec_sub(e->pos, e->hitbox->offset);
    vec camPos = G->camera->pos;

    if (pos.x > camPos.x + WW || pos.x + e->hitbox->size.x < camPos.x ||
        pos.y > camPos.y + HH || pos.y + e->hitbox->size.y < camPos.y) {
        return;
    }

    al_draw_filled_rectangle(pos.x - camPos.x, pos.y - camPos.y, pos.x + e->hitbox->size.x - camPos.x, pos.y + e->hitbox->size.y - camPos.y, al_map_rgba(255, 0, 0, 0.3));
}
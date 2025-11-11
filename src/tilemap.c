#include "tilemap.h"
#include "game.h"

#include "entity.h"
#include "collision.h"

void tilemap_load(game *G, const char **data, int tilesize, int n_rows, int n_cols, int collision_env, int render_env, int (*renderFunc)(entity *e))
{
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            switch(data[i][j]) {
                case '1': {
                    entity *soil = entity_create(G);
                    soil->pos = vec_create(j * tilesize, i * tilesize);
                    hitbox_attatch(soil, tilesize, tilesize, 0);
                    subscribe(G->envs[render_env], soil, renderFunc);
                    subscribe(G->envs[collision_env], soil, NULL);
                } break;
            }
        }
    }
}
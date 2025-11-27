#include "tilemap.h"
#include "game.h"

#include "entity.h"
#include "collision.h"
#include "sprite.h"
#include "world.h"

void tilemap_load(game *G, const char **data, int tilesize, int n_rows, int n_cols, int collision_env, int render_env, int (*renderFunc)(entity *e, game *g))
{
    const char *tile_data[] = {
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_01.png",
        "assets/sheet/tile_10.png",
        "assets/sheet/tile_08.png",
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_00.png",
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_00.png",
        "assets/sheet/tile_12.png",
        "assets/sheet/tile_09.png",
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_00.png",
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_00.png",
        "assets/sheet/tile_11.png",
        "assets/sheet/tile_00.png",
    };

    int dir = 0;

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            switch(data[i][j]) {
                case '1': {

                    dir = 0;

                    if (i == 0 || (data[i-1][j] != '1' && data[i-1][j] != '1'))
                        dir += 1; // Top
                    if (j == 0 || (data[i][j-1] != '1' && data[i][j-1] != '1'))
                        dir += 2; // Left
                    if (i == n_rows - 1 || (data[i+1][j] != '1' && data[i+1][j] != '1'))
                        dir += 4; // Bottom
                    if (j == n_cols - 1 || (data[i][j+1] != '1' && data[i][j+1] != '1'))
                        dir += 8; // Right

                    entity *soil = entity_create(G);
                    soil->pos = vec_create(j * tilesize, i * tilesize);
                    soil->sprite = sprite_create(G, 1, vec_create((double)tilesize, (double)tilesize), (const char *[]){
                        tile_data[dir],
                    }, SPR_TOP | SPR_LEFT);
                    hitbox_attatch(soil, tilesize, tilesize, HB_TOP | HB_LEFT);
                    subscribe(G->envs[render_env], soil, renderFunc);
                    if (dir != 0)
                        subscribe(G->envs[collision_env], soil, NULL);
                } break;
                case '2': {
                    
                    dir = 0;

                    if (i == 0 || data[i-1][j] == '0')
                        dir += 1; // Top
                    if (j == 0 || data[i][j-1] == '0')
                        dir += 2; // Left
                    if (i == n_rows - 1 || data[i+1][j] == '0')
                        dir += 4; // Bottom
                    if (j == n_cols - 1 || data[i][j+1] == '0')
                        dir += 8; // Right


                    entity *soil = entity_create(G);
                    soil->pos = vec_create(j * tilesize, i * tilesize);
                    soil->sprite = sprite_create(G, 1, vec_create((double)tilesize, (double)tilesize), (const char *[]){
                        "assets/sheet/grapple_00.png",
                    }, SPR_TOP | SPR_LEFT);
                    hitbox_attatch(soil, tilesize, tilesize, HB_TOP | HB_LEFT);
                    subscribe(G->envs[render_env], soil, renderFunc);
                    subscribe(G->envs[collision_env], soil, NULL);
                    subscribe(G->envs[collision_env + 1], soil, NULL);

                    struct groundData *gdata = malloc(sizeof(struct groundData));
                    soil->data = gdata;

                    if (dir == 1)
                        gdata->direction = vec_create(0.0, 1.0);
                    else if (dir == 2)
                        gdata->direction = vec_create(1.0, 0.0);
                    else if (dir == 4)
                        gdata->direction = vec_create(0.0, -1.0);
                    else
                        gdata->direction = vec_create(-1.0, 0.0);

                } break;
                case '3':
                case '4':
                case '5':
                case '6':
                    spike_init(G, data[i][j] - '3', vec_create(j * tilesize, i * tilesize));
                    break;
            }
        }
    }
}
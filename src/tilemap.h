#ifndef __TILEMAP
#define __TILEMAP

typedef struct game game;
typedef struct entity entity;

void tilemap_load(game *G, const char **data, int tilesize, int n_rows, int n_cols, int collision_env, int render_env, int (*renderFunc)(entity *e));

#endif
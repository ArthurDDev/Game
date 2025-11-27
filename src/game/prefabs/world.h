#ifndef __WORLD
#define __WORLD

#include "vec.h"

typedef struct game game;
typedef struct entity entity;

struct groundData {
    vec direction;
};

int soil_render(entity *e, game *G);

int spike_init(game *g, int dir, vec pos);


#endif
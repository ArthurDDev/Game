#ifndef __BACKGROUND
#define __BACKGROUND

#include "vec.h"
typedef struct entity entity;
typedef struct game game;

struct bddata {
    vec position;
};

int init_background(game *G);

#endif
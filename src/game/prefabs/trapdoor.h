#ifndef __TRAPDOOR
#define __TRAPDOOR

typedef struct game game;
typedef struct entity entity;
typedef struct vec vec;

int init_trapdoor(game *g, vec pos);
int destroy_trapdoor(entity *e, game *g);

#endif
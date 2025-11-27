#ifndef __SHOT
#define __SHOT

typedef struct game game;
typedef struct vec vec;
typedef struct entity entity;

int init_shot(game *G, vec position, vec vel, entity *player);
int destroy_shot(game *G, entity *shot);

#endif
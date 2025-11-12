#ifndef __PLAYER
#define __PLAYER

typedef struct game game;
typedef struct entity entity;
typedef struct vec vec;

int init_player(game *g, vec pos);
int destroy_player(game *g, entity *e);

#endif
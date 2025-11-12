#ifndef __CONTROLER
#define __CONTROLER

typedef struct game game;
typedef struct entity entity;

int init_controller(game *g);
int destroy_controller(game *g, entity *e);

#endif
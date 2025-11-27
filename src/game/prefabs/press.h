#ifndef __PRESS
#define __PRESS

typedef struct game game;
typedef struct vec vec;
typedef struct entity entity;
typedef struct game game;

int init_press(game *g, vec pos);
int destroy_press(entity *e, game *g);

#endif
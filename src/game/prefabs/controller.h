#ifndef __CONTROLER
#define __CONTROLER

typedef struct game game;
typedef struct entity entity;

struct controllerData {
    int life;
    int maxLife;
};

int init_controller(game *g);
int destroy_controller(entity *e, game *g);

#endif
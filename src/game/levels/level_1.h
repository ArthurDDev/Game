#ifndef __L1
#define __L1

#define COLLISION_ENV 6
#define TELEPORT_COLLISION_ENV 7

typedef struct game game;

int load_l1(game *G);
int unload_l1(game *G);

#endif
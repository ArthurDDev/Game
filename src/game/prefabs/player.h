#ifndef __PLAYER
#define __PLAYER

#include "engine.h"

typedef struct game game;
typedef struct entity entity;
typedef struct vec vec;

struct playerData {
	vec velocity;
    vec lastDir;
	int direction;

	int state;
	vec gravityDir;

	sprite *walkingSprite;
	sprite *idleSprite;
};

int init_player(game *g, vec pos);
int destroy_player(game *g, entity *e);

#endif
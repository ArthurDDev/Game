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
	sprite *shortSprite;
	sprite *shortSpriteWalk;

	int grace;
	int can_shoot;
};

int init_player(game *g, vec pos);
int destroy_player(entity *e, game *g);

int damage_player(game *g, entity *e, int damagetype);

#endif
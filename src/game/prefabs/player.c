#include "player.h"

#include <stdio.h>
#include <math.h>

#include "engine.h"
#include "shot.h"

#define SHOTSPEED 20.0
#define COLLISION_ENV 5
#define GRAVITY_STRENGTH 2.0
#define GRAVITY_STRENGTH_FALLING 2.2
#define WALK_ACC 2.0
#define WALK_DEACC 3.0
#define MAX_SPEED 20.0

enum pstate {
	IDLE,
	JUMPING,
	FALLING,
	WALKING,
};

struct playerData {
	vec velocity;
	int direction;

	int state;
	vec gravityDir;

	sprite *walkingSprite;
	sprite *idleSprite;
};

int isOnGround(entity *e, game *g)
{
	struct playerData *pdata = e->data;

	int collides;

	vec_add(e->pos, pdata->gravityDir);

	collides = collides_env(e, g->envs[COLLISION_ENV]);

	vec_sub(e->pos, pdata->gravityDir);

	return collides;
}

int process_player(entity *e, game *g)
{
	//
    struct playerData *pdata = e->data;

	// Gravidade
	vec vel = pdata->velocity;
	
	if (!isOnGround(e, g)) {
		vel = vec_add(vel, vec_mult(vec_normalize(pdata->gravityDir), GRAVITY_STRENGTH));
		printf("Não está no chão\n");
	}
	else
		printf("Está no chão\n");

	// Input
	if (fabs(pdata->gravityDir.x) < fabs(pdata->gravityDir.y)) {
		// Movimentando na horizontal
		if ((g->keys[ALLEGRO_KEY_D] ^ g->keys[ALLEGRO_KEY_A])) {
			if (g->keys[ALLEGRO_KEY_D]) {
				if (vel.x < MAX_SPEED) { 
					vel.x = (vel.x + WALK_ACC);
					if (vel.x > MAX_SPEED)
					vel.x = MAX_SPEED;
				}
			}
			if (g->keys[ALLEGRO_KEY_A]) {
				if (vel.x > -MAX_SPEED) { 
					vel.x = (vel.x - WALK_ACC);
					if (vel.x < MAX_SPEED)
					vel.x = -MAX_SPEED;
				}
			}
		}
		else {
			if (fabs(vel.x) <= WALK_DEACC)
				vel.x = 0;
			else if (vel.x > 0)
				vel.x -= WALK_DEACC;
			else
				vel.x += WALK_DEACC;
		}

	}
	else {
		// Movimentando na vertical
	}
	printf("vel: %f %f\n", vel.x, vel.y);
	printf("pos: %f %f\n", e->pos.x, e->pos.y);
	e->pos = vec_add(e->pos, vel);
	// Colisões
	entity *col = collides_env(e, g->envs[COLLISION_ENV]);
	if (col) {
		printf("Está colidindo\n");
		while (col) {
			while (collides(e, col)) {
				if (pdata->velocity.x != 0 || pdata->velocity.y != 0)
					e->pos = vec_sub(e->pos, vec_normalize(pdata->velocity));
				else
					e->pos = vec_sub(e->pos, pdata->gravityDir);
			}

			col = collides_env(e, g->envs[COLLISION_ENV]);
		}
		if (vel.x > 0)
			e->pos.x = ceil(e->pos.x);
		else
			e->pos.x = floor(e->pos.x);

		
		if (vel.y > 0)
			e->pos.y = ceil(e->pos.y);
		else
			e->pos.y = floor(e->pos.y);
		
		if (pdata->gravityDir.x != 0)
			vel.x = 0;
		else
			vel.y = 0;	
		//vel = vec_create(vel.x * pdata->gravityDir.y, vel.y = pdata->gravityDir.x);
	}

	pdata->velocity = vel;

	return 0;
}

int render_player(entity *e, game *g)
{
    struct playerData *pdata = e->data;

	vec newPos = position_to_camera(g, e->pos);
	newPos = vec_sub(newPos, e->hitbox->offset);

	if (pdata->velocity.x != 0.0)
		e->sprite = pdata->walkingSprite;
	else
		e->sprite = pdata->idleSprite;

	if (pdata->direction == 1)
		camera_render(g, e->pos, e->sprite, 3.0, 3.0);
	else
		camera_render(g, e->pos, e->sprite, -3.0, 3.0);
	//al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, al_map_rgba(255, 0, 0, 0.01));

	return 0;
}

int init_player(game *g, vec pos)
{
    entity *player = entity_create(g);

    struct playerData *pdata = malloc(sizeof(struct playerData));
    player->data = pdata;

	// Animação

    player->sprite = sprite_create(g, 4, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/walking_0.png",
		"assets/characters/walking_1.png",
		"assets/characters/walking_2.png",
		"assets/characters/walking_3.png"
	}, SPR_BOTTOM);
	hitbox_attatch(player, 32.0, 48.0, HB_BOTTOM);
    
    pdata->walkingSprite = player->sprite;
	pdata->idleSprite = sprite_create(g, 2, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/idle_0.png",
		"assets/characters/idle_1.png",
	}, SPR_BOTTOM);
    
	pdata->state = IDLE;
    
	// Movimentação
	player->pos = pos;
	pdata->velocity = vec_create(0.0, 0.0);
	pdata->gravityDir = vec_create(0.0, 1.0);

	subscribe(g->envs[PROCESS_ENV], player, process_player);
    subscribe(g->envs[RENDER_ENV], player, render_player);

    return player->id;
}

int destroy_player(game *g, entity *e)
{
    if(!g || !e)
        return 1;

    return 0;
}

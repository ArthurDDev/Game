#include "player.h"

#include <stdio.h>
#include <math.h>

#include "engine.h"
#include "shot.h"
#include "level_1.h"
#include "controller.h"

#define SHOTSPEED 40.0
#define GRAVITY_STRENGTH 2.0
#define GRAVITY_STRENGTH_FALLING 2.2
#define WALK_ACC 2.0
#define WALK_DEACC 3.0
#define FLOATING_DEACC 1.0
#define MAX_SPEED 20.0
#define JUMP_STRENGTH 30.0
#define DASH_STRENGTH 50.0

enum pstate {
	IDLE,
	JUMPING,
	FALLING,
	WALKING,
	SHORT,
	SHORT_WALKING
};

int damage_player(game *g, entity *e, int damage)
{
	struct controllerData *cdata = (struct controllerData *)(g->controller->data);
	cdata->life -= damage;

	level_change(g, damage);

	return 0;
}

char isOnSurface(entity *e, game *g, vec surface)
{
	e->pos = vec_add(e->pos, vec_mult(surface, 1.0));

	entity *collides = collides_env(e, g->envs[COLLISION_ENV]);

	e->pos = vec_sub(e->pos, vec_mult(surface, 1.0));

	if (collides != NULL)
		return 1;
	else
		return 0;
}

int process_player(entity *e, game *g)
{
	//
    struct playerData *pdata = e->data;
	
	// Gravidade
	vec vel = pdata->velocity;
	char onGround = isOnSurface(e, g, pdata->gravityDir);

	if (!onGround) {
		vel = vec_add(vel, vec_mult(vec_normalize(pdata->gravityDir), GRAVITY_STRENGTH));
	}

	// Input
	// Movimentação
	if (onGround) {
		if (g->keys[ALLEGRO_KEY_SPACE]) {
			g->keys[ALLEGRO_KEY_SPACE] = 0;
			vel = vec_sub(vel, vec_mult(vec_normalize(pdata->gravityDir), JUMP_STRENGTH));
		}
		pdata->state = IDLE;
		if ((pdata->gravityDir.x == 0.0 && pdata->gravityDir.y == 1.0 && g->keys[ALLEGRO_KEY_S]) ||
			(pdata->gravityDir.x == 0.0 && pdata->gravityDir.y == -1.0 && g->keys[ALLEGRO_KEY_W]) ||
			(pdata->gravityDir.x == 1.0 && pdata->gravityDir.y == 0.0 && g->keys[ALLEGRO_KEY_D]) ||
			(pdata->gravityDir.x == -1.0 && pdata->gravityDir.y == 0.0 && g->keys[ALLEGRO_KEY_A])) {
			pdata->state = SHORT;
		}
	}
	else
		pdata->state = FALLING;

	double *moveAxis;
	char rightMove, leftMove;

	double max_speed = MAX_SPEED;
	if (pdata->state == SHORT || pdata->state == SHORT_WALKING)
		max_speed = MAX_SPEED / 2.0;

	if (fabs(pdata->gravityDir.x) < fabs(pdata->gravityDir.y)) {
		// Movimentando na horizontal
		moveAxis = &vel.x;
		rightMove = g->keys[ALLEGRO_KEY_D];
		leftMove = g->keys[ALLEGRO_KEY_A];
	}
	else {
		// Movimentando na vertical
		moveAxis = &vel.y;
		rightMove = g->keys[ALLEGRO_KEY_S];
		leftMove = g->keys[ALLEGRO_KEY_W];
	}

	if ((rightMove ^ leftMove)) {
		if (pdata->state == SHORT)
			pdata->state = SHORT_WALKING;
		else if (pdata->state != FALLING)
			pdata->state = WALKING;

		if (rightMove) {
			pdata->direction = 1;
			if (*moveAxis < max_speed) { 
				if (*moveAxis < 0)
					*moveAxis += WALK_DEACC;
				else
					*moveAxis += WALK_ACC;
				if (*moveAxis > max_speed)
					*moveAxis = max_speed;
			}
		}
		if (leftMove) {
			pdata->direction = -1;
			if (*moveAxis > -max_speed) { 
				if (*moveAxis > 0)
					*moveAxis -= WALK_DEACC;
				else
					*moveAxis -= WALK_ACC;
				if (*moveAxis < -max_speed)
					*moveAxis = -max_speed;
			}
		}
	}
	else {
		if (onGround) {
			if (fabs(*moveAxis) <= WALK_DEACC)
				*moveAxis = 0;
			else if (*moveAxis > 0)
				*moveAxis -= WALK_DEACC;
			else
				*moveAxis += WALK_DEACC;
		}
		else {
			if (fabs(*moveAxis) <= FLOATING_DEACC)
				*moveAxis = 0;
			else if (*moveAxis > 0)
				*moveAxis -= FLOATING_DEACC;
			else
				*moveAxis += FLOATING_DEACC;
		}
	}
	
	if (fabs(pdata->gravityDir.x) < fabs(pdata->gravityDir.y)) {
		if ((isOnSurface(e, g, vec_create(1.0, 0.0))  && vel.x > 0) || 
			(isOnSurface(e, g, vec_create(-1.0, 0.0)) && vel.x < 0))
			vel.x = 0;
		if (isOnSurface(e, g, vec_invert(pdata->gravityDir)) && (pdata->gravityDir.y * vel.y < 0))
			vel.y = 0;
	}
	else {
		if ((isOnSurface(e, g, vec_create(0.0, 1.0))  && vel.y > 0) || 
			(isOnSurface(e, g, vec_create(0.0, -1.0)) && vel.y < 0))
			vel.y = 0;
		if (isOnSurface(e, g, vec_invert(pdata->gravityDir)) && (pdata->gravityDir.y * vel.y < 0))
			vel.x = 0;
	}
	
	e->pos = vec_add(e->pos, vel);


	// Teleporte

	vec shotDir = vec_create(0.0, 0.0);

	if (g->keys[ALLEGRO_KEY_D])
		shotDir.x = 1.0;
	else if (g->keys[ALLEGRO_KEY_A])
		shotDir.x = -1.0;
	if (g->keys[ALLEGRO_KEY_S])
		shotDir.y = 1.0;
	else if (g->keys[ALLEGRO_KEY_W])
		shotDir.y = -1.0;

	if (shotDir.x == 0 && shotDir.y == 0)
		shotDir = pdata->lastDir;

	if (g->keys[ALLEGRO_KEY_N]) {
		g->keys[ALLEGRO_KEY_N] = 0;
		init_shot(g, vec_add(e->pos, vec_mult(pdata->gravityDir, -20)), vec_mult(vec_normalize(pdata->lastDir), SHOTSPEED), e);
		vel = vec_sub(vel, vec_mult(vec_normalize(pdata->lastDir), DASH_STRENGTH));
	}
	pdata->lastDir = shotDir;


	// Colisões
	entity *col = collides_env(e, g->envs[COLLISION_ENV]);

	if (col) {
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
		
		if (isOnSurface(e, g, pdata->gravityDir)) {
			if (pdata->gravityDir.x != 0)
				vel.x = 0;
			else
				vel.y = 0;	
		}
	}

	pdata->velocity = vel;

	g->camera->pos = vec_create(e->pos.x - WW/2, e->pos.y - HH / 2 - 50.0);
	if (g->camera->pos.x < 0)
		g->camera->pos.x = 0;
	if (g->camera->pos.y < 0)
		g->camera->pos.y = 0;

	return 0;
}

int render_player(entity *e, game *g)
{
    struct playerData *pdata = e->data;

	vec newPos = position_to_camera(g, e->pos);
	newPos = vec_sub(newPos, e->hitbox->offset);

	switch (pdata->state) {
		case IDLE:
			e->sprite = pdata->idleSprite;
			break;
		case WALKING:
			e->sprite = pdata->walkingSprite;
			break;
		case SHORT:
			e->sprite = pdata->shortSprite;
			break;
		case SHORT_WALKING:
			e->sprite = pdata->shortSpriteWalk;
			break;
		case FALLING:
			e->sprite = pdata->idleSprite;
			break;
		case JUMPING:
			// Manter o sprite atual
			break;
		default:
			e->sprite = pdata->idleSprite;
			break;
	}

	if (pdata->direction == 1)
		camera_render(g, e->pos, e->sprite, 3.0, 3.0);
	else
		camera_render(g, e->pos, e->sprite, -3.0, 3.0);
	//al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, al_map_rgba(255, 0, 0, 0.01));

	camera_render_hitbox(g, e);

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

	pdata->shortSprite = sprite_create(g, 2, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/short_0.png",
		"assets/characters/short_1.png",
	}, SPR_BOTTOM);
    
	pdata->shortSpriteWalk = sprite_create(g, 4, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/shortwalk_0.png",
		"assets/characters/shortwalk_1.png",
		"assets/characters/shortwalk_2.png",
		"assets/characters/shortwalk_3.png",
	}, SPR_BOTTOM);

    
	// Movimentação
	player->pos = pos;
	pdata->velocity = vec_create(0.0, 0.0);
	pdata->gravityDir = vec_create(0.0, 1.0);
	pdata->lastDir = vec_create(1.0, 0.0);

	subscribe(g->envs[PROCESS_ENV], player, process_player);
    subscribe(g->envs[RENDER_ENV], player, render_player);

	g->player = player;

    return player->id;
}

int destroy_player(game *g, entity *e)
{
    if(!g || !e)
        return 1;

	g->player = NULL;

	struct playerData *pdata = e->data;

	sprite_destroy(pdata->shortSprite);
	sprite_destroy(pdata->shortSpriteWalk);
	sprite_destroy(pdata->idleSprite);
	sprite_destroy(pdata->walkingSprite);

    return 0;
}

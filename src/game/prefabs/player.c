#ifndef __PLAYER
#define __PLAYER

#include "engine.h"
#include "player.h"

struct playerData {
	vec velocity;
	int direction;

	sprite *walkingSprite;
	sprite *idleSprite;
};

int process_player(entity *e, game *g)
{
    struct playerData *pdata = e->data;
	vec vel = pdata->velocity;
	if (g->keys[ALLEGRO_KEY_D]) {
		vel.x = 10.0;
		pdata->direction = 1;
	}
	else if (g->keys[ALLEGRO_KEY_A]) {
		vel.x = -10.0;
		pdata->direction = -1;
	}
	else {
		vel.x = 0.0;
	}

	vel.y += 2.0;
	
	e->pos.y ++;
	if (collides_env(e, g->envs[5])) {
		vel.y = 0.0;
		if (g->keys[ALLEGRO_KEY_W])
			vel.y = -30.0;
	}
	e->pos.y --;

	if (collides_env(e, g->envs[5])) {
		vel.y = 0;
		e->pos.y = e->pos.y / 1;
		while (collides_env(e, g->envs[5]))
			e->pos.y -= 1.0;
	}

	pdata->velocity = vel;
	e->pos = vec_add(e->pos, vel);

	g->camera->pos.x = e->pos.x - WW/2;
	g->camera->pos.y = e->pos.y - HH/2;

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

    
    player->pos = pos;
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

#endif
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "entity.h"
#include "collision.h"
#include "vec.h"
#include "game.h"
#include "spriteProvider.h"
#include "env.h"
#include "tilemap.h"
#include "camera.h"
#include "level.h"
#include "sprite.h"



int soil_render(entity *e, game *G)
{
	vec newPos = position_to_camera(G, e->pos);
	//al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, C_WHITE);
	camera_render(G, e->pos, e->sprite, 3.0, 3.0);

	return 0;
}

int camera_process(entity *e, game *G)
{
	if (G->keys[ALLEGRO_KEY_RIGHT])
		e->pos.x += 10.0;
	if (G->keys[ALLEGRO_KEY_LEFT])
		e->pos.x -= 10.0;
	if (G->keys[ALLEGRO_KEY_UP])
		e->pos.y -= 10.0;
	if (G->keys[ALLEGRO_KEY_DOWN])
		e->pos.y += 10.0;

	return 0;
}

struct playerData {
	vec velocity;
	int direction;

	sprite *walkingSprite;
	sprite *idleSprite;
};

int player_process(entity *e, game *G)
{
	struct playerData *pdata = e->data;
	vec vel = pdata->velocity;
	if (G->keys[ALLEGRO_KEY_D]) {
		vel.x = 10.0;
		pdata->direction = 1;
	}
	else if (G->keys[ALLEGRO_KEY_A]) {
		vel.x = -10.0;
		pdata->direction = -1;
	}
	else {
		vel.x = 0.0;
	}

	vel.y += 2.0;
	
	e->pos.y ++;
	if (collides_env(e, G->envs[5])) {
		vel.y = 0.0;
		if (G->keys[ALLEGRO_KEY_W])
			vel.y = -30.0;
	}
	e->pos.y --;

	if (collides_env(e, G->envs[5])) {
		vel.y = 0;
		e->pos.y = e->pos.y / 1;
		while (collides_env(e, G->envs[5]))
			e->pos.y -= 1.0;
	}

	pdata->velocity = vel;
	e->pos = vec_add(e->pos, vel);

	G->camera->pos.x = e->pos.x - WW/2;
	G->camera->pos.y = e->pos.y - HH/2;

	return 0;
}


int player_render(entity *e, game *g)
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

int load1(game *g)
{
	if (!g)
		return 1;

	/*
	int cameraID = camera_create(g);

	entity *player = entity_create(g);
	subscribe(g->envs[RENDER_ENV], player, player_render);
	player->pos = vec_create(100.0, 100.0);
	player->sprite = spriteProvider_get(g->sprites, "assets/mysha.png");
	hitbox_attatch(player, 32.0, 32.0, 0);

	return 0;
*/
	return 0;
}

int unload1(game *g)
{
	if (!g)
		return 1;

	return 0;
}

int load2(game *g)
{
	camera_create(g);
	
	entity *player = entity_create(g);
	player->pos = vec_create(200.0, 200.0);
	player->sprite = sprite_create(g, 4, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/walking_0.png",
		"assets/characters/walking_1.png",
		"assets/characters/walking_2.png",
		"assets/characters/walking_3.png"
	}, SPR_BOTTOM);
	hitbox_attatch(player, 32.0, 48.0, HB_BOTTOM);
	
	struct playerData *pdata = malloc(sizeof(struct playerData));
	pdata->velocity = vec_create(0.0, 0.0);
	pdata->direction = 1;

	pdata->walkingSprite = player->sprite;
	pdata->idleSprite = sprite_create(g, 2, vec_create(32.0, 32.0), (const char *[]){
		"assets/characters/idle_0.png",
		"assets/characters/idle_1.png",
	}, SPR_BOTTOM);

	player->data = pdata;
	subscribe(g->envs[PROCESS_ENV], player, player_process);
	subscribe(g->envs[RENDER_ENV], player, player_render);

	subscribe(g->envs[PROCESS_ENV], g->camera, camera_process);

	int collision_env = insert_env(g, processEnv_create(g->n_envs));

	const char *map_data[] = {
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000000000000000000000",
		"000000000000000000000111110000000000000",
		"000000000000000000000111110000000000000",
		"000000000000000000000111110000000000000",
		"000000000000000000000111110000000000000",
		"111111111111111111111111111111111111111",
		"111111111111111111111111111111111111111",
		"111111111111111111111111111111111111111",
	};

	tilemap_load(g, map_data, 48, 17, 40, collision_env, RENDER_ENV, soil_render);


	return 0;
}

int unload2(game *g)
{
	if (!g)
		return 1;

	return 0;
}

int main()
{	
	level *l1 = level_create(load1, unload1);
	level *l2 = level_create(load2, unload2);

	level *levels[] = {l1, l2};

	game *G = game_create(2, 1, levels);

	/*
	

	*/

	if (game_process(G))
		return 1;
		
	if (game_destroy(G))
		return 1;
	
	return 0;
}

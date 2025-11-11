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

game *G;
int cameraID;

int soil_render(entity *e)
{
	vec newPos = position_to_camera(G, cameraID, e->pos);
	al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, C_WHITE);
}

int camera_process(entity *e)
{
	if (G->keys[ALLEGRO_KEY_RIGHT])
		e->pos.x += 10.0;
	if (G->keys[ALLEGRO_KEY_LEFT])
		e->pos.x -= 10.0;
	if (G->keys[ALLEGRO_KEY_UP])
		e->pos.y -= 10.0;
	if (G->keys[ALLEGRO_KEY_DOWN])
		e->pos.y += 10.0;
}

int player_render(entity *e)
{
	vec newPos = position_to_camera(G, cameraID, e->pos);

	camera_render(G, cameraID, e->pos, e->sprite);
	al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, C_RED);
}

struct playerData {
	vec velocity;
};

int player_process(entity *e)
{
	struct playerData *pdata = e->data;
	vec vel = pdata->velocity;
	if (G->keys[ALLEGRO_KEY_D])
		vel.x = 10.0;
	else if (G->keys[ALLEGRO_KEY_A])
		vel.x = -10.0;
	else
		vel.x = 0.0;

	vel.y += 2.0;
	
	e->pos.y ++;
	if (collides_env(e, G->envs[4])) {
		vel.y = 0.0;
		if (G->keys[ALLEGRO_KEY_W])
			vel.y = -30.0;
	}
	e->pos.y --;

	if (collides_env(e, G->envs[4])) {
		vel.y = 0;
		e->pos.y = e->pos.y / 1;
		while (collides_env(e, G->envs[4]))
			e->pos.y -= 1.0;
	}

	pdata->velocity = vel;
	e->pos = vec_add(e->pos, vel);
}

int main()
{	
	G = game_create();
	
	int collision_env = insert_env(G, processEnv_create(G->n_envs));
	cameraID = camera_create(G);
	entity *camera = G->envs[MASTER_ENV]->get(cameraID, G->envs[MASTER_ENV]);
	subscribe(G->envs[PROCESS_ENV], camera, camera_process);

	entity *player = entity_create(G);
	player->pos = vec_create(100.0, 100.0);
	player->sprite = spriteProvider_get(G->sprites, "assets/mysha.png");
	hitbox_attatch(player, 32.0, 32.0, 0);

	struct playerData *pdata = malloc(sizeof(struct playerData));
	pdata->velocity = vec_create(0.0, 0.0);
	player->data = pdata;
	
	subscribe(G->envs[PROCESS_ENV], player, player_process);
	subscribe(G->envs[RENDER_ENV], player, player_render);


	const char *map_data[] = {
		"11111111111111111111",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000001111001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"10000000000000000001",
		"11111111111111111111",
	};

	tilemap_load(G, map_data, 64, 15, 20, collision_env, RENDER_ENV, soil_render);

	if (game_process(G))
		return 1;
		
	if (game_destroy(G))
		return 1;
	
	return 0;
}

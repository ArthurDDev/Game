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

game *G;

int soil_render(entity *e)
{
	al_draw_filled_rectangle(e->pos.x, e->pos.y, e->pos.x + e->hitbox->size.x, e->pos.y + e->hitbox->size.y, C_WHITE);
}


int player_render(entity *e)
{
	al_draw_bitmap(e->sprite, e->pos.x, e->pos.y, 0);
	al_draw_filled_rectangle(e->pos.x, e->pos.y, e->pos.x + e->hitbox->size.x, e->pos.y + e->hitbox->size.y, C_RED);
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

	entity *player = entity_create(G);
	player->pos = vec_create(100.0, 100.0);
	player->sprite = spriteProvider_get(G->sprites, "assets/mysha.png");
	hitbox_attatch(player, 32.0, 32.0, 0);

	struct playerData *pdata = malloc(sizeof(struct playerData));
	pdata->velocity = vec_create(0.0, 0.0);
	player->data = pdata;
	
	subscribe(G->envs[PROCESS_ENV], player, player_process);
	subscribe(G->envs[RENDER_ENV], player, player_render);

	entity *soil = entity_create(G);
	soil->pos = vec_create(0.0, 400.0);
	hitbox_attatch(soil, WW, 100.0, 0);
	subscribe(G->envs[RENDER_ENV], soil, soil_render);
	subscribe(G->envs[collision_env], soil, NULL);

	soil = entity_create(G);
	soil->pos = vec_create(300.0, 300.0);
	hitbox_attatch(soil, 400.0, 50.0, 0);
	subscribe(G->envs[RENDER_ENV], soil, soil_render);
	subscribe(G->envs[collision_env], soil, NULL);

	if (game_process(G))
		return 1;
		
	if (game_destroy(G))
		return 1;
	
	return 0;
}

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

entity *floor;

int floor_render(entity *e)
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

	vel.y += 2.0; // gravidade
	
	e->pos.y ++;
	if (G->keys[ALLEGRO_KEY_W] && collides(e, floor))
		vel.y = -30.0;
	e->pos.y --;

	printf("Player vel: (%f, %f)\n", vel.x, vel.y);

	if (collides(e, floor)) {
		vel.y = 0;
		e->pos.y = e->pos.y / 1;
		while (collides(e, floor))
			e->pos.y -= 1.0;
	}

	pdata->velocity = vel;
	e->pos = vec_add(e->pos, vel);
}

int main(){
	
	G = game_create();

	entity *player = entity_create(vec_create(100.0, 100.0));
	player->sprite = spriteProvider_get(G->sprites, "assets/mysha.png");

	hitbox_attatch(player, 32.0, 32.0, 0);
	struct playerData *pdata = malloc(sizeof(struct playerData));
	pdata->velocity = vec_create(0.0, 0.0);
	player->data = pdata;

	G->envs[PROCESS_ENV]->subscribe(player, G->envs[PROCESS_ENV], player_process);
	G->envs[RENDER_ENV]->subscribe(player, G->envs[RENDER_ENV], player_render);

	floor = entity_create(vec_create(0.0, 400.0));
	hitbox_attatch(floor, WW, 100.0, 0);

	G->envs[RENDER_ENV]->subscribe(floor, G->envs[RENDER_ENV], floor_render);

	if (game_process(G))
		return 1;

	if (game_destroy(G))
		return 1;

	return 0;
}

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "entity.h"
#include "vec.h"
#include "game.h"
#include "spriteProvider.h"

game *G;

int player_render(entity *e)
{
	al_draw_bitmap(e->sprite, e->pos.x, e->pos.y, 0);
}

int player_process(entity *e)
{
	vec acc = vec_create(0.0, 0.0);
	if (G->keys[ALLEGRO_KEY_D])
		acc.x += 10.0;
	if (G->keys[ALLEGRO_KEY_A])
		acc.x -= 10.0;
	if (G->keys[ALLEGRO_KEY_W])
		acc.y -= 10.0;
	if (G->keys[ALLEGRO_KEY_S])
		acc.y += 10.0;

	e->pos = vec_add(e->pos, acc);
}

int main(){
	
	G = game_create();

	entity *player = entity_create(vec_create(100.0, 100.0));
	player->sprite = spriteProvider_get(G->sprites, "assets/mysha.png");
	G->envs[PROCESS_ENV]->subscribe(player, G->envs[PROCESS_ENV], player_process);
	G->envs[RENDER_ENV]->subscribe(player, G->envs[RENDER_ENV], player_render);

	if (game_process(G))
		return 1;

	if (game_destroy(G))
		return 1;

	return 0;
}

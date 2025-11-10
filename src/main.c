#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "entity.h"
#include "vec.h"
#include "game.h"
#include "spriteProvider.h"


int player_process(entity *e) {
	printf("Eu estou processando algo\n");
};

int player_render(entity *e) {
	al_draw_bitmap(e->sprite, 100, 100, 0);
}

int main(){
	
	game *G = game_create();
	
	///
	vec pos = {0.0, 0.0};
	entity *player = entity_create(pos);
	
	player->sprite = spriteProvider_get(G->sprites, "assets/mysha.png");
	G->envs[RENDER_ENV]->subscribe(player, G->envs[RENDER_ENV], player_render);
	///

	if (game_process(G))
		return 1;

	if (game_destroy(G))
		return 1;

	return 0;
}

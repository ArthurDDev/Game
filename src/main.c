#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include "entity.h"
#include "vec.h"
#include "game.h"

int main(){
	
	game *G = game_create();

	if (game_process(G))
		return 1;

	if (game_destroy(G))
		return 1;

	return 0;
}

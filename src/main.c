#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "engine.h"

#include "menu.h"
#include "level_1.h"

int main()
{
	level *levels[] = {
		level_create(load_menu, unload_menu), 
		level_create(load_l1, unload_l1),
	};

	game *G = game_create(2, 1, levels);

	if (game_process(G))
		return 1;
		
	if (game_destroy(G))
		return 1;
	
	return 0;
}

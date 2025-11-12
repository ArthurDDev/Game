#include "world.h"
#include "engine.h"


int soil_render(entity *e, game *G)
{
	vec newPos = position_to_camera(G, e->pos);
	//al_draw_filled_rectangle(newPos.x, newPos.y, newPos.x + e->hitbox->size.x, newPos.y + e->hitbox->size.y, C_WHITE);
	camera_render(G, e->pos, e->sprite, 3.0, 3.0);

	return 0;
}

#include "world.h"
#include "engine.h"


int soil_render(entity *e, game *G)
{
	camera_render(G, e->pos, e->sprite, 3.0, 3.0);

	return 0;
}

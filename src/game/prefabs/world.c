#include "world.h"
#include "engine.h"

int soil_render(entity *e, game *G)
{
	camera_render(G, e->pos, e->sprite, 3.0, 3.0);

	return 0;
}

int spike_render(entity *e, game *G)
{
	camera_render(G, e->pos, e->sprite, 3.0, 3.0);

	return 0;
}

int spike_init(game *g, int dir, vec pos)
{
	const char *image_path;
	entity *spike = entity_create(g);
	spike->pos = pos;

	switch (dir) {
		case 0:
			image_path = "assets/spikes/spikes0.png";
			hitbox_attatch(spike, 32.0, 8.0, HB_BOTTOM);
			break;
		case 1:
			image_path = "assets/spikes/spikes1.png";
			hitbox_attatch(spike, 8.0, 32.0, HB_RIGHT);
			break;
		case 2:
			image_path = "assets/spikes/spikes2.png";
			hitbox_attatch(spike, 32.0, 8.0, HB_TOP);
			break;
		case 3:
			image_path = "assets/spikes/spikes3.png";
			hitbox_attatch(spike, 8.0, 8.0, HB_LEFT);
			break;
		default:
			return 1;
	}

	spike->sprite = sprite_create(g, 1, vec_create(16.0, 16.0), (const char *[]){
		image_path,
	}, SPR_TOP | SPR_LEFT);

	subscribe(g->envs[RENDER_ENV], spike, spike_render);

	return 0;
}

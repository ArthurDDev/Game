#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include "entity.h"
#include "vec.h"

#define C_BLACK al_map_rgb(0, 0, 0)
#define C_WHITE al_map_rgb(255, 255, 255)

#define WW 1000
#define HH 500

int main(){
	al_init();																		
	al_install_keyboard();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(WW, HH);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	ALLEGRO_EVENT event;
	al_start_timer(timer);

	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info(1, &info);
	printf("%d %d %d %d\n", info.x1, info.y1, info.x1, info.x2);

	al_set_window_position(disp, (info.x1 + info.x2) / 2 - WW/2, (info.y1 + info.y2) / 2 - HH/2);

	vec pos = {50.0, 30.0};
	entity *player = entity_create(pos, processPlayer, renderPlayer);
	player->process(player);

	while(1){
		al_wait_for_event(queue, &event);

		if (event.type == 30){

			al_clear_to_color(C_BLACK);

    		al_flip_display();
		}
		else if (event.type == 42) break;
	}

	player = entity_destroy(player);

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}

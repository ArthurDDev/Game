#ifndef __CAMERA
#define CAMERA

#include <allegro5/allegro.h>

typedef struct game game;
typedef struct vec vec;

// Retorna o ID da camera
int camera_create(game *G);

// Retorna um ID atualizado com os dados
vec position_to_camera(game *G, int cameraID, vec pos);

// Renderiza um sprite com a posição atualizada
void camera_render(game *G, int cameraID, vec pos, ALLEGRO_BITMAP *bmp);

#endif
#ifndef __CAMERA
#define __CAMERA

typedef struct game game;
typedef struct vec vec;
typedef struct sprite sprite;

// Retorna o ID da camera
int camera_create(game *G);

// Retorna um ID atualizado com os dados
vec position_to_camera(game *G, vec pos);

// Renderiza um sprite com a posição atualizada
void camera_render(game *G, vec pos, sprite *spr, double scale_x, double scale_y);

#endif
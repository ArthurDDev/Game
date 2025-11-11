#ifndef __LEVEL
#define __LEVEL

typedef struct game game;

typedef struct level level;
struct level {
    int (*load) (game *G);
    int (*unload) (game *G);
};

level *level_create(int (*load) (game *G), int (*unload) (game *G));
level *level_destroy(level *l);

void *level_change(game *g, int next_level);

#endif
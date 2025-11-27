#ifndef __VEC
#define __VEC

typedef struct vec vec;
struct vec {
    double x;
    double y;
};

vec vec_create(double x, double y);

vec vec_add(vec a, vec b);
vec vec_sub(vec a, vec b);
vec vec_mult(vec a, double b);
vec vec_normalize(vec a);
vec vec_invert(vec a);

#endif
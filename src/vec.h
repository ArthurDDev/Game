#ifndef __VEC
#define __VEC

typedef struct vec {
    double x;
    double y;
} vec;

vec vec_create(double x, double y);

vec vec_add(vec a, vec b);
vec vec_sub(vec a, vec b);

#endif
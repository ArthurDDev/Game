#include <math.h>

#include "vec.h"


vec vec_create(double x, double y)
{
    vec v;
    v.x = x;
    v.y = y;
    
    return v;
}

vec vec_add(vec a, vec b)
{
    vec v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;

    return v;
}

vec vec_sub(vec a, vec b)
{
    vec v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;

    return v;
}

vec vec_mult(vec a, double b)
{
    return vec_create(a.x * b, a.y * b);
}

vec vec_normalize(vec a)
{
    double mod = sqrt(a.x * a.x + a.y * a.y);

    return vec_create(a.x / mod, a.y / mod);
}

vec vec_invert(vec a)
{
    return vec_create(-a.x, -a.y);
}
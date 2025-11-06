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
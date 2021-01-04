#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector2.h"

struct Transform {
    Vector2 pos;
    Vector2 size;
    int scale = 1;
};

#endif
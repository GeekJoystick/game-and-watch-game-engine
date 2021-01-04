#ifndef SPRITE_H
#define SPRITE_H
#include <stdint.h>

struct Sprite {
    uint16_t width, height;
    const char* data;
};

#endif
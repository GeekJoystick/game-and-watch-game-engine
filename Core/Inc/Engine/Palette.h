#ifndef PALETTE_H
#define PALETTE_H
#include <stdint.h>

class Palette {
    uint16_t colors[16];
public:
    uint16_t GetColor(char color){
      return colors[(color & 15)];
    }
    uint16_t GetUpColor(char color) {
        return colors[(color & 240) >> 4];
    }
    uint16_t GetDownColor(char color) {
        return colors[(color & 15)];
    }
    void SetColors(uint16_t _colors[16]) {
        for (int i = 0; i < 16; i++){
          colors[i] = _colors[i];
        }
    }
};

#endif
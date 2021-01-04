#ifndef PALETTE_H
#define PALETTE_H
#include <stdint.h>

class Palette {
    uint16_t colors[16];
public:
    /**
     * Returns the color based on the data given.
     * Concerned data is only the lower four bits
     * of the byte you give.
     * @param color The color id byte
     */
    uint16_t GetColor(char color){
      return colors[(color & 15)];
    }

    /**
     * Returns the color based on the higher four bits
     * of the byte you give.
     * @param color The color id byte
     */
    uint16_t GetUpColor(char color) {
        return colors[(color & 240) >> 4];
    }
    /**
     * Returns the color based on the lower four bits
     * of the byte you give.
     * @param color The color id byte
     */
    uint16_t GetDownColor(char color) {
        return colors[(color & 15)];
    }

    /**
     * Set the palette colors.
     * @param colors Color data
     */
    void SetColors(uint16_t _colors[16]) {
        for (int i = 0; i < 16; i++){
          colors[i] = _colors[i];
        }
    }
};

#endif
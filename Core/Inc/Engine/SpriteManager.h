#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H
#include <stdint.h>
#include "Sprite.h"

class SpriteManager {
    Sprite* sprites[MAX_SPRITES];
    static SpriteManager* instance;
public:
    static SpriteManager* GetInstance() {
        if (instance == nullptr) {
            instance = new SpriteManager();
        }
        return instance;
    }
    /** 
     * Replaces a certain sprite slot with a given sprite
     * @param[in] id The sprite slot to write to
     * @param[in] sprite A pointer to the sprite we want to set
     */
    void SetSprite(uint16_t id, Sprite *sprite) {
        if (sprites[id] != nullptr){
          sprites[id]->~Sprite();
        }
        sprites[id] = sprite;
    }
    void CreateSprite(uint16_t id, const char* data, uint16_t w, uint16_t h) {
        if (sprites[id] != nullptr){
          sprites[id]->~Sprite();
        }
        sprites[id] = new Sprite{ w, h, (char*)data };
    }
    Sprite* GetSprite(uint16_t id) {
        return sprites[id];
    }
};
SpriteManager* SpriteManager::instance = nullptr;

#endif
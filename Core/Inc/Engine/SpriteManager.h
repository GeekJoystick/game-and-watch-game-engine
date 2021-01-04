#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H
#include <stdint.h>
#include "Defines.h"
#include "Sprite.h"

class SpriteManager {
    Sprite* sprites[MAX_SPRITES];
    static SpriteManager* instance;
public:
    /**
     * Returns a singleton instance of the class. 
     * Note: you can have other instances, but you can't
     * access them through this method.
     */
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

    /**
     * Creates a sprite within a certain slot.
     * @param[in] id The sprite slot to write to
     * @param[in] data The sprite data to be written
     * @param[in] w The width of the sprite
     * @param[in] h The height of the sprite
     */
    void CreateSprite(uint16_t id, const char* data, uint16_t w, uint16_t h) {
        if (sprites[id] != nullptr){
          sprites[id]->~Sprite();
        }
        sprites[id] = new Sprite{ w, h, (char*)data };
    }

    /**
     * Returns the sprite pointer stored in the given slot.
     * @param[in] id The sprite slot to get the sprite from
     */
    Sprite* GetSprite(uint16_t id) {
        return sprites[id];
    }
};
SpriteManager* SpriteManager::instance = nullptr;

#endif
#ifndef ENTITY_H
#define ENTITY_H
#include <stdint.h>
#include "Vector2.h"
#include "Transform.h"
#include "KeyState.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Renderer.h"

class EntityManager;
class Entity {
protected:
    Transform transform;
    bool flipX, flipY;
    uint16_t spriteID;
    const char* tag = "None";
public:
    Entity(Vector2 _pos) {
        transform.pos = _pos;
        transform.size = {8, 8};
        spriteID = 0;
        flipX = false;
        flipY = false;
    }

    Transform* GetTransform() {
        return &transform;
    }

    const char* Tag() {
        return tag;
    }

    virtual void Events(KeyState buttons) {
        
    }

    virtual void Update(EntityManager* entityManager) {

    }

    virtual void Draw(Renderer* renderer) {
        Sprite* sprite = SpriteManager::GetInstance()->GetSprite(spriteID);
        renderer->DrawSprite(sprite, transform.pos.x, transform.pos.y, transform.scale, flipX, flipY);
    }
};

#endif
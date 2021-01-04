#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <stdint.h>
#include "Defines.h"
#include "Entity.h"

class EntityManager {
    static EntityManager* instance;
    Entity* entities[MAX_ENTITIES];
    int freeEntities = 0;
    uint16_t currentEntity;
public:
    /**
     * Returns a singleton instance of the class. 
     * Note: you can have other instances, but you can't
     * access them through this method.
     */
    static EntityManager* GetInstance() {
        if (instance == nullptr) {
            instance = new EntityManager();
        }
        return instance;
    }

    /**
     * Creates an entity at given slot
     * Example syntax: CreateEntity<EntityClass>(0);
     */
    template <class T>
    T* CreateEntity(uint16_t pos) {
        if (pos >= 0 && pos < MAX_ENTITIES) {
            T* entity = new T();
            entities[pos] = entity;
            return entity;
        }
        return nullptr;
    }

    /**
     * Returns a pointer to the entity located in the given slot
     * @param ID slot to get Entity from
     */
    Entity* GetEntity(int ID) {
        if (ID >= 0 && ID < MAX_ENTITIES) {
            return entities[ID];
        }
        return nullptr;
    }

    void Events(KeyState buttons) {
        for (auto entity : entities) {
            if (entity != nullptr) {
                entity->Events(buttons);
            }
        }
    }

    void Update() {
        for (auto entity : entities) {
            if (entity != nullptr) {
                entity->Update(this);
            }
        }
    }

    void Draw(Renderer* ren) {
        for (auto entity : entities) {
            if (entity != nullptr) {
                entity->Draw(ren);
            }
        }
    }
public:
};
EntityManager* EntityManager::instance = nullptr;

#endif
//Switch these defs to see if your program is compatible
//with both desktop and game and watch.
//They won't affect compilation
#ifndef COMPILING
#define DESKTOP
//define GAMENWATCH
#endif

#include "GameAndWatchEngine.h"

class Player: public Entity {
protected:
    float vx = 0;
    float vy = 0;
    float speed = 3;
public:
    Player()
        : Entity(0, 0)
    {
        spriteID = 0;
        tag = T_Player;
    }
    void Events(uint32_t buttons) override{
         if (buttons & B_Up){
           vy = -speed;
           vx = 0;
         }else if (buttons & B_Down){
           vy = speed;
           vx = 0;
         }else if (buttons & B_Left){
           vx = -speed;
           vy = 0;
         }else if (buttons & B_Right){
           vx = speed;
           vy = 0;
         }
    }

    void Update(EntityManager* entityManager) override{
      if (!paused){
        if (vx != 0 && vy != 0) {
            vx = 0.707f * (vx / abs(vx));
            vy = 0.707f * (vy / abs(vy));
        }
        if (vx < 0.0f) {
            flipX = true;
        }
        else if (vx > 0.0f) {
            flipX = false;
        }

        transform.x += vx;
        transform.y += vy;
      }
    }


};

class Fuel : public Entity{
public:
    Fuel()
        : Entity(0, 0)
    {
        spriteID = 1;
        tag = T_Fuel;
    }

    void Update(EntityManager* entityManager) override {
      if (!paused){
        Entity* entity;
        for (int i = 0; i < MAX_ENTITIES; i++) {
          entity = entityManager->GetEntity(i);
          if (entity != nullptr) {
            switch (entity->Tag()) {
            case T_Player:
              Transform other = *entity->GetTransform();
              if (Math::RectCollision(transform, other)) {
                  transform.x = rand() % (int)(WIDTH - (transform.width*transform.scale));
                  transform.y = rand() % (int)(HEIGHT - (transform.height*transform.scale));
              }
              break;
            }
          }
        }
      }
    }
};

class PutTheGuyOnFireOut: public Game{
public:
  PutTheGuyOnFireOut()
  : Game()
  {
    uint16_t colors[16] = {0x0000, 0xD596, 0xE1E1, 0xC041, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    palette.SetColors(colors);

    renderer->SetPalette(&palette);

    spriteManager->CreateSprite(0, (char*)"\x00\x30\x00\x00\x00\x03\x33\x00\x00\x32\x33\x30\x00\x11\x23\x30\x03\x11\x21\x33\x03\x22\x22\x33\x03\x22\x22\x33\x00\x33\x33\x00", 4, 8);
    spriteManager->CreateSprite(1, (char*)"\x30\x02\x23\x00\x30\x20\x00\x30\x03\x22\x22\x23\x02\x23\x23\x23\x03\x22\x32\x23\x03\x23\x23\x23\x03\x22\x22\x23\x03\x33\x33\x33", 4, 8);

    entityManager->CreateEntity<Player>(0);
    entityManager->CreateEntity<Fuel>(1);
  }
};

int main(void)
{
  PutTheGuyOnFireOut game;
  game.Start();
}
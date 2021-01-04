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
    Vector2 vel;
    float speed = 3;
public:
    Player()
        : Entity({0, 0})
    {
        spriteID = 0;
        transform.scale = 3;
        tag = "Player";
    }
    void Events(KeyState buttons) override{
         if (buttons.Up){
           vel.y = -speed;
           vel.x = 0;
         }else if (buttons.Down){
           vel.y = speed;
           vel.x = 0;
         }else if (buttons.Left){
           vel.x = -speed;
           vel.y = 0;
         }else if (buttons.Right){
           vel.x = speed;
           vel.y = 0;
         }
    }

    void Update(EntityManager* entityManager) override{
      if (vel.x != 0 && vel.y != 0) {
          vel.x = 0.707f * (vel.x / abs(vel.x));
          vel.y = 0.707f * (vel.y / abs(vel.y));
      }
      if (vel.x < 0.0f) {
          flipX = true;
      }
      else if (vel.x > 0.0f) {
          flipX = false;
      }

      transform.pos.x += vel.x;
      transform.pos.y += vel.y;
    }
};

class Fuel : public Entity{
public:
    Fuel()
        : Entity({0, 0})
    {
        spriteID = 1;
        transform.scale = 3;
        tag = "Fuel";
    }

    void Update(EntityManager* entityManager) override {
      Entity* entity;
      for (int i = 0; i < MAX_ENTITIES; i++) {
        entity = entityManager->GetEntity(i);
        if (entity != nullptr) {
          if (entity->Tag() == "Player"){
            Transform other = *entity->GetTransform();
            if (Math::RectCollision(transform, other)) {
                transform.pos.x = rand() % (int)(WIDTH - (transform.size.x*transform.scale));
                transform.pos.y = rand() % (int)(HEIGHT - (transform.size.y*transform.scale));
            }
          }
        }
      }
    }
};

class PutTheGuyOnFireOut: public Engine{
public:
  PutTheGuyOnFireOut()
  : Engine()
  {
    uint16_t colors[16] = {
    0x0000, 0xD596, 0xE1E1, 0xC041, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    palette.SetColors(colors);

    renderer->SetPalette(&palette);

    spriteManager->CreateSprite(0, "\x00\x30\x00\x00\x00\x03\x33\x00\x00\x32\x33\x30\x00\x11\x23\x30\x03\x11\x21\x33\x03\x22\x22\x33\x03\x22\x22\x33\x00\x33\x33\x00", 4, 8);
    spriteManager->CreateSprite(1, "\x30\x02\x23\x00\x30\x20\x00\x30\x03\x22\x22\x23\x02\x23\x23\x23\x03\x22\x32\x23\x03\x23\x23\x23\x03\x22\x22\x23\x03\x33\x33\x33", 4, 8);

    entityManager->CreateEntity<Player>(0);
    entityManager->CreateEntity<Fuel>(1);
  }
};

int main(void)
{
  PutTheGuyOnFireOut game;
  game.Start();
}
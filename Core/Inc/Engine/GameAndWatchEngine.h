#include "Defines.h"
#include "Sprite.h"
#include "Vector2.h"
#include "Transform.h"
#include "Math.h"
#include "Palette.h"
#include "KeyState.h"
#include "Input.h"
#include "Renderer.h"
#include "SpriteManager.h"
#include "Entity.h"
#include "EntityManager.h"

class Engine{
protected:
  Palette palette;
  Renderer* renderer;
  SpriteManager* spriteManager;
  EntityManager* entityManager;

  bool running;
  uint16_t backgroundColor;

public:
  Engine(){
    uint16_t colors[16] = {0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    palette.SetColors(colors);

    renderer = new Renderer(WIDTH, HEIGHT, "Game&Watch engine, by Geek_Joystick");
    renderer->SetPalette(&palette);

    spriteManager = SpriteManager::GetInstance();
    entityManager = EntityManager::GetInstance();

    backgroundColor = 0;
  }

  virtual void Start(){
    running = true;
    while (running)
    {
      Events();

      Update();

      Draw();
    }
  }

protected:
  virtual void Events(){
    KeyState buttons = Input::GetButtons();
    if (buttons.End){
      running = false;
    }

    entityManager->Events(buttons);
  }

  virtual void Update(){
    entityManager->Update();
  }

  virtual void Draw(){
    renderer->Clear(backgroundColor);
    entityManager->Draw(renderer);
    renderer->Update();
  }
};

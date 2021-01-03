#ifndef COMPILING
#define DESKTOP
//define GAMENWATCH
#endif

#define WIDTH 320
#define HEIGHT 240
#define MAX_SPRITES 256
#define MAX_ENTITIES 256

#ifdef DESKTOP
#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>

#define BPP   2
#define SCALE 2
#define B_Left (1 << 0)
#define B_Up (1 << 1)
#define B_Right (1 << 2)
#define B_Down (1 << 3)
#define B_A (1 << 4)
#define B_B (1 << 5)
#define B_TIME (1 << 6)
#define B_GAME (1 << 7)
#define B_PAUSE (1 << 8)

uint16_t framebuffer[WIDTH * HEIGHT];
#endif
#ifdef GAMENWATCH
#include "main.h"

#include "buttons.h"
#include "flash.h"
#include "lcd.h"
#include <cstring>

LTDC_HandleTypeDef hltdc;

OSPI_HandleTypeDef hospi1;

SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

SPI_HandleTypeDef hspi2;

uint16_t audiobuffer[48000] __attribute__((section (".audio")));

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LTDC_Init(void);
static void MX_SPI2_Init(void);
static void MX_OCTOSPI1_Init(void);
static void MX_SAI1_Init(void);
static void MX_NVIC_Init(void);
#endif

struct Sprite {
    uint16_t width, height;
    const char* data;
};

struct Transform {
    float x = 0;
    float y = 0;
    float width = 8;
    float height = 8;
    int scale = 3;
};

class Math {
public:
    static bool RectCollision(Transform o1, Transform o2) {
        return HorizontalCollision(o1, o2) && VerticalCollision(o1, o2);
    }

    static bool HorizontalCollision(Transform o1, Transform o2) {
        return (
            (((o1.width*o1.scale) > (o2.width*o2.scale)) &&
                ((o2.x >= o1.x && o2.x < o1.x + (o1.width*o1.scale)) ||
                (o2.x + (o2.width*o2.scale) >= o1.x && o2.x + (o2.width*o2.scale) < o1.x + (o1.width*o1.scale)))
            ) || 
            (!((o1.width*o1.scale) > (o2.width*o2.scale)) &&
                ((o1.x >= o2.x && o1.x < o2.x + (o2.width*o2.scale)) ||
                (o1.x + (o1.width*o1.scale) >= o2.x && o1.x + (o1.width*o1.scale) < o2.x + (o2.width*o2.scale)))
            )
        );
    }

    static bool VerticalCollision(Transform o1, Transform o2) {
        return (
            (((o1.height*o1.scale) > (o2.height*o2.scale)) &&
                ((o2.y >= o1.y && o2.y < o1.y + (o1.height*o1.scale)) ||
                    (o2.y + (o2.height*o2.scale) >= o1.y && o2.y + (o2.height*o2.scale) < o1.y + (o1.height*o1.scale)))
                ) ||
            (!((o1.height*o1.scale) > (o2.height*o2.scale)) &&
                ((o1.y >= o2.y && o1.y < o2.y + (o2.height*o2.scale)) ||
                    (o1.y + (o1.height*o1.scale) >= o2.y && o1.y + (o1.height*o1.scale) < o2.y + (o2.height*o2.scale)))
                )
            );
    }

    template <class T, class U, class V>
    static T Clamp(T num, U min, V max) {
        while (num < min) {
            num += max - min;
        }
        while (num >= max) {
            num -= max - min;
        }
        return num;
    }
};

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

struct KeyState{
  bool Up    = false;
  bool Down  = false;
  bool Left  = false;
  bool Right = false;
  bool A     = false;
  bool B     = false;
  bool Game  = false;
  bool Time  = false;
  bool Pause = false;
  bool End = false;
};

class Input{
public:
  static KeyState GetButtons(){
    #ifdef DESKTOP
    SDL_Event event;
    KeyState state;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_WINDOWEVENT){
        if (event.window.event == SDL_WINDOWEVENT_CLOSE){
          state.End = true;
        }
      }
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          state.Up = true;
          break;
        case SDLK_DOWN:
          state.Down = true;
          break;
        case SDLK_LEFT:
          state.Left = true;
          break;
        case SDLK_RIGHT:
          state.Right = true;
          break;
        case SDLK_x:
          state.A = true;
          break;
        case SDLK_c:
          state.B = true;
          break;
        case SDLK_BACKSPACE:
          state.Game = true;
          break;
        case SDLK_RETURN:
          state.Pause = true;
          break;
        case SDLK_RSHIFT:
          state.Time = true;
          break;
        case SDLK_ESCAPE:
          state.End = true;
          break;
        }
      }
    }
    #endif
    #ifdef GAMENWATCH
    uint32_t buttons = get_buttons();
    if (buttons & B_Up)
      state.Up = true;
    if (buttons & B_Down)
      state.Down = true;
    if (buttons & B_Left)
      state.Left = true;
    if (buttons & B_Right)
      state.Right = true;
    if (buttons & B_A)
      state.A = true;
    if (buttons & B_B)
      state.B = true;
    if (buttons & B_GAME)
      state.Game = true;
    if (buttons & B_PAUSE)
      state.Pause = true;
    if (buttons & B_TIME)
      state.Time = true;
    #endif
    return state;
  }
};

class Renderer {
  #ifdef DESKTOP
  SDL_Window* window;
  SDL_Renderer* sdl_renderer;
  SDL_Texture* fb_texture;
  #endif

  Palette* palette;
public:
    Renderer(){};
    Renderer(int width, int height, const char* title)
    {
      #ifdef DESKTOP
      SDL_Init(SDL_INIT_VIDEO);

      window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width * SCALE, height * SCALE,
        0);

      sdl_renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_PRESENTVSYNC);

      fb_texture = SDL_CreateTexture(sdl_renderer,
        SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET,
        width, height);
      #endif
      #ifdef GAMENWATCH
      HAL_Init();
      SystemClock_Config();
      MX_GPIO_Init();
      MX_DMA_Init();
      MX_LTDC_Init();
      MX_SPI2_Init();
      MX_OCTOSPI1_Init();
      MX_SAI1_Init();
      MX_NVIC_Init();

      lcd_init(&hspi2, &hltdc);
      memset(framebuffer, 0xff, 320*240*2);

      flash_memory_map(&hospi1);

      uint32_t add = 0x90000000;
      uint32_t* ptr = (uint32_t*)add;
      if(*ptr == 0x88888888) {
        Error_Handler();
      }
      #endif

      palette = nullptr;
    }
    ~Renderer() {
      #ifdef DESKTOP
      SDL_DestroyTexture(fb_texture);
      SDL_DestroyRenderer(sdl_renderer);
      SDL_DestroyWindow(window);
      #endif
      #ifdef GAMENWATCH

      #endif
    }
    void Update() {
      #ifdef DESKTOP
      SDL_SetRenderTarget(sdl_renderer, NULL);
      SDL_UpdateTexture(fb_texture, NULL, framebuffer, WIDTH * BPP);
      SDL_RenderCopy(sdl_renderer, fb_texture, NULL, NULL);
      SDL_RenderPresent(sdl_renderer);
      SDL_SetRenderTarget(sdl_renderer, fb_texture);
      #endif
      #ifdef GAMENWATCH
      HAL_Delay(20);
      #endif
    }

    void Clear(char color){
        uint16_t c = palette->GetColor(color);
        for (int y = 0; y < HEIGHT; y++){
            for (int x = 0; x < WIDTH; x++){
            framebuffer[y*WIDTH+x] = c;
            }
        }
    }

    void DrawSprite(Sprite* sprite, int x, int y, int scale = 1, bool flipX = false, bool flipY = false, char transparentColor = 0) {
        if (palette == nullptr) {
          #ifdef DESKTOP
          std::cout << "You didn't set a palette!" << std::endl;
          exit(1);
          #endif
          #ifdef GAMENWATCH
          Error_Handler();
          #endif
        }
        int u, v;
        uint16_t tColor = palette->GetColor(transparentColor);
        uint16_t color = 0;
        for (int j = 0; j < sprite->height; j++) {
            for (int i = 0; i < sprite->width; i++) {
                u = 0;
                v = 0;
                for (int k = 0; k < 2; k++) {
                    if ((flipX ? 1 - k : k) > 0) {
                        color = palette->GetDownColor(sprite->data[(flipY ? sprite->height - j - 1 : j) * sprite->width + (flipX ? sprite->width - i - 1 : i)]);
                    }
                    else {
                        color = palette->GetUpColor(sprite->data[(flipY ? sprite->height - j - 1 : j) * sprite->width + (flipX ? sprite->width - i - 1 : i)]);
                    }
                    if (!(color == tColor)){
                      for (int b = 0; b < scale; b++) {
                          for (int a = 0; a < scale; a++) {
                              while ((u + ((i * 2) + k) * scale + a + x) >= WIDTH) {
                                  u -= WIDTH;
                              }
                              while ((u + ((i * 2) + k) * scale + a + x) < 0) {
                                  u += WIDTH;
                              }
                              while ((v + j * scale + b + y) >= HEIGHT) {
                                  v -= HEIGHT;
                              }
                              while ((v + j * scale + b + y) < 0) {
                                  v += HEIGHT;
                              }

                              framebuffer[(((y + j * scale + b + v)) * WIDTH + ((x + ((i * 2) + k) * scale + a + u)))] = color;
                          }
                      }
                    }
                }
            }
        }
    }
    
    void SetPalette(Palette* _palette) {
      palette = _palette;
    }
};

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

class EntityManager;

class Entity {
protected:
    Transform transform;
    bool flipX, flipY;
    uint16_t spriteID;
    const char* tag = "None";
public:
    Entity(float _x, float _y) {
        transform.x = _x;
        transform.y = _y;
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
        renderer->DrawSprite(sprite, transform.x, transform.y, transform.scale, flipX, flipY);
    }
};

class EntityManager {
    static EntityManager* instance;
    Entity* entities[MAX_ENTITIES];
    int freeEntities = 0;
    uint16_t currentEntity;
public:
    static EntityManager* GetInstance() {
        if (instance == nullptr) {
            instance = new EntityManager();
        }
        return instance;
    }

    template <class T>
    T* CreateEntity(uint16_t pos) {
        if (pos >= 0 && pos < MAX_ENTITIES) {
            T* entity = new T();
            entities[pos] = entity;
            return entity;
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

    Entity* GetEntity(int ID) {
        if (ID >= 0 && ID < MAX_ENTITIES) {
            return entities[ID];
        }
        return nullptr;
    }
public:
};
EntityManager* EntityManager::instance = nullptr;

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

#ifdef GAMENWATCH
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 140;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SPI2
                              |RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_OSPI
                              |RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 192;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 5;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3M = 4;
  PeriphClkInitStruct.PLL3.PLL3N = 9;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 24;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_CLKP;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_CLKP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* OCTOSPI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);
}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
  hltdc.Init.HorizontalSync = 9;
  hltdc.Init.VerticalSync = 1;
  hltdc.Init.AccumulatedHBP = 60;
  hltdc.Init.AccumulatedVBP = 7;
  hltdc.Init.AccumulatedActiveW = 380;
  hltdc.Init.AccumulatedActiveH = 247;
  hltdc.Init.TotalWidth = 392;
  hltdc.Init.TotalHeigh = 255;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 320;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 240;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 255;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0x24000000;
  pLayerCfg.ImageWidth = 320;
  pLayerCfg.ImageHeight = 240;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 255;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 0;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 0;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = GFXMMU_VIRTUAL_BUFFER0_BASE;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief OCTOSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */

  /* USER CODE END OCTOSPI1_Init 0 */

  OSPIM_CfgTypeDef sOspiManagerCfg = {0};

  /* USER CODE BEGIN OCTOSPI1_Init 1 */

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThreshold = 4;
  hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_MACRONIX;
  hospi1.Init.DeviceSize = 20;
  hospi1.Init.ChipSelectHighTime = 2;
  hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi1.Init.ClockPrescaler = 1;
  hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  hospi1.Init.ChipSelectBoundary = 0;
  hospi1.Init.ClkChipSelectHighTime = 0;
  hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_BYPASSED;
  hospi1.Init.MaxTran = 0;
  hospi1.Init.Refresh = 0;
  if (HAL_OSPI_Init(&hospi1) != HAL_OK)
  {
    Error_Handler();
  }
  sOspiManagerCfg.ClkPort = 1;
  sOspiManagerCfg.NCSPort = 1;
  sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
  if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */

  /* USER CODE END OCTOSPI1_Init 2 */

}

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_MONOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIO_Speaker_enable_GPIO_Port, GPIO_Speaker_enable_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : GPIO_Speaker_enable_Pin */
  GPIO_InitStruct.Pin = GPIO_Speaker_enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIO_Speaker_enable_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_PAUSE_Pin BTN_GAME_Pin BTN_TIME_Pin */
  GPIO_InitStruct.Pin = BTN_PAUSE_Pin|BTN_GAME_Pin|BTN_TIME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD1 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_A_Pin BTN_Left_Pin BTN_Down_Pin BTN_Right_Pin
                           BTN_Up_Pin BTN_B_Pin */
  GPIO_InitStruct.Pin = BTN_A_Pin|BTN_Left_Pin|BTN_Down_Pin|BTN_Right_Pin
                          |BTN_Up_Pin|BTN_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) {
    // Blink display to indicate failure
    lcd_backlight_off();
    HAL_Delay(500);
    lcd_backlight_on();
    HAL_Delay(500);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif 
#endif
#ifndef RENDERER_H
#define RENDERER_H
#include "Defines.h"
#include "Sprite.h"
#include "Palette.h"

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
    
    /**
     * Updates the window
     */
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

    /**
     * Clears the windows with the given color index
     * @param color Color byte
     */
    void Clear(char color){
        uint16_t c = palette->GetColor(color);
        for (int y = 0; y < HEIGHT; y++){
            for (int x = 0; x < WIDTH; x++){
            framebuffer[y*WIDTH+x] = c;
            }
        }
    }

    /**
     * Draw a sprite on screen
     * @param sprite Pointer to the sprite to be drawn
     * @param x Horizontal position on screen
     * @param y Vertical position on screen
     * @param scale Scaling of the sprite (default: 1)
     * @param flipX Horizontal flipping of the sprite (default: false)
     * @param flipY Vertical flipping of the sprite (default: false)
     */
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
    
    /**
     * Set the palette to the given palette pointer
     * @param palette Pointer to the palette you want to set
     */
    void SetPalette(Palette* _palette) {
      palette = _palette;
    }
};

#endif
#ifndef INPUT_H
#define INPUT_H
#include "KeyState.h"

class Input{
public:
  static KeyState GetButtons(){
    KeyState state;
    #ifdef DESKTOP
    SDL_Event event;
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

#endif
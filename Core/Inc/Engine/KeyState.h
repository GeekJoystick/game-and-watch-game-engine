#ifndef KEY_STATE_H
#define KEY_STATE_H

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

#endif
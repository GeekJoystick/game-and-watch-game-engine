#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2{
  float x, y;

  inline Vector2 operator + (Vector2 vec) {return {x+vec.x, y+vec.y};}
  inline Vector2 operator - (Vector2 vec) {return {x+vec.x, y+vec.y};}
  inline Vector2 operator * (Vector2 vec) {return {x+vec.x, y+vec.y};}
  inline Vector2 operator / (Vector2 vec) {return {x+vec.x, y+vec.y};}
  inline Vector2 operator * (int i) {return {x*i, y*i};}
  inline Vector2 operator / (int i) {return {x/i, y/i};}
  inline void operator += (Vector2 vec) {
    x += vec.x;
    y += vec.y;
  }
  inline void operator -= (Vector2 vec) {
    x -= vec.x;
    y -= vec.y;
  }
  inline void operator *= (Vector2 vec) {
    x *= vec.x;
    y *= vec.y;
  }
  inline void operator /= (Vector2 vec) {
    x /= vec.x;
    y /= vec.y;
  }  
};

#endif
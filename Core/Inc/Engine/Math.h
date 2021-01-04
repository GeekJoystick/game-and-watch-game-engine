#ifndef MATH_H
#define MATH_H
#include "Transform.h"

class Math {
public:
    static bool RectCollision(Transform o1, Transform o2) {
        return HorizontalCollision(o1, o2) && VerticalCollision(o1, o2);
    }

    template <class T, class U, class V>
    static bool ValueInRange(T x, U min, V max){
        return (x >= min && x < max);
    }

    static bool HorizontalCollision(Transform o1, Transform o2) {
        if (o1.size.x*o1.scale < o2.size.x*o2.scale){
          return (ValueInRange(o1.pos.x, o2.pos.x, o2.pos.x+o2.size.x*o2.scale) || 
          ValueInRange(o1.pos.x+o1.size.x*o1.scale, o2.pos.x, o2.pos.x+o2.size.x*o2.scale));
        }else{
          return (ValueInRange(o2.pos.x, o1.pos.x, o1.pos.x+o1.size.x*o1.scale) || 
          ValueInRange(o2.pos.x+o2.size.x*o2.scale, o1.pos.x, o1.pos.x+o1.size.x*o1.scale));
        }
    }

    static bool VerticalCollision(Transform o1, Transform o2) {
        if (o1.size.y*o1.scale < o2.size.y*o2.scale){
          return (ValueInRange(o1.pos.y, o2.pos.y, o2.pos.y+o2.size.y*o2.scale) || 
          ValueInRange(o1.pos.y+o1.size.y*o1.scale, o2.pos.y, o2.pos.y+o2.size.y*o2.scale));
        }else{
          return (ValueInRange(o2.pos.y, o1.pos.y, o1.pos.y+o1.size.y*o1.scale) || 
          ValueInRange(o2.pos.y+o2.size.y*o2.scale, o1.pos.y, o1.pos.y+o1.size.y*o1.scale));
        }
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

#endif
#ifndef MATH_H
#define MATH_H
#include "Transform.h"

class Math {
public:
    /**
     * Returns true if the two given rects are
     * in collision with each other
     * @param o1 First rect
     * @param o2 Second rect
     */
    static bool RectCollision(Transform o1, Transform o2) {
        return HorizontalCollision(o1, o2) && VerticalCollision(o1, o2);
    }

    /**
     * Returns true if x is within the range
     * between min and max (min included)
     * @param x Value we want to compare
     * @param min Minimum value to compare x with
     * @param max Maximum value to compare x with
     */
    template <class T, class U, class V>
    static bool ValueInRange(T x, U min, V max){
        return (x >= min && x < max);
    }

    /**
     * Returns true if two rects are colliding horizontally
     * @param o1 First rect
     * @param o2 Second rect
     */
    static bool HorizontalCollision(Transform o1, Transform o2) {
        if (o1.size.x*o1.scale < o2.size.x*o2.scale){
          return (ValueInRange(o1.pos.x, o2.pos.x, o2.pos.x+o2.size.x*o2.scale) || 
          ValueInRange(o1.pos.x+o1.size.x*o1.scale, o2.pos.x, o2.pos.x+o2.size.x*o2.scale));
        }else{
          return (ValueInRange(o2.pos.x, o1.pos.x, o1.pos.x+o1.size.x*o1.scale) || 
          ValueInRange(o2.pos.x+o2.size.x*o2.scale, o1.pos.x, o1.pos.x+o1.size.x*o1.scale));
        }
    }

    /**
     * Returns true if two rects are colliding vertically
     * @param o1 First rect
     * @param o2 Second rect
     */
    static bool VerticalCollision(Transform o1, Transform o2) {
        if (o1.size.y*o1.scale < o2.size.y*o2.scale){
          return (ValueInRange(o1.pos.y, o2.pos.y, o2.pos.y+o2.size.y*o2.scale) || 
          ValueInRange(o1.pos.y+o1.size.y*o1.scale, o2.pos.y, o2.pos.y+o2.size.y*o2.scale));
        }else{
          return (ValueInRange(o2.pos.y, o1.pos.y, o1.pos.y+o1.size.y*o1.scale) || 
          ValueInRange(o2.pos.y+o2.size.y*o2.scale, o1.pos.y, o1.pos.y+o1.size.y*o1.scale));
        }
    }

    /**
     * Clamp a number between a certain range
     * If number is out of range, loops back into range
     * @param num Number to clamp
     * @param min Minimum number of the range
     * @param max Maximum number of the range
     */
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
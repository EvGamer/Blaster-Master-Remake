#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"
#include "../utils/Rectangle.h"

class Entity {
  public:
    inline void setSpeedX(float speedX) {
      _speedX = speedX;
      _dirrection = _speedX > 0 ? 1 : -1;
    }
    inline void setSpeedY(float speedY) {
      _speedY = speedY;
    }
    inline Rect getRect() { return Rect(_x, _y, _width, _height); }
    inline float getX() { return _x; }
    inline float getY() { return _y; }
    inline float getLeft() { return getX(); }
    inline float getRight() { return _x + _width; }
    inline float getBottom() { return getY(); }
    inline float getTop() { return _y + _height; }
    inline char getDirrectionX() {
      return signbit(_speedX)
        ? -1
        : (_speedX == 0 ? 0 : 1);
    }
    inline float getSpeedX() { return _speedX; }
    virtual float getSpeedY() { return _speedY; }
    virtual void onTileCollision(Point correction) = 0;
    virtual void draw() = 0;

  protected:
    float _speedX = 0;
    float _speedY = 0;
    float _x = 0;
    float _y = 0;
    float _width = 0;
    float _height = 0;
    char _dirrection = 1;
    Texture2D _texture;
};


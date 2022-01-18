#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"
#include "../utils/Rectangle.h"
#include "../utils/Point.h"
#include "../graphics/TextureKeeper.h"

class Entity {
  public:
    inline Rect rect() { return Rect(_x, _y, _width, _height); }
    inline float x() { return _x; }
    inline float y() { return _y; }
    inline float left() { return x(); }
    inline float right() { return _x + _width; }
    inline float bottom() { return y(); }
    inline float top() { return _y + _height; }
    inline char dirrectionX() {
      return signbit(_speedX)
        ? -1
        : (_speedX == 0 ? 0 : 1);
    }
    inline float speedX() { return _speedX; }
    inline void speedX(float value) {
      _speedX = value;
      _dirrection = _speedX > 0 ? 1 : -1;
    }

    virtual float speedY() { return _speedY; }
    inline void speedY(float value) {
      _speedY = value;
    }
    virtual void onTileCollision(WorldVector correction) = 0;
    virtual void draw() = 0;

  protected:
    float _speedX = 0;
    float _speedY = 0;
    float _x = 0;
    float _y = 0;
    float _width = 0;
    float _height = 0;
    char _dirrection = 1;
    TextureKeeper _texture;
};


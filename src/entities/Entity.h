#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"

class Entity {
  public:
    virtual void updatePosition();
    inline void setSpeedX(float speedX) {
      _speedX = speedX;
      _dirrection = _speedX > 0 ? 1 : -1;
    }
    inline void setSpeedY(float speedY) {
      _speedY = speedY;
    }
    inline float getX() { return _x; }
    inline float getY() { return _y; }
    inline float getWest() { return getX(); }
    inline float getEast() { return _x + _width; }
    inline float getSouth() { return getY(); }
    inline float getNorth() { return _y + _height; }
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
    GLuint _textureId;
};


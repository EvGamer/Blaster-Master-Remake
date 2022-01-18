#pragma once
#include "TextureKeeper.h"
#include "../utils/Point.h"

class Sprite {
  public:
    Sprite() = default;
    Sprite(TextureKeeper texture, float scale = 1);
    Sprite(const char* filename, float scale = 1);
    Sprite(TextureKeeper texture, Rectangle source, float scale = 1);
    Sprite(const char* filename, Rectangle source, float scale = 1);
  protected:
    void _getCropLimits(Rectangle source);
    TextureKeeper _texture;
    float _top = 0;
    float _bottom = 1;
    float _left = 0;
    float _right = 1;
    WorldVector _sizeInWorld;
    constexpr static WorldVector _toSizeInWorld(float width, float height, float scale);
};

class WorldSprite: public Sprite {
  using Sprite::Sprite;
  public:
    void draw(WorldVector position);
};

class ScreenSprite: public Sprite {
  using Sprite::Sprite;
  public:
    void draw(ScreenVector position);
};

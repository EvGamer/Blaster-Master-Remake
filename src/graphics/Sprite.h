#pragma once
#include "TextureResource.h"
#include "../utils/Point.h"

class Sprite {
  public:
    Sprite() = default;
    Sprite(const TextureResource& texture, float scale = 1);
    Sprite(const char* filename, float scale = 1);
    Sprite(const TextureResource& texture, Rectangle source, float scale = 1);
    Sprite(const char* filename, Rectangle source, float scale = 1);
  protected:
    void _getCropLimits(Rectangle source);
    TextureResource _texture;
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

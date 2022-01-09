#pragma once
#include "TextureKeeper.h"
#include "../utils/Point.h"

class TextureFragment {
  public:
    TextureFragment() = default;
    TextureFragment(TextureKeeper texture, Rectangle source);
    TextureFragment(const char* filename, Rectangle source);
  protected:
    void _getTextureCoordFromImageCoord(Rectangle source);
    TextureKeeper _texture;
    float _top;
    float _bottom;
    float _left;
    float _right;
    WorldVector _sizeInWorld;
};

class WorldTextureFragment: public TextureFragment {
  using TextureFragment::TextureFragment;
  public:
    void draw(WorldVector position);
};

class ScreenTextureFragment: public TextureFragment {
  using TextureFragment::TextureFragment;
  public:
    void draw(ScreenVector position);
};

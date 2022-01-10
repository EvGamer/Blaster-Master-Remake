#pragma once
#include "../graphics/TextureKeeper.h"
#include "../graphics/TextureFragment.h"

class FillBar {
  public:
    FillBar(const char* filename, Rectangle backgroundTexFrame, Rectangle barTexFrame, ScreenVector _barShift);
    update(float fillRatio);
    draw(ScreenVector position);
  private:
    TextureKeeper _texture;
    ScreenTextureFragment _background;
    ScreenTextureFragment _bar;
    Rectangle _barTexFrame;
    ScreenVector _barShift;
    int16_t _cutoff = 0;
    float fillRatio = 1.0f;
}
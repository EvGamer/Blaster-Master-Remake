#pragma once
#include "../graphics/TextureKeeper.h"
#include "../graphics/TextureFragment.h"

class FillBar {
  public:
    FillBar() = default;
    FillBar(
      const char* filename,
      Rectangle backgroundTexFrame, 
      Rectangle barTexFrame, 
      ScreenVector _barShift,
      float scale = 1,
      Byte wholeSteps = 0
    );
    void update(float fillRatio);
    void draw(ScreenVector position);
  private:
    TextureKeeper _texture;
    ScreenTextureFragment _background;
    ScreenTextureFragment _bar;
    Rectangle _barTexFrame;
    ScreenVector _barShift;
    Byte _wholeSteps;
    int16_t _cutoff = 0;
    float _fillRatio = 1.0f;
    float _scale = 1;
};

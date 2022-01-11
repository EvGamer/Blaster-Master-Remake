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

namespace BrickFillBarConst {
  const Rectangle BACKGROUND_TEXTURE_RECTANGLE = { 12, 7, 9, 50 };
  const Rectangle FILLING_TEXTURE_RECTANGLE = { 45, 8, 6, 31 };
  const ScreenVector FILLING_SHIFT = { 2, 2 };
  const float TEXTURE_SCALE = 2;
  const uint8_t MEASUREMENT_UNITS = 8;
};

class BrickFillBar: public FillBar {
  public:
    BrickFillBar(const char* filename): FillBar(
      filename,
      BrickFillBarConst::BACKGROUND_TEXTURE_RECTANGLE,
      BrickFillBarConst::FILLING_TEXTURE_RECTANGLE,
      BrickFillBarConst::FILLING_SHIFT,
      BrickFillBarConst::TEXTURE_SCALE,
      BrickFillBarConst::MEASUREMENT_UNITS
    ) {}
};

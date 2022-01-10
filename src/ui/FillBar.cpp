#include "FillBar.h"

FillBar::FillBar(const char* filename, Rectangle backgroundTexFrame, Rectangle barTexFrame, ScreenVector barShift) {
  _texture = TextureKeeper(filename);
  _background = ScreenTextureFragment(_texture, backgroundTexFrame);
  _bar = ScreenTextureFragment(_texture, barTexFrame);
  _barTexFrame = barTexFrame;
  _barShift = barShift;
}

void FillBar::update(float fillRatio) {
  if (fillRatio == _fillRatio) return;
  if (fillRatio > 1) fillRatio = 1;

  _fillRatio = fillRatio;
  int16_t _cutoff = std::floor((1 - fillRatio) * _barTexFrame.height);

  _bar = ScreenTextureFragment(
    _texture,
    {
      _barTexFrame.x,
      _barTexFrame.y + _cutoff,
      _barTexFrame.width,
      _barTexFrame.height - _cutoff,
    }
  )
}

void FillBar::draw(ScreenVector position) {
  _background.draw(position);
  ScreenVector barPosition = position + _barShift;
  barPosition.y += _cutoff;
  _bar.draw(barPosition);
}
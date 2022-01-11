#include <math.h>
#include "FillBar.h"


FillBar::FillBar(
  const char* filename,
  Rectangle backgroundTexFrame, 
  Rectangle barTexFrame, 
  ScreenVector barShift, 
  float scale, 
  Byte wholeSteps
) {
  _texture = TextureKeeper(filename);
  _scale = scale;
  _background = ScreenSprite(_texture, backgroundTexFrame, _scale);
  _bar = ScreenSprite(_texture, barTexFrame, _scale);
  _barTexFrame = barTexFrame;
  _barShift = barShift;
  _wholeSteps = wholeSteps;
}

void FillBar::update(float fillRatio) {
  if (_wholeSteps > 0) {
    fillRatio = ceil(_wholeSteps * fillRatio) / _wholeSteps;
  }
  if (fillRatio == _fillRatio) return;
  if (fillRatio > 1) fillRatio = 1;

  _fillRatio = fillRatio;
  _cutoff = floor((1 - fillRatio) * _barTexFrame.height);

  _bar = ScreenSprite(
    _texture,
    {
      _barTexFrame.x,
      _barTexFrame.y + _cutoff,
      _barTexFrame.width,
      _barTexFrame.height - _cutoff,
    },
    _scale
  );
}

void FillBar::draw(ScreenVector position) {
  _background.draw(position);
  ScreenVector barPosition = position + _barShift;
  barPosition.y += _cutoff * _scale;
  _bar.draw(barPosition);
}
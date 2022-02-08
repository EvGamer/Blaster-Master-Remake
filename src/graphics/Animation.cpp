
#include "Animation.h"

inline void Animation::initialize(TextureResource iTexture, float iSizeX, float iSizeY,
                                  uint8_t iX0, uint8_t iY0, uint8_t iRows,
                                  uint8_t iColums, unsigned iDelay,
                                  PlayType iPlay) {
  _play = iPlay;
  _texture = iTexture;
  _timer = iDelay;
  _delay = iDelay;
  _x0 = iX0;
  _y0 = iY0;
  _x = 0;
  _y = 0;
  _sizeX = iSizeX;
  _height = iSizeY;
  _rows = iRows;
  _columns = iColums;
  if (_delay != 0) {
    _stop = false;
  } else {
    _stop = true;
  }
}


Animation::Animation() {};

Animation::Animation(
  TextureResource iTexture, float iSizeX, float iSizeY, uint8_t iX0,
  uint8_t iY0, uint8_t iRows, uint8_t iColums,
  uint16_t iDelay, PlayType iPlay
) {
  initialize(iTexture, iSizeX, iSizeY, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

Animation::Animation(
  TextureResource iTexture, float iSize,
  uint8_t iX0, uint8_t iY0, uint8_t iRows, uint8_t iColums, 
  unsigned iDelay, PlayType iPlay
) {
  initialize(iTexture, iSize, iSize, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

unsigned Animation ::draw(char dir, float x1, float y1, float x2, float y2) {
  float fx, fx1, fy, fy1;
  if (dir > 0) {
    fx = (_x + _x0) * _sizeX;
    fx1 = fx + _sizeX;
  } else {
    fx1 = (_x + _x0) * _sizeX, fx = fx1 + _sizeX;
  }
  fy = (_y + _y0) * _height;
  fy1 = fy + _height;
  drawSprite(_texture, x1, y1, x2, y2, fx, fy, fx1, fy1);

  if (!_stop) {
    if (_timer == 0) {
      _timer = _delay;
      _x++;
      _y++;
      _x = _x % _columns;
      _y = _y % _rows;
      if ((_play != LOOP) && (_x == 0) && (_y == 0)) {
        _stop = true;
      }
    } else {
      _timer--;
    };
  }
  return _x;
}

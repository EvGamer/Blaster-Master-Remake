#include "utils.h"
#pragma once

enum PlayType { ONCE = 0x00, LOOP = 0x01, REVERSE = 0x02 };

class Animation {
 private:
  PlayType _play;
  TextureResource _texture;
  unsigned _timer;
  unsigned _delay;
  unsigned char _x;
  unsigned char _y;
  unsigned char _rows;
  unsigned char _columns;
  unsigned char _x0;
  unsigned char _y0;
  float _sizeX;
  float _height;
  bool _stop;

 public:
  bool isValid = true;

  Animation();

  Animation(
    TextureResource iTexture, float iSizeX, float iSizeY,
    uint8_t iX0, uint8_t iY0,
    uint8_t iRows, uint8_t iColums, 
    uint16_t iDelay, PlayType iPlay
  );

  Animation(
    TextureResource iTexture, float iSize, uint8_t iX0, uint8_t iY0,
    uint8_t iRows, uint8_t iColums, unsigned iDelay, PlayType iPlay
  );

  inline void initialize(
    TextureResource iTexture, float iSizeX, float iSizeY,
    uint8_t iX0, uint8_t iY0, uint8_t iRows,
    uint8_t iColums, unsigned iDelay, PlayType iPlay
  );

  unsigned draw(char dir, float x1, float y1, float x2, float y2);
  inline void setCol(unsigned col) { _x = col; }
  inline void setRow(unsigned rou) { _y = rou; }
  inline void stop() { _stop = true; }
  inline void resume() { _stop = false; }
  inline const bool isStopped() const { return _stop; }
};

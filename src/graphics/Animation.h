#include "utils.h"
#pragma once

enum playType { ONCE = 0x00, LOOP = 0x01, REVERSE = 0x02 };

class Animation {
 private:
  playType m_play;
  GLuint _textureId;
  unsigned m_timer;
  unsigned m_delay;
  unsigned char _x;
  unsigned char _y;
  unsigned char m_rows;
  unsigned char m_colums;
  unsigned char m_x0;
  unsigned char m_y0;
  float m_sizeX;
  float _height;
  bool m_stop;

 public:
  bool isValid = true;

  Animation();

  Animation(GLuint iTexture, float iSizeX, float iSizeY, uint8_t iX0,
            uint8_t iY0, uint8_t iRows, uint8_t iColums, unsigned iDelay,
            playType iPlay);

  Animation(GLuint iTexture, float iSize, uint8_t iX0, uint8_t iY0,
            uint8_t iRows, uint8_t iColums, unsigned iDelay, playType iPlay);

  inline void initialize(GLuint iTexture, float iSizeX, float iSizeY,
                         uint8_t iX0, uint8_t iY0, uint8_t iRows,
                         uint8_t iColums, unsigned iDelay, playType iPlay);

  unsigned draw(char dir, float x1, float y1, float x2, float y2);
  inline void setCol(unsigned col) { _x = col; }
  inline void setRow(unsigned rou) { _y = rou; }
  inline void freeze() { m_stop = true; }
  inline void unfreeze() { m_stop = false; }
  inline bool isEnded() { return m_stop; }
};

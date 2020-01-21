
#include "Animation.h"

inline void Animation::initialize(GLuint iTexture, float iSizeX, float iSizeY,
                                  uint8_t iX0, uint8_t iY0, uint8_t iRows,
                                  uint8_t iColums, unsigned iDelay,
                                  playType iPlay) {
  m_play = iPlay;
  _textureId = iTexture;
  m_timer = iDelay;
  m_delay = iDelay;
  m_x0 = iX0;
  m_y0 = iY0;
  _x = 0;
  _y = 0;
  m_sizeX = iSizeX;
  _height = iSizeY;
  m_rows = iRows;
  m_colums = iColums;
  if (m_delay != 0) {
    m_stop = false;
  } else {
    m_stop = true;
  }
}


Animation::Animation() {};

Animation::Animation(GLuint iTexture, float iSizeX, float iSizeY, uint8_t iX0,
                     uint8_t iY0, uint8_t iRows, uint8_t iColums,
                     unsigned iDelay, playType iPlay) {
  initialize(iTexture, iSizeX, iSizeY, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

Animation::Animation(GLuint iTexture, float iSize, uint8_t iX0, uint8_t iY0,
                      uint8_t iRows, uint8_t iColums, unsigned iDelay,
                      playType iPlay) {
  initialize(iTexture, iSize, iSize, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

unsigned Animation ::draw(char dir, float x1, float y1, float x2, float y2) {
  float fx, fx1, fy, fy1;
  if (dir > 0) {
    fx = (_x + m_x0) * m_sizeX;
    fx1 = fx + m_sizeX;
  } else {
    fx1 = (_x + m_x0) * m_sizeX, fx = fx1 + m_sizeX;
  }
  fy = (_y + m_y0) * _height;
  fy1 = fy + _height;
  const float by = 0.003;
  float bx = by * dir;

  drawSprite(_textureId, x1, y1, x2, y2, fx, fy + by, fx1 - bx, fy1);

  if (!m_stop) {
    if (m_timer == 0) {
      m_timer = m_delay;
      _x++;
      _y++;
      _x = _x % m_colums;
      _y = _y % m_rows;
      if ((m_play != LOOP) && (_x == 0) && (_y == 0)) {
        m_stop = true;
      }
    } else {
      m_timer--;
    };
  }
  glColor3f(1, 1, 1);
  return _x;
}

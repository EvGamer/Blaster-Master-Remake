
#include "Animation.h"

inline void Animation::initialize(GLuint *iTexture, float iSizeX, float iSizeY,
                                  uint8_t iX0, uint8_t iY0, uint8_t iRows,
                                  uint8_t iColums, unsigned iDelay,
                                  playType iPlay) {
  m_play = iPlay;
  m_texture = iTexture;
  m_timer = iDelay;
  m_delay = iDelay;
  m_x0 = iX0;
  m_y0 = iY0;
  m_x = 0;
  m_y = 0;
  m_sizeX = iSizeX;
  m_sizeY = iSizeY;
  m_rows = iRows;
  m_colums = iColums;
  if (m_delay != 0) {
    m_stop = false;
  } else {
    m_stop = true;
  }
}

Animation::Animation(GLuint *iTexture, float iSizeX, float iSizeY, uint8_t iX0,
                     uint8_t iY0, uint8_t iRows, uint8_t iColums,
                     unsigned iDelay, playType iPlay) {
  initialize(iTexture, iSizeX, iSizeY, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

Animation ::Animation(GLuint *iTexture, float iSize, uint8_t iX0, uint8_t iY0,
                      uint8_t iRows, uint8_t iColums, unsigned iDelay,
                      playType iPlay) {
  initialize(iTexture, iSize, iSize, iX0, iY0, iRows, iColums, iDelay, iPlay);
}

unsigned Animation ::draw(char dir, float x1, float y1, float x2, float y2) {
  float fx, fx1, fy, fy1;
  if (dir > 0) {
    fx = (m_x + m_x0) * m_sizeX;
    fx1 = fx + m_sizeX;
  } else {
    fx1 = (m_x + m_x0) * m_sizeX, fx = fx1 + m_sizeX;
  }
  fy = (m_y + m_y0) * m_sizeY;
  fy1 = fy + m_sizeY;
  const float by = 0.003;
  float bx = by * dir;
  drawSprite(m_texture, x1, y1, x2, y2, fx, fy + by, fx1 - bx, fy1);
  if (!m_stop) {
    if (m_timer == 0) {
      m_timer = m_delay;
      m_x++;
      m_y++;
      m_x = m_x % m_colums;
      m_y = m_y % m_rows;
      if ((m_play != LOOP) && (m_x == 0) && (m_y == 0)) {
        m_stop = true;
      }
    } else {
      m_timer--;
    };
  }
  glColor3f(1, 1, 1);
  return m_x;
}

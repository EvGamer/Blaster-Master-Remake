#include "utils.h"
#include "../typeAliases.h"
#include "../stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../thirdParty/stb/stb_image.h"

void drawSprite(GLuint textureId, float x1, float y1, float x2, float y2,
                float tx1, float ty1, float tx2, float ty2) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glBegin(GL_QUADS);
  glTexCoord2f(tx1, ty2);
  glVertex2f(x1, y1);
  glTexCoord2f(tx1, ty1);
  glVertex2f(x1, y2);
  glTexCoord2f(tx2, ty1);
  glVertex2f(x2, y2);
  glTexCoord2f(tx2, ty2);
  glVertex2f(x2, y1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

GLuint loadTexture(const char* filename, GLint filtration) {
  GLuint texture;
  int sX;
  int sY;
  int ChN;
  unsigned char* img = stbi_load(filename, &sX, &sY, &ChN, 4);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtration);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtration);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sX, sY, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    img);
  return texture;
}

GLuint loadTexture(const char* filename) {
  return loadTexture(filename, GL_NEAREST);
}

GLuint loadTextureL(const char* filename) {
  return loadTexture(filename, GL_LINEAR);
}

GLuint loadTexture(String filename, GLint filtration) {
  return loadTexture(filename.c_str(), filtration);
}

GLuint loadTexture(String filename) {
  return loadTexture(filename.c_str());
}

GLuint loadTextureL(String filename) {
  return loadTexture(filename.c_str());
}

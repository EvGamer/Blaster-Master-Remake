#include "utils.h"
#include "../typeAliases.h"
#include "../stdafx.h"

void drawSprite(Texture2D texture, float x1, float y1, float x2, float y2,
                float tx1, float ty1, float tx2, float ty2) {
  const float tileWidth = tx2 - tx1;
  const float width = x2 - x1;
  Rectangle source{tx1, ty1, tileWidth, ty2 - ty1};
  Rectangle destination{x1, y1, width, y2 - y1};
  DrawTextureTiled(texture, source, destination, {0,0}, 0, width / tileWidth, WHITE)
}

Texture2D loadTexture(const char* filename, int filtration) {
  Texture2D texture = LoadTexture(filename);
  SetTextureFilter(texture, filtration);
  return LoadTexture(filename);
}

Texture2D loadTexture(const char* filename) {
  return LoadTexture(filename);
}

Texture2D loadTextureL(const char* filename) {
  return loadTexture(filename, GL_LINEAR);
}

Texture2D loadTexture(String filename, int filtration) {
  return loadTexture(filename.c_str(), filtration);
}

Texture2D loadTexture(String filename) {
  return loadTexture(filename.c_str());
}

Texture2D loadTextureL(String filename) {
  return loadTextureL(filename.c_str());
}

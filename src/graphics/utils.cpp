#include "utils.h"
#include "../typeAliases.h"
#include "../stdafx.h"

const unsigned COORD_UNIT = 32;

void drawSprite(Texture2D texture, float worldX0, float worldY0, float worldX, float worldY,
                float relativeCropX0, float relativeCropY0, float relativeCropX, float relativeCropY) {
  const float cropX0 = relativeCropX0 * texture.width;
  const float cropX = relativeCropX * texture.width;
  const float cropY0 = relativeCropY0 * texture.height;
  const float cropY = relativeCropY * texture.height;
  const float x0 = worldX0 * COORD_UNIT;
  const float x = worldX * COORD_UNIT;
  const float y0 = worldY0 * COORD_UNIT;
  const float y = worldY * COORD_UNIT;
  
  const float cropWidth = cropX - cropX0;
  const float cropHeight = cropY - cropY0;
  const float width = x - x0;
  const float height = y - y0;
  Rectangle source{cropWidth > 0 ? cropX0 : cropX, cropY0, cropWidth, cropHeight};
  Rectangle destination{x0, -y, width, height};
  DrawTexturePro(texture, source, destination, {0, 0}, 0, WHITE);
}

Texture2D loadTexture(const char* filename, int filtration) {
  Texture2D texture = LoadTexture(filename);
  SetTextureFilter(texture, filtration);
  return texture;
}

Texture2D loadTexture(const char* filename) {
  return LoadTexture(filename);
}

Texture2D loadTextureL(const char* filename) {
  return loadTexture(filename, TEXTURE_FILTER_BILINEAR);
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

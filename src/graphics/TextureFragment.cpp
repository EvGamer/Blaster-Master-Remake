#include "TextureFragment.h"

TextureFragment::TextureFragment(TextureKeeper texture, Rectangle source) {
  _texture = texture;
  _getTextureCoordFromImageCoord(source);
}

TextureFragment::TextureFragment(const char* filename, Rectangle source) {
  _texture = TextureKeeper(filename);
  _getTextureCoordFromImageCoord(source);
}

void TextureFragment::_getTextureCoordFromImageCoord(Rectangle source) {
  auto texture = _texture.getTexture();
  _top = source.y / texture.height;
  _bottom = (source.y + source.height) / texture.height;
  _left = source.x / texture.width;
  _right = (source.x + source.width) / texture.width;
}

void TextureFragment::draw(Vector2 position) {
  
}

void TextureFragment::draw(Rectangle position) {
  
}
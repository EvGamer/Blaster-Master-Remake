#include "TextureFragment.h"
#include "utils.h"
#include "../engine/constants.h"

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

  _sizeInWorld = WorldVector{ source.width, source.height } / COORD_UNIT;

}

void WorldTextureFragment::draw(WorldVector inWorldPosition) {
  drawSprite(
    _texture,
    inWorldPosition.x, inWorldPosition.y,
    inWorldPosition.x + _sizeInWorld.x, inWorldPosition.y + _sizeInWorld.y,
    _left, _top, _right, _bottom
  );
};

void ScreenTextureFragment::draw(ScreenVector position) {
  WorldVector inWorldPosition = position / COORD_UNIT;
  drawSprite(
    _texture,
    inWorldPosition.x, -inWorldPosition.y - _sizeInWorld.y,
    inWorldPosition.x + _sizeInWorld.x, -inWorldPosition.y,
    _left, _top, _right, _bottom
  );
}

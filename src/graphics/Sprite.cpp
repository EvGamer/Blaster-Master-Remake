#include <cassert>

#include "Sprite.h"
#include "utils.h"
#include "../engine/constants.h"

Sprite::Sprite(TextureKeeper texture, float scale):
  _texture(texture)
{
  const auto image = _texture.getTexture();
  _sizeInWorld = _toSizeInWorld(image.width, image.height, scale);
}

Sprite::Sprite(const char* filename, float scale):
  _texture(filename)
{
  const auto image = _texture.getTexture();
  _sizeInWorld = _toSizeInWorld(image.width, image.height, scale);
}


Sprite::Sprite(const char* filename, Rectangle source, float scale):
  _texture(filename),
  _sizeInWorld(_toSizeInWorld(source.width, source.height, scale))
{
  _getCropLimits(source);
}

Sprite::Sprite(TextureKeeper texture, Rectangle source, float scale): 
  _texture(texture),
  _sizeInWorld(_toSizeInWorld(source.width, source.height, scale))
{
  _getCropLimits(source);
}

void Sprite::_getCropLimits(Rectangle source) {
  auto texture = _texture.getTexture();
  assert(texture.height > 0 && texture.width > 0); // Can't be 0. Texture is not loaded or loaded incorrectly
  _top = source.y / texture.height;
  _bottom = (source.y + source.height) / texture.height;
  _left = source.x / texture.width;
  _right = (source.x + source.width) / texture.width;
}

void WorldSprite::draw(WorldVector inWorldPosition) {
  drawSprite(
    _texture,
    inWorldPosition.x, inWorldPosition.y,
    inWorldPosition.x + _sizeInWorld.x, inWorldPosition.y + _sizeInWorld.y,
    _left, _top, _right, _bottom
  );
};

void ScreenSprite::draw(ScreenVector position) {
  WorldVector inWorldPosition = position / COORD_UNIT;
  drawSprite(
    _texture,
    inWorldPosition.x, -inWorldPosition.y - _sizeInWorld.y,
    inWorldPosition.x + _sizeInWorld.x, -inWorldPosition.y,
    _left, _top, _right, _bottom
  );
}

constexpr WorldVector Sprite::_toSizeInWorld(float width, float height, float scale) {
  return WorldVector{width, height} / COORD_UNIT * scale;
};

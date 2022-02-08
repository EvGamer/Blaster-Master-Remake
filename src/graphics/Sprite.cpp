#include <cassert>

#include "Sprite.h"
#include "utils.h"
#include "../engine/constants.h"

Sprite::Sprite(const TextureResource& texture, float scale):
  _texture(texture)
{
  const auto image = _texture.texture();
  _sizeInWorld = _toSizeInWorld(image.width, image.height, scale);
}

Sprite::Sprite(const char* filename, float scale):
  _texture(filename)
{
  const auto image = _texture.texture();
  _sizeInWorld = _toSizeInWorld(image.width, image.height, scale);
}


Sprite::Sprite(const char* filename, Rectangle source, float scale):
  _texture(filename),
  _sizeInWorld(_toSizeInWorld(source.width, source.height, scale))
{
  _getCropLimits(source);
}

Sprite::Sprite(const TextureResource& texture, Rectangle source, float scale):
  _texture(texture),
  _sizeInWorld(_toSizeInWorld(source.width, source.height, scale))
{
  _getCropLimits(source);
}

void Sprite::_getCropLimits(Rectangle source) {
  auto& textureHeight = _texture.height();
  auto& textureWidth = _texture.width();
  assert(textureHeight > 0 && textureWidth > 0); // Can't be 0. Texture is not loaded or loaded incorrectly
  _top = source.y / textureHeight;
  _bottom = (source.y + source.height) / textureHeight;
  _left = source.x / textureWidth;
  _right = (source.x + source.width) / textureWidth;
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
  WorldVector inWorldPosition(position / COORD_UNIT);
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

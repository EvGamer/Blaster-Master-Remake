#include "./TextureResource.h"

TextureResource::TextureResource(): _sharedCounter{new UInt(1)} {}

TextureResource::TextureResource(const char* filename):
  _sharedCounter{new UInt(1)},
  _texture{LoadTexture(filename)},
  _filename{filename}
{}

TextureResource::TextureResource(String filename): TextureResource(filename.c_str()) {}

void TextureResource::_copy(const TextureResource& toCopy) {
  _texture = toCopy._texture;
  _filename = toCopy._filename;
  _sharedCounter = toCopy._sharedCounter;
  (*_sharedCounter)++;
  // TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Copy texture keeper. Counter value: %i", _texture.id, (*_sharedCounter));
}

TextureResource::TextureResource(const TextureResource& toCopy) {
  _copy(toCopy);
}

void TextureResource::operator=(const TextureResource& toCopy) {
  _copy(toCopy);
}

TextureResource::~TextureResource() {
  (*_sharedCounter)--;
  auto sharedCounter = (*_sharedCounter);
  auto textureId = _texture.id;
  if (sharedCounter != 0 || textureId <= 0) {
    // TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Decrement counter, Copy order: %i, Counter: %i, Filename: %s", textureId, _copyOrder, sharedCounter, _filename.c_str());
    return;
  }
  UnloadTexture(_texture);
  delete _sharedCounter;
}
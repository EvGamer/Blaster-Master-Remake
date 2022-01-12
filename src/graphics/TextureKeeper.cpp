#include "./TextureKeeper.h"

TextureKeeper::TextureKeeper(): _sharedCounter{new UInt(1)} {}

TextureKeeper::TextureKeeper(const char* filename): 
  _sharedCounter{new UInt(1)},
  _texture{LoadTexture(filename)},
  _filename{filename}
{}

TextureKeeper::TextureKeeper(String filename): TextureKeeper(filename.c_str()) {}

void TextureKeeper::_copy(const TextureKeeper& toCopy) {
  _texture = toCopy._texture;
  _filename = toCopy._filename;
  _sharedCounter = toCopy._sharedCounter;
  (*_sharedCounter)++;
  // TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Copy texture keeper. Counter value: %i", _texture.id, (*_sharedCounter));
}

TextureKeeper::TextureKeeper(const TextureKeeper& toCopy) {
  _copy(toCopy);
}

void TextureKeeper::operator=(const TextureKeeper& toCopy) {
  _copy(toCopy);
}

TextureKeeper::~TextureKeeper() {
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
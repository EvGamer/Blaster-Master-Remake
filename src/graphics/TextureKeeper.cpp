#include "./TextureKeeper.h"

TextureKeeper::TextureKeeper() {
  _sharedCounter = new UInt(1);
}

TextureKeeper::TextureKeeper(const char* filename) {
  _sharedCounter = new UInt(1);
  load(String(filename));
}

TextureKeeper::TextureKeeper(String filename) {
  _sharedCounter = new UInt(1);
  load(filename);
}

void TextureKeeper::load(String filename) {
  TraceLog(LOG_INFO, "TEXKEEP: Load texture");
  _filename = filename;
  _texture = LoadTexture(_filename.c_str());
}

void TextureKeeper::copy(const TextureKeeper& toCopy) {
  _texture = toCopy._texture;
  _filename = toCopy._filename;
  _sharedCounter = toCopy._sharedCounter;
  _copyOrder = toCopy._copyOrder + 1;
  (*_sharedCounter)++;
  // TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Copy texture keeper. Counter value: %i", _texture.id, (*_sharedCounter));
}

TextureKeeper::TextureKeeper(const TextureKeeper& toCopy) {
  copy(toCopy);
}

TextureKeeper& TextureKeeper::operator=(const TextureKeeper& toCopy) {
  copy(toCopy);
}

TextureKeeper::~TextureKeeper() {
  (*_sharedCounter)--;
  auto sharedCounter = (*_sharedCounter);
  auto textureId = _texture.id;
  if (sharedCounter != 0 || textureId <= 0) {
    // TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Decrement counter, Copy order: %i, Counter: %i, Filename: %s", textureId, _copyOrder, sharedCounter, _filename.c_str());
    return;
  }
  TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Delete keeper. Copy Order: %i, Counter: %i, Filename: %s", textureId, _copyOrder, sharedCounter, _filename.c_str());
  UnloadTexture(_texture);
  delete _sharedCounter;
}
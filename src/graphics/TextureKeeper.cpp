#include "./TextureKeeper.h"

TextureKeeper::TextureKeeper() {
  _sharedCounter = new std::atomic_uint_fast32_t(1);
}

TextureKeeper::TextureKeeper(char* filename): TextureKeeper() {
  TraceLog(LOG_INFO, "TEXKEEP: Load texture. Counter value %i", (*_sharedCounter).load());
  _texture = LoadTexture(filename);
}

TextureKeeper::TextureKeeper(String filename): TextureKeeper() {
  _texture = LoadTexture(filename.c_str());
}

TextureKeeper::TextureKeeper(const TextureKeeper& toCopy) {
  _texture = toCopy._texture;
  _sharedCounter = toCopy._sharedCounter;
  (*_sharedCounter)++;
  // TraceLog(LOG_INFO, "TEXKEEP: Copy texture keeper. Counter value: %i", (*_sharedCounter).load());
}

TextureKeeper TextureKeeper::operator=(const TextureKeeper& toCopy) {
  return TextureKeeper(toCopy);
}

TextureKeeper::~TextureKeeper() {
  (*_sharedCounter)--;
  if ((*_sharedCounter).load() != 0) return;
  TraceLog(LOG_INFO, "TEXKEEP: Delete keeper. Counter value: %i", (*_sharedCounter).load());
  UnloadTexture(_texture);
  delete _sharedCounter;
}
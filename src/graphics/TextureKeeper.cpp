#include "./TextureKeeper.h"

TextureKeeper::TextureKeeper() {
  _sharedCounter = new std::atomic_uint_fast32_t(1);
}

TextureKeeper::TextureKeeper(const char* filename) {
  _sharedCounter = new std::atomic_uint_fast32_t(1);
  load(String(filename));
}

TextureKeeper::TextureKeeper(String filename) {
  _sharedCounter = new std::atomic_uint_fast32_t(1);
  load(filename);
}

void TextureKeeper::load(String filename) {
  TraceLog(LOG_INFO, "TEXKEEP: Load texture");
  _filename = filename;
  _texture = LoadTexture(_filename.c_str());
}

TextureKeeper::TextureKeeper(const TextureKeeper& toCopy) {
  _texture = toCopy._texture;
  _filename = toCopy._filename;
  _sharedCounter = toCopy._sharedCounter;
  (*_sharedCounter)++;
  TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Copy texture keeper. Counter value: %i", _texture.id, (*_sharedCounter).load());
}

TextureKeeper TextureKeeper::operator=(const TextureKeeper& toCopy) {
  return TextureKeeper(toCopy);
}

TextureKeeper::~TextureKeeper() {
  (*_sharedCounter)--;
  if ((*_sharedCounter).load() != 0 || _texture.id <= 0) return;
  TraceLog(LOG_INFO, "TEXKEEP: [ID: %i] Delete keeper. Counter: %i, Filename: %s", _texture.id, (*_sharedCounter).load(), _filename.c_str());
  UnloadTexture(_texture);
  delete _sharedCounter;
}
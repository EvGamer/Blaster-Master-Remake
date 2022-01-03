#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"
#include <atomic>

// Class that safely loads and unloads texture upon destruction
class TextureKeeper {
  public:
  TextureKeeper();
  TextureKeeper(String filename);
  TextureKeeper(char* filename);
  TextureKeeper(const TextureKeeper& toCopy);
  ~TextureKeeper();
  TextureKeeper operator=(const TextureKeeper& toCopy);
  inline const Texture2D& getTexture() { return _texture; };

  private:
  std::atomic_uint_fast32_t* _sharedCounter;
  Texture2D _texture;
};

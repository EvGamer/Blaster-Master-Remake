#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"
#include <atomic>

// Class that safely loads and unloads texture upon destruction
class TextureKeeper {
  public:
  TextureKeeper();
  TextureKeeper(String filename);
  TextureKeeper(const char* filename);
  TextureKeeper(const TextureKeeper& toCopy);
  ~TextureKeeper();
  void operator=(const TextureKeeper& toCopy);
  inline const Texture2D& getTexture() { return _texture; };
  void load(String filename);
  void copy(const TextureKeeper& toCopy);

  private:
  UInt* _sharedCounter;
  UInt _copyOrder = 0;
  Texture2D _texture = {
    .id = 0,
  };
  String _filename;
};

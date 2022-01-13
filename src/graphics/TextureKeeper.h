#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"

// Class that safely loads and unloads texture upon destruction
class TextureKeeper {
  public:
    TextureKeeper();
    TextureKeeper(String filename);
    TextureKeeper(const char* filename);
    TextureKeeper(const TextureKeeper& toCopy);
    ~TextureKeeper();
    void operator=(const TextureKeeper& toCopy);
    inline const Texture2D& texture() { return _texture; };

  private:
    void _copy(const TextureKeeper& toCopy);
    uint16_t* _sharedCounter{nullptr};
    Texture2D _texture = { .id = 0 };
    String _filename;
};

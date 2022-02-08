#pragma once
#include "../stdafx.h"
#include "../typeAliases.h"

// Class that safely loads and unloads texture upon destruction
class TextureResource {
  public:
    TextureResource();
    TextureResource(String filename);
    TextureResource(const char* filename);
    TextureResource(const TextureResource& toCopy);
    ~TextureResource();
    void operator=(const TextureResource& toCopy);
    inline const Texture2D& texture() const { return _texture; };
    inline const unsigned int& id() const { return _texture.id; };

  private:
    void _copy(const TextureResource& toCopy);
    uint16_t* _sharedCounter{nullptr};
    Texture2D _texture = { .id = 0 };
    String _filename;
};

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
    inline const int& width() const { return _texture.width; };
    inline const int& height() const { return _texture.height; };

  private:
    void _copy(const TextureResource& toCopy);
    uint16_t* _sharedCounter{nullptr};
    Texture2D _texture = {
      .id = 0,
      .width = 0,
      .height = 0
    };
    String _filename;
};

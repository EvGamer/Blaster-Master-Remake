#include "TextureKeeper.h"

class TextureFragment {
  public:
    TextureFragment(TextureKeeper texture, Rectangle source);
    TextureFragment(const char* filename, Rectangle source);
    void draw(Vector2 position);
    void draw(Rectangle destination);
  private:
    void _getTextureCoordFromImageCoord(Rectangle source);
    TextureKeeper _texture;
    float _top;
    float _bottom;
    float _left;
    float _right;
};
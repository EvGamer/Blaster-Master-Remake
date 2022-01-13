#include "utils.h"
#include "../typeAliases.h"
#include "../stdafx.h"
#include "./TextureKeeper.h"
#include "rlgl.h"

namespace {
  const unsigned COORD_UNIT = 32;
  const uint8_t MAX_COLOR = 255;
}

void drawSprite(
  const TextureKeeper& textureKeeper, float worldX0, float worldY0, float worldX, float worldY,
  float relativeCropX0, float relativeCropY0, float relativeCropX, float relativeCropY
) {
  Texture2D texture = textureKeeper.texture();
  const float x0 = worldX0 * COORD_UNIT;
  const float x = worldX * COORD_UNIT;
  const float y = -worldY0 * COORD_UNIT;
  const float y0 = -worldY * COORD_UNIT;

  rlCheckRenderBatchLimit(4); // MAke sure there is enough free space for the batch buffer

  rlSetTexture(texture.id);
  rlBegin(RL_QUADS);
    rlColor4ub(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);
    rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

    rlTexCoord2f(relativeCropX0, relativeCropY0);
    rlVertex2f(x0, y0);
    
    rlTexCoord2f(relativeCropX0, relativeCropY);
    rlVertex2f(x0, y);

    rlTexCoord2f(relativeCropX, relativeCropY);
    rlVertex2f(x, y);

    rlTexCoord2f(relativeCropX, relativeCropY0);
    rlVertex2f(x, y0);
  rlEnd();
  rlSetTexture(0);
}

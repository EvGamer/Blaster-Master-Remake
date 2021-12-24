#include "../stdafx.h"
#pragma once

void drawSprite(Texture2D texture, float x1, float y1, float x2, float y2,
                float tx1, float ty1, float tx2, float ty2);

Texture2D loadTexture (const char* filename, int filtration);

Texture2D loadTexture (const char* filename);

Texture2D loadTexture (const char* filename);

Texture2D loadTexture(std::string filename, int filtration);

Texture2D loadTexture(std::string filename);

Texture2D loadTextureL(std::string filename);

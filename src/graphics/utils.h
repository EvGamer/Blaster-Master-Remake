#include "../stdafx.h"
#pragma once

void drawSprite(GLuint textureId, float x1, float y1, float x2, float y2,
                float tx1, float ty1, float tx2, float ty2);

GLuint loadTexture(std::string filename, GLint filtration);

GLuint loadTexture(std::string filename);

GLuint loadTextureL(std::string filename);

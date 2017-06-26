#include "stdafx.h"
#include "animation.h"

GLuint loadTexture(char* filename)
{
	GLuint texture;
	int sX;
	int sY;
	int ChN;
	unsigned char *img = stbi_load(filename,&sX,&sY,&ChN,4);
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sX,sY,0,GL_RGBA,GL_UNSIGNED_BYTE,img); 
	return texture;
}

GLuint loadTextureL(char* filename)
{
	GLuint texture;
	int sX;
	int sY;
	int ChN;
	unsigned char *img = stbi_load(filename,&sX,&sY,&ChN,4);
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sX,sY,0,GL_RGBA,GL_UNSIGNED_BYTE,img); 
	return texture;
}

struct shotType
{
	animation *burstAnim;
	animation *flyAnim;
	float spriteX;
	float spriteY;
	bool foe;
	float damage;
	GLuint *texture;
	bool falling;
};

class physics
{
	private:
		unsigned char num;//���������� ������
		float size;
		unsigned TiL;//������ � ������.
		struct 
		{
			bool solid;
			float friction;
			unsigned char texX;
			unsigned char texY;
		} tile[256];
		
		
		
		GLuint texture;
		static const unsigned mapSX=70;
		static const unsigned mapSY=70
		//Map, hardcoded in haste
		unsigned char map[70*70]={6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
6,23,24,6,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
23,24,23,24,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
6,23,24,6,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
23,24,23,24,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
6,23,24,6,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
23,24,23,24,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,6,23,24,6,167,168,118,119,120,168,167,167,167,168,118,119,6,23,
6,23,24,6,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
6,23,24,6,86,87,87,88,86,87,87,88,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,6,23,
23,24,23,24,104,152,151,151,151,152,102,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
6,23,24,6,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,6,23,24,6,167,168,118,119,120,168,167,167,167,168,118,119,6,23,
23,24,23,24,120,184,167,167,167,184,118,119,86,87,87,87,87,87,88,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,86,87,87,87,87,88,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
6,23,24,6,120,184,167,167,167,184,118,119,104,152,151,151,151,152,102,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,6,23,
23,24,23,24,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
6,23,24,6,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,6,23,24,6,167,168,118,119,120,168,167,167,167,168,118,119,6,23,
6,24,23,24,86,87,87,88,86,87,87,88,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,86,87,87,87,87,88,118,119,120,184,167,167,167,184,118,119,86,87,87,87,87,87,87,87,87,87,87,87,87,87,87,87,87,87,88,167,167,184,118,119,23,24,
23,23,24,6,104,152,151,151,151,152,102,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,104,152,151,151,151,152,102,103,104,152,151,151,151,152,102,103,104,152,151,167,167,184,118,119,6,23,
6,24,23,24,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,23,24,
23,23,24,6,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,148,149,148,149,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,6,23,
6,24,23,24,120,184,167,167,167,184,118,119,86,87,87,88,167,184,118,119,120,184,167,167,167,184,164,165,164,165,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,148,149,148,149,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,23,24,23,24,6,23,
6,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,164,165,164,165,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,86,87,88,6,23,24,6,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,148,149,148,149,148,149,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,164,165,164,165,164,165,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
6,23,24,6,6,23,24,6,86,87,87,88,86,87,87,88,86,87,87,88,86,87,87,88,86,87,87,88,86,87,87,88,86,87,87,88,120,184,167,167,167,23,24,23,24,23,24,23,24,86,87,87,88,86,87,87,88,184,118,119,120,184,167,167,23,24,23,24,6,23,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,152,102,103,104,152,151,151,151,184,118,119,120,184,167,167,6,23,24,6,23,24,
6,23,24,6,6,23,24,6,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,88,23,24,23,24,23,24,23,24,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,88,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,152,102,103,104,152,151,151,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,88,86,87,87,88,86,87,87,88,23,24,23,24,23,24,23,24,86,87,87,88,86,87,87,88,184,118,119,120,184,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,152,102,103,104,152,151,151,151,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,88,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,88,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,152,102,103,104,152,151,151,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,86,87,87,87,88,86,87,87,88,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,86,87,87,88,86,87,87,88,184,118,119,120,184,167,167,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,152,102,103,104,152,151,151,151,184,118,119,120,184,167,167,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,86,87,87,88,184,167,167,167,184,118,119,120,184,167,167,167,6,23,24,6,6,23,24,6,23,24,23,24,23,24,23,24,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,152,102,103,104,152,151,151,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,6,23,24,6,6,23,24,6,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,120,184,86,87,87,88,118,119,120,86,87,87,88,6,23,24,6,6,23,24,6,23,24,23,24,23,24,23,24,86,87,87,88,86,87,87,88,184,118,119,120,184,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,6,23,24,6,6,23,24,6,152,102,103,104,152,151,151,151,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,167,6,23,24,6,6,23,24,6,23,24,23,24,23,24,23,24,168,118,119,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,23,24,23,24,23,24,23,24,6,23,24,6,6,23,24,6,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,167,184,118,119,120,184,167,167,167,184,118,119,120,184,167,167,167,86,87,87,87,88,167,167,167,6,23,24,6,6,23,24,6,23,24,23,24,23,24,23,24,184,118,119,120,184,167,167,167,86,87,87,88,86,87,87,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,120,184,167,167,167,152,102,103,104,152,151,151,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,120,168,167,167,167,168,118,119,120,168,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,148,149,1,2,3,184,118,119,120,184,167,167,6,23,24,6,23,24,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,164,165,17,18,19,184,118,119,120,184,167,167,23,24,23,24,6,23,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,
23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,23,24,
6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23,24,6,6,23};
		
		struct shotNode
		{
			animation *burstAnim;
			animation *flyAnim;
			float spriteX;
			float spriteY;
			float x;
			float y;
			float speedX;
			float speedY;
			bool hit;
			bool foe;
			bool falling;
			float damage;
			shotNode *next;
			shotNode *prev;
		} *shotFirst=NULL;
		
		shotNode *shotLast=NULL;
		
	public:
		
		void addShot(float x,float y,float speedX,float speedY,shotType *wpn)
		{
			shotNode *buf;
			buf=new shotNode;
			//��������� ���������� � ��������
			buf->x=x;
			buf->y=y;
			buf->speedX=speedX;
			buf->speedY=speedY;
			buf->spriteX=wpn->spriteX;
			buf->spriteY=wpn->spriteY;
			buf->burstAnim = new animation(*wpn->burstAnim);
			buf->flyAnim= new animation(*wpn->flyAnim);
			buf->damage=wpn->damage;
			buf->foe=wpn->foe;
			buf->falling=wpn->falling;
			buf->hit=false;
			//���������� ����.
			if(shotFirst==NULL)
			{
				buf->next=NULL;
				buf->prev=NULL;
				shotFirst=buf;
				shotLast=buf;
			}
			else
			{
				buf->prev=shotLast;
				buf->next=NULL;
				shotLast->next=buf;
				shotLast=buf;
			}
		}
		
		void removeShot(shotNode *trash)
		{
			if(trash!=NULL)	
			{
				if ((trash==shotFirst)&&(trash==shotLast))
				{
					shotFirst=NULL;
				}
				else if (trash==shotFirst)
				{
					shotFirst=shotFirst->next;
					shotFirst->prev=NULL;
				}
				else if (trash==shotLast)
				{
					shotLast=shotLast->prev;
					shotLast->next=NULL;
				}
				else
				{
					(trash->prev)->next=trash->next;
					(trash->next)->prev=trash->prev;
				}
				delete trash->flyAnim;
				delete trash->burstAnim;
				delete trash;
			}
		}
		
		float hit(float x1,float y1,float x2,float y2,bool foe)
		{
			float damage=0;
			shotNode *cur=shotFirst;
			shotNode *trash=NULL;
			float bonus=0;
			while (cur!=NULL)
			{
				float cx=cur->x;
				float cy=cur->y;
				if ((cy>=y1)&&(cy<=y2)&&(cx>=x1)&&(cx<=x2)&&(foe==cur->foe)&&(!cur->hit))
				{
					if(cur->damage>damage)damage=cur->damage;
					cur->hit=true;
					cur->burstAnim->freeze();
					
					//removeShot(trash);	
				}
				cur=cur->next;
				
			}
			return damage;
		};
		
		bool collide(float x,float y)
		{
			unsigned i=floor(x);
			unsigned j=floor(y);
			return tile[map[(mapSY-1-j)*mapSX+i]-1].solid;
		}
		
		float friction;
		float gravity;
	
		
		bool getFrict(unsigned char i,unsigned char j)
		{
			return tile[map[(mapSY-1-j)*mapSX+i]-1].friction;
		}
		
		void update()
		{
			shotNode *cur;
			shotNode *trash;
			cur=shotFirst;
			while (cur!=NULL)
			{
				if (!cur->hit)
				{
					cur->x+=cur->speedX;
					cur->y+=cur->speedY;
					if (cur->falling) cur->speedY-=0.1*gravity;
					cur->hit=collide(cur->x,cur->y);
				}
				trash=cur;
				cur=cur->next;
				if (trash->burstAnim->is_end()){removeShot(trash);}
			}
		}
		
		void draw(int cx,int cy,unsigned char num)
		{
			
			float tx=tile[num].texX*size;
			float ty=tile[num].texY*size;
			float b=0.001;
			//cy=TiL-1-cy;
			drawSprite(&texture,cx,cy,cx+1,cy+1,tx+b,ty+b,tx+size-b,ty+size-b);
		};
		
		
		//void set_texture(char* fileName){texture=LoadTexture(fileName);};
			
		
		void set(unsigned char i,int tx,int ty,bool solid,float fric)
		{
			tile[i].solid=solid;
			tile[i].texX=tx;
			tile[i].texY=TiL-ty;
			tile[i].friction=fric;
		}
		
		void set(unsigned char i,int tx,int ty,bool solid)
		{
			tile[i].solid=solid;
			tile[i].texX=tx;
			tile[i].texY=ty;
			if (solid){tile[i].friction=friction;}
			else {tile[i].friction=0;};
		}
		
		void setDefault(unsigned first, unsigned last)
		{
			for(unsigned i=first-1;i<last;i++)
			{
				set(i,(i)%TiL,(i)/TiL,false);
			}
		}
		//
		void setSolid(unsigned first,unsigned last)
		{
			for(unsigned i=first;i<=last;i++)
			{
				tile[i-1].solid=true;
			}
		}
		void add(int tx,int ty,bool solid,float fric)
		{
			set(num,tx,ty,solid,fric);
			num++;
		}
		
		void add(int tx,int ty,bool solid)
		{
			set(num,tx,ty,solid);
			num++;
		}
		
		void drawLevel(float scrX,float scrY)
		{
			unsigned tX0=floor(scrX);
			unsigned tY0=floor(scrY);
			unsigned tXn=ceil(scrX+32);
			unsigned tYn=ceil(scrY+24);
			for(int i=tX0;i<=tXn;i++)
   			 {
    			for(int j=tY0;j<tYn;j++)
				{
					draw(i,j,map[(mapSY-1-j)*mapSX+i]-1);//
    			};
    		};
    		
    		char dir;
			float x0;
			float x1,x2;
			float y0;
			shotNode *cur;
			cur=shotFirst;
			while (cur!=NULL)
			{
				if (cur->speedX<0) 
				{
					dir=-1;
					x1=cur->x-cur->spriteX;
					x0=x1-2;
					x2=x0;
				}
				else
				{
					dir=1;
					x0=cur->x+cur->spriteX;
					x1=x0+2;
					x2=x1-1;
				}
				y0=cur->y;
				if (!cur->hit) cur->flyAnim->draw(dir,x0,y0,x1,y0+1);
				else           cur->burstAnim->draw(1,x2,y0,x2+1,y0+1);
				cur=cur->next;
			}
		}
		
		physics(char* fileName,float TilesInLine)
		{
			num=0;
			size=1/TilesInLine;
			TiL=TilesInLine;
			gravity=1;
			shotLast=NULL;
			shotFirst=NULL;
			friction=0;
			texture=loadTexture(fileName);
		
		}

};

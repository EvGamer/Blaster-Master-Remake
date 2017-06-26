
#include "animation.h"


inline void animation::initialize
(			
	GLuint *iTexture, 
	float iSizeX, 
	float iSizeY, 
	uint8_t iX0, 
	uint8_t iY0, 
	uint8_t iRows, 
	uint8_t iColums, 
	unsigned iDelay,
	playType iPlay
)
{
	play=iPlay;
	texture=iTexture;
	timer=iDelay;
	delay=iDelay;
	x0=iX0;
	y0=iY0;
	x=0;
	y=0;
	sizeX=iSizeX;
	sizeY=iSizeY;
	rows=iRows;
	colums=iColums;
	if (delay!=0) {stop=false;}
	else {stop=true;}
}


animation::animation
(			
	GLuint *iTexture, 
	float iSizeX, 
	float iSizeY, 
	uint8_t iX0, 
	uint8_t iY0, 
	uint8_t iRows, 
	uint8_t iColums, 
	unsigned iDelay,
	playType iPlay
)
{
	initialize(iTexture,iSizeX,iSizeY,iX0,iY0,iRows,iColums,iDelay,iPlay);
}

animation :: animation
(
	GLuint *iTexture, 
	float iSize, 
	uint8_t iX0, 
	uint8_t iY0, 
	uint8_t iRows, 
	uint8_t iColums, 
	unsigned iDelay,
	playType iPlay
)
{
	initialize(iTexture,iSize,iSize,iX0,iY0,iRows,iColums,iDelay,iPlay);
}

unsigned animation :: draw(char dir,float x1,float y1,float x2,float y2)
{
	float fx,fx1,fy,fy1;
	if (dir>0) {fx=(x+x0)*sizeX;fx1=fx+sizeX;}
	else {fx1=(x+x0)*sizeX,fx=fx1+sizeX;}
	fy=(y+y0)*sizeY;
	fy1=fy+sizeY;
	const float by=0.003;
	float bx=by*dir;
	drawSprite(texture,x1,y1,x2,y2,fx,fy+by,fx1-bx,fy1);
	if (!stop)
	{
		if (timer==0)
		{
			timer=delay;
			x++;y++;
			x=x%colums;
			y=y%rows;
			if((play!=LOOP)&&(x==0)&&(y==0)) {stop=true;}
		}
		else {timer--;};

	}
	glColor3f(1,1,1);
	return x;
				
}




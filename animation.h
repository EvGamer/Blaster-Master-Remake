#include "stdafx.h"
enum playType{ONCE=0x00,LOOP=0x01,REVERSE=0x02};

void drawSprite(GLuint *texture,float x1,float y1,float x2,float y2,float tx1,float ty1,float tx2,float ty2)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,*texture);
	glBegin(GL_QUADS);
		glTexCoord2f(tx1,ty2);	glVertex2f(x1,y1);
		glTexCoord2f(tx1,ty1);	glVertex2f(x1,y2);
		glTexCoord2f(tx2,ty1);	glVertex2f(x2,y2);
		glTexCoord2f(tx2,ty2);	glVertex2f(x2,y1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	
}

class animation
{
	private:
		playType play;
		GLuint *texture;
		unsigned timer;
		unsigned delay;
		unsigned char x;
		unsigned char y;
		unsigned char rows;
		unsigned char colums;
		unsigned char x0;
		unsigned char y0;
		float sizeX;
		float sizeY;
		bool stop;
	public:
		animation(
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
		
		animation(
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
			play=iPlay;
			texture=iTexture;
			delay=iDelay;
			timer=delay;
			x0=iX0;
			y0=iY0;
			x=0;
			y=0;
			sizeX=iSize;
			sizeY=iSize;
			rows=iRows;
			colums=iColums;
			if (delay!=0) {stop=false;}
			else {stop=true;}
		}
		
		unsigned draw(char dir,float x1,float y1, float x2, float y2)
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
		
		inline void setCol(unsigned col)
		{
			x=col;
		}
		
		inline void setRow(unsigned rou)
		{
			y=rou;
		}
		
		inline void freeze()
		{
			stop=true;
		}
		
		inline void unfreeze()
		{
			stop=false;
		}
		
		inline bool is_end()
		{
			return stop;
		}
		
};

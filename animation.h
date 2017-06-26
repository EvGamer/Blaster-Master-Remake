#include "imageOps.h"


enum playType{ONCE=0x00,LOOP=0x01,REVERSE=0x02};

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
		);
		
		
		animation(
			GLuint *iTexture, 
			float iSize, 
			uint8_t iX0, 
			uint8_t iY0, 
			uint8_t iRows, 
			uint8_t iColums, 
			unsigned iDelay,
			playType iPlay
		);
		
		inline void initialize
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
		);
		
		unsigned draw(char dir,float x1,float y1, float x2, float y2);
		inline void setCol(unsigned col){x=col;}
		inline void setRow(unsigned rou){y=rou;}
		inline void freeze(){stop=true;	}
		inline void unfreeze(){stop=false;}
		inline bool is_end(){return stop;}
		
};

#include "imageOps.h"


enum playType{ONCE=0x00,LOOP=0x01,REVERSE=0x02};

class animation
{
	private:
		playType m_play;
		GLuint *m_texture;
		unsigned m_timer;
		unsigned m_delay;
		unsigned char m_x;
		unsigned char m_y;
		unsigned char m_rows;
		unsigned char m_colums;
		unsigned char m_x0;
		unsigned char m_y0;
		float m_sizeX;
		float m_sizeY;
		bool m_stop;
		
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
		inline void setCol(unsigned col){m_x=col;}
		inline void setRow(unsigned rou){m_y=rou;}
		inline void freeze(){m_stop=true;	}
		inline void unfreeze(){m_stop=false;}
		inline bool is_end(){return m_stop;}
		
};


#include "animation.h"
#include "mapArray.h"


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
		unsigned char num;//количество тайлов
		float size;
		unsigned TiL;//тайлов в строке.
		float friction;
		float gravity;
		struct 
		{
			bool solid;
			float friction;
			unsigned char texX;
			unsigned char texY;
		} tile[256];
		
		GLuint texture;
		static const unsigned mapSX=MAP_LENGTH;
		static const unsigned mapSY=MAP_HEIGHT;
		//Map, hardcoded in haste
		unsigned char map[mapSX*mapSY];
		
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
		
		void addShot(float x,float y,float speedX,float speedY,shotType *wpn);
		void removeShot(shotNode *trash);
		float hit(float x1,float y1,float x2,float y2,bool foe);
		bool collide(float x,float y);
		inline float getGravity(){return gravity;};
		inline void setGravity(float a_gravity){ gravity= a_gravity; }
		bool getFrict(unsigned char i,unsigned char j);
		void update();
		void draw(int cx,int cy,unsigned char num);		
		void set(unsigned char i,int tx,int ty,bool solid,float fric);
		void set(unsigned char i,int tx,int ty,bool solid);
		inline void setGlobalFriction(float a_friction) { friction = a_friction; };
		void setDefault(unsigned first, unsigned last);
		void setSolid(unsigned first,unsigned last);
		void add(int tx,int ty,bool solid,float fric);	
		void add(int tx,int ty,bool solid);
		void drawLevel(float scrX,float scrY);
		physics(std::string fileName,float TilesInLine);

};

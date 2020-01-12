
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
	GLuint *m_texture;
	bool falling;
};

class physics
{
	private:
		unsigned char m_num;//���������� ������
		float m_size;
		unsigned m_lineLenght;//������ � ������.
		float m_friction;
		float m_gravity;
		struct 
		{
			bool solid;
			float friction;
			unsigned char texX;
			unsigned char texY;
		} m_tile[256];
		
		GLuint m_texture;
		static const unsigned _mapSX=MAP_LENGTH;
		static const unsigned _mapSY=MAP_HEIGHT;
		//Map, hardcoded in haste
		unsigned char m_map[_mapSX*_mapSY];
		
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
		inline float getGravity(){return m_gravity;};
		inline void setGravity(float a_gravity){ m_gravity = a_gravity; }
		bool getFrict(unsigned char i,unsigned char j);
		void update();
		void draw(int cx,int cy,unsigned char a_num);		
		void set(unsigned char i,int tx,int ty,bool solid,float fric);
		void set(unsigned char i,int tx,int ty,bool solid);
		inline void setGlobalFriction(float a_friction) { m_friction = a_friction; };
		void setDefault(unsigned first, unsigned last);
		void setSolid(unsigned first,unsigned last);
		void add(int tx,int ty,bool solid,float fric);	
		void add(int tx,int ty,bool solid);
		void drawLevel(float scrX,float scrY);
		physics(std::string fileName,float TilesInLine);

};

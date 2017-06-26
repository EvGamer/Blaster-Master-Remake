


#include "physics.h"


class player
{
	protected:
		GLuint *texture;
		float health=8;
		bool control;
		bool roll;
		bool ground=true;
		float startX;
		float startY;
		float x;
		float y;
		float sizeX;
		float sizeY;
		char dir;
		float jumpHeight =4;
		float jumpSpeed =0.08;
		float speedX;
		float speedY;
		float gunY;
		float gunX;
		float tada;
		int jumpPressed;
		int inv=0;
		float limit;
		int wpnHeat=0;
		unsigned curFrame;
		animation *standAnim;
		animation *walkAnim;
		animation *jumpAnim;
		animation *fallAnim;
		animation *curAnim;
		animation *deathAnim;
		shotType blaster;
	   	//unsigned char ca;
	   	//shotType blaster;
	   	physics *world;
	   	
	public:
	
		player(float x_in,float y_in,GLuint *textank,GLuint *texshoot,physics *world_link);
		
		void move(int dirrection);
		void jump();
		void hurt(float damage);
		inline float getFrontX(){return gunX;}
		inline float getX(){return x;}
		inline float getY(){return y;}
		void shoot();
		void continueFalling();
		void update();		
		inline bool is_dead(){deathAnim->is_end();}
		void revive();
		void drawGizmo();
		inline bool collide(float ox,float oy){	return (ox>x)&&(ox<x+sizeX)&&(oy>y)&&(oy<y+sizeX);}
		void draw();	
		inline float hull()	{if (health>0) return health; return 0;}
		
		~player()
		{
			delete standAnim;
			delete walkAnim;
			delete jumpAnim;
			delete fallAnim;
			delete blaster.burstAnim;
			delete blaster.flyAnim;
		}
		
};










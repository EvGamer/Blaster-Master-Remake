#include "player.h"

class enemy 
{
	private:
		float health;
		bool hit;
		float x;
		float y;
		float sizeX;
		float sizeY;
		float speedX=1;
		float speedY;
		char dir;
		int reload;
		int heat;
		char shotLeft=0;
		animation *deathAnim;
		animation *walkAnim;
		animation *shootAnim;
		animation *curAnim;
		GLuint *texture;
		physics *world;
		shotType grenade;
	public:
		enemy(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in);	
		inline bool is_dead(){	return deathAnim->is_end();	}
		inline void self_destruct();
		void update(player* p1);
		void draw();	
};

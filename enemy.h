#include "player.h"

class enemy 
{
	private:
		float m_health;
		bool m_hit;
		float m_x;
		float m_y;
		float m_sizeX;
		float m_sizeY;
		float m_speedX=1;
		float m_speedY;
		char m_dir;
		int m_reload;
		int m_heat;
		char m_shotLeft=0;
		animation *m_deathAnim;
		animation *m_walkAnim;
		animation *m_shootAnim;
		animation *m_curAnim;
		GLuint *m_texture;
		physics *m_world;
		shotType m_grenade;
	public:
		enemy(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in);	
		inline bool is_dead(){	return m_deathAnim->is_end();	}
		void self_destruct();
		void update(player* p1);
		void draw();	
};

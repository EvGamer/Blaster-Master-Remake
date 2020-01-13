#include "../entities/Enemy/Enemy.h"

class enemyList
{
	private:
		struct _node
		{
			Enemy *link;
			_node *prev;
			_node *next;
		};
		
		_node *m_first;
		_node *m_last;
	public:
		
		enemyList(float x_in,float y_in,char dirrection,GLuint *tex,World *world_in);
		enemyList();
		void add(float x_in,float y_in,char dirrection,GLuint *tex,World *world_in);
		void destroy(_node *trash);
		void update(Player *p1);
		void draw();
		void self_destruct();
		~enemyList();
};

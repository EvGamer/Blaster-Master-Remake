#include "enemy.h"

class enemyList
{
	private:
		struct node
		{
			enemy *link;
			node *prev;
			node *next;
		};
		
		node *first;
		node *last;
	public:
		
		enemyList(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in);
		enemyList();
		void add(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in);
		void destroy(node *trash);
		void update(player *p1);
		void draw();
		void self_destruct();
		~enemyList();
};

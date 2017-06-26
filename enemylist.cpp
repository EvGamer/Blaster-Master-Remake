#include "enemylist.h"

enemyList::enemyList(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
{
	node *buf=new node;
	buf->link=new enemy(x_in,y_in,dirrection,tex,world_in);
	buf->next=NULL;
	buf->prev=NULL;
	first=buf;
	last=buf;
}
		
enemyList::enemyList()
{
	first=NULL;
	last=NULL;
}
	
void enemyList::add(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
{
	node *buf = new node;
	buf->link=new enemy(x_in,y_in,dirrection,tex,world_in);
	if (last!=NULL)
	{
		buf->prev=last;
		buf->next=NULL;
		last->next=buf;
		last=buf;
	}
	else
	{
		buf->next=NULL;
		buf->prev=NULL;
		first=buf;
		last=buf;
	}
}
		
void enemyList::destroy(node *trash)
{
	if(trash!=NULL)	
	{
		if ((trash==first)&&(trash==last))
		{
			first=NULL;
			last=NULL;
		}
		else if (trash==first)
		{
			first=first->next;
			first->prev=NULL;
		}
		else if (trash==last)
		{
			last=last->prev;
			last->next=NULL;
		}
		else
		{
			(trash->prev)->next=trash->next;
			(trash->next)->prev=trash->prev;
		}
		delete trash->link;
		delete trash;
	}
}
	
void enemyList::update(player *p1)
{
	node *cur=first;
	node *trash=NULL;
	while (cur!=NULL)
	{
		cur->link->update(p1);
		trash=cur;
		cur=cur->next;
		if (trash!=NULL) if (trash->link->is_dead()) destroy(trash);
	}
}

void enemyList::draw()
{
	node *cur=first;
	while (cur!=NULL)
	{
		cur->link->draw();
		cur=cur->next;
	}
}
		
inline void enemyList::self_destruct()
{
	node *cur=first;
	while (cur!=NULL)
	{
		cur->link->self_destruct();
		cur=cur->next;
	}
}
		
	
enemyList::~enemyList()
{
	node *cur=first;
	node *trash;
	while (cur!=NULL)
	{
		trash=cur;
		cur=cur->next;
		destroy(trash);
	}
}

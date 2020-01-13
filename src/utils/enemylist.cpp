#include "enemylist.h"

enemyList::enemyList(float x_in,float y_in,char dirrection,GLuint *tex,World *world_in)
{
	_node *buf=new _node;
	buf->link=new Enemy(x_in,y_in,dirrection,tex,world_in);
	buf->next=NULL;
	buf->prev=NULL;
	m_first=buf;
	m_last=buf;
}
		
enemyList::enemyList()
{
	m_first=NULL;
	m_last=NULL;
}
	
void enemyList::add(float x_in,float y_in,char dirrection,GLuint *tex,World *world_in)
{
	_node *buf = new _node;
	buf->link=new Enemy(x_in,y_in,dirrection,tex,world_in);
	if (m_last!=NULL)
	{
		buf->prev=m_last;
		buf->next=NULL;
		m_last->next=buf;
		m_last=buf;
	}
	else
	{
		buf->next=NULL;
		buf->prev=NULL;
		m_first=buf;
		m_last=buf;
	}
}
		
void enemyList::destroy(_node *trash)
{
	if(trash!=NULL)	
	{
		if ((trash==m_first)&&(trash==m_last))
		{
			m_first=NULL;
			m_last=NULL;
		}
		else if (trash==m_first)
		{
			m_first=m_first->next;
			m_first->prev=NULL;
		}
		else if (trash==m_last)
		{
			m_last=m_last->prev;
			m_last->next=NULL;
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
	
void enemyList::update(Player *p1)
{
	_node *cur=m_first;
	_node *trash=NULL;
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
	_node *cur=m_first;
	while (cur!=NULL)
	{
		cur->link->draw();
		cur=cur->next;
	}
}
		
void enemyList::self_destruct()
{
	_node *cur=m_first;
	while (cur!=NULL)
	{
		cur->link->self_destruct();
		cur=cur->next;
	}
}
		
	
enemyList::~enemyList()
{
	_node *cur=m_first;
	_node *trash;
	while (cur!=NULL)
	{
		trash=cur;
		cur=cur->next;
		destroy(trash);
	}
}

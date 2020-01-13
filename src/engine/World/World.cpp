
#include "World.h"

World::World(std::string fileName,float TilesInLine)
{
	//dont ask.
	for(int i=0;i<=_mapSY;i++)
	{
		for(int j=0;j<_mapSY;j++)
		{
			int c=(_mapSY-1-j) * _mapSX+i;
			m_map[c] = MAP_ARRAY[c];//
 		};
 	};
	

	m_num=0;
	m_size=1/TilesInLine;
	m_lineLenght=TilesInLine;
	m_gravity=1;
	shotLast=NULL;
	shotFirst=NULL;
	m_friction=0;
	m_texture=loadTexture(fileName);
	
}


void World::addShot(float x,float y,float speedX,float speedY,shotType *wpn)
{
	shotNode *buf;
	buf=new shotNode;

	buf->x=x;
	buf->y=y;
	buf->speedX=speedX;
	buf->speedY=speedY;
	buf->spriteX=wpn->spriteX;
	buf->spriteY=wpn->spriteY;
	buf->burstAnim = new Animation(*wpn->burstAnim);
	buf->flyAnim= new Animation(*wpn->flyAnim);
	buf->damage=wpn->damage;
	buf->foe=wpn->foe;
	buf->falling=wpn->falling;
	buf->hit=false;

	if(shotFirst==NULL)
	{
		buf->next=NULL;
		buf->prev=NULL;
		shotFirst=buf;
		shotLast=buf;
	}
	else
	{
		buf->prev=shotLast;
		buf->next=NULL;
		shotLast->next=buf;
		shotLast=buf;
	}
};

void World::removeShot(shotNode *trash)
{
	if(trash!=NULL)	
	{
		if ((trash==shotFirst)&&(trash==shotLast))
		{
			shotFirst=NULL;
		}
		else if (trash==shotFirst)
		{
			shotFirst=shotFirst->next;
			shotFirst->prev=NULL;
		}
		else if (trash==shotLast)
		{
			shotLast=shotLast->prev;
			shotLast->next=NULL;
		}
		else
		{
			(trash->prev)->next=trash->next;
			(trash->next)->prev=trash->prev;
		}
		delete trash->flyAnim;
		delete trash->burstAnim;
		delete trash;
	}
};

float World::hit(float x1,float y1,float x2,float y2,bool foe)
{
	float damage=0;
	shotNode *cur=shotFirst;
	shotNode *trash=NULL;
	float bonus=0;
	while (cur!=NULL)
	{
		float cx=cur->x;
		float cy=cur->y;
		if ((cy>=y1)&&(cy<=y2)&&(cx>=x1)&&(cx<=x2)&&(foe==cur->foe)&&(!cur->hit))
		{
			if(cur->damage>damage)damage=cur->damage;
			cur->hit=true;
			cur->burstAnim->freeze();
			//removeShot(trash);	
		}
		cur=cur->next;	
	}
	return damage;
};

bool World::collide(float x,float y)
{
	unsigned i=floor(x);
	unsigned j=floor(y);
	return m_tile[m_map[(_mapSY-1-j)*_mapSX+i]-1].solid;
};

bool World::getFrict(unsigned char i,unsigned char j)
{
	return m_tile[m_map[(_mapSY-1-j)*_mapSX+i]-1].friction;
}

void World::update()
{
	shotNode *cur;
	shotNode *trash;
	cur=shotFirst;
	while (cur!=NULL)
	{
		if (!cur->hit)
		{
			cur->x+=cur->speedX;
			cur->y+=cur->speedY;
			if (cur->falling) cur->speedY-=0.1*m_gravity;
			cur->hit=collide(cur->x,cur->y);
		}
		trash=cur;
		cur=cur->next;
		if (trash->burstAnim->is_end()){removeShot(trash);}
	}
}

void World::draw(int cx,int cy,unsigned char a_num)
{
	
	float tx=m_tile[a_num].texX*m_size;
	float ty=m_tile[a_num].texY*m_size;
	float b=0.001;
	//cy=m_lineLenght-1-cy;
	drawSprite(&m_texture,cx,cy,cx+1,cy+1,tx+b,ty+b,tx+m_size-b,ty+m_size-b);
};

void World::set(unsigned char i,int tx,int ty,bool solid,float fric)
{
	m_tile[i].solid=solid;
	m_tile[i].texX=tx;
	m_tile[i].texY=m_lineLenght-ty;
	m_tile[i].friction=fric;
}


void World::set(unsigned char i,int tx,int ty,bool solid)
{
	m_tile[i].solid=solid;
			m_tile[i].texX=tx;
			m_tile[i].texY=ty;
			if (solid){m_tile[i].friction=m_friction;}
			else {m_tile[i].friction=0;};
	
	///float tileFriction = 0;
	//if (solid) tileFriction = friction;
	//set(i,tx,ty,solid,tileFriction);
}

void World::setDefault(unsigned first, unsigned last)
{
	for(unsigned i=first-1;i<last;i++) set(i,(i)%m_lineLenght,(i)/m_lineLenght,false);
}

void World::setSolid(unsigned first,unsigned last)
{
	for(unsigned i=first;i<=last;i++) m_tile[i-1].solid=true;
}

void World::add(int tx,int ty,bool solid,float fric)
{
	set(m_num,tx,ty,solid,fric);
	m_num++;
}

void World::add(int tx,int ty,bool solid)
{
	set(m_num,tx,ty,solid);
	m_num++;
}

void World::drawLevel(float scrX,float scrY)
{
	unsigned tX0=floor(scrX);
	unsigned tY0=floor(scrY);
	unsigned tXn=ceil(scrX+32);
	unsigned tYn=ceil(scrY+24);
	for(int i=tX0;i<=tXn;i++)
	{
		for(int j=tY0;j<tYn;j++)
		{
			draw(i,j,m_map[(_mapSY-1-j)*_mapSX+i]-1);//
 		};
 	};
	char dir;
	float x0;
	float x1,x2;
	float y0;
	shotNode *cur;
	cur=shotFirst;
	while (cur!=NULL)
	{
		if (cur->speedX<0) 
		{
			dir=-1;
			x1=cur->x-cur->spriteX;
			x0=x1-2;
			x2=x0;
		}
		else
		{
			dir=1;
			x0=cur->x+cur->spriteX;
			x1=x0+2;
			x2=x1-1;
		}
		y0=cur->y;
		if (!cur->hit) cur->flyAnim->draw(dir,x0,y0,x1,y0+1);
		else           cur->burstAnim->draw(1,x2,y0,x2+1,y0+1);
		cur=cur->next;
	}
}

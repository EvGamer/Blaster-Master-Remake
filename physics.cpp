
#include "physics.h"

physics::physics(std::string fileName,float TilesInLine)
{
	//dont ask.
	for(int i=0;i<=mapSY;i++)
	{
		for(int j=0;j<mapSY;j++)
		{
			int c=(mapSY-1-j)*mapSX+i;
			map[c] = MAP_ARRAY[c];//
 		};
 	};
	

	num=0;
	size=1/TilesInLine;
	TiL=TilesInLine;
	gravity=1;
	shotLast=NULL;
	shotFirst=NULL;
	friction=0;
	texture=loadTexture(fileName);
	
}


void physics::addShot(float x,float y,float speedX,float speedY,shotType *wpn)
{
	shotNode *buf;
	buf=new shotNode;
	//Заполняем информацию о выстреле
	buf->x=x;
	buf->y=y;
	buf->speedX=speedX;
	buf->speedY=speedY;
	buf->spriteX=wpn->spriteX;
	buf->spriteY=wpn->spriteY;
	buf->burstAnim = new animation(*wpn->burstAnim);
	buf->flyAnim= new animation(*wpn->flyAnim);
	buf->damage=wpn->damage;
	buf->foe=wpn->foe;
	buf->falling=wpn->falling;
	buf->hit=false;
	//Встраиваем узел.
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

void physics::removeShot(shotNode *trash)
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

float physics::hit(float x1,float y1,float x2,float y2,bool foe)
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

bool physics::collide(float x,float y)
{
	unsigned i=floor(x);
	unsigned j=floor(y);
	return tile[map[(mapSY-1-j)*mapSX+i]-1].solid;
};

bool physics::getFrict(unsigned char i,unsigned char j)
{
	return tile[map[(mapSY-1-j)*mapSX+i]-1].friction;
}

void physics::update()
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
			if (cur->falling) cur->speedY-=0.1*gravity;
			cur->hit=collide(cur->x,cur->y);
		}
		trash=cur;
		cur=cur->next;
		if (trash->burstAnim->is_end()){removeShot(trash);}
	}
}

void physics::draw(int cx,int cy,unsigned char num)
{
	
	float tx=tile[num].texX*size;
	float ty=tile[num].texY*size;
	float b=0.001;
	//cy=TiL-1-cy;
	drawSprite(&texture,cx,cy,cx+1,cy+1,tx+b,ty+b,tx+size-b,ty+size-b);
};

void physics::set(unsigned char i,int tx,int ty,bool solid,float fric)
{
	tile[i].solid=solid;
	tile[i].texX=tx;
	tile[i].texY=TiL-ty;
	tile[i].friction=fric;
}


void physics::set(unsigned char i,int tx,int ty,bool solid)
{
	tile[i].solid=solid;
			tile[i].texX=tx;
			tile[i].texY=ty;
			if (solid){tile[i].friction=friction;}
			else {tile[i].friction=0;};
	
	///float tileFriction = 0;
	//if (solid) tileFriction = friction;
	//set(i,tx,ty,solid,tileFriction);
}

void physics::setDefault(unsigned first, unsigned last)
{
	for(unsigned i=first-1;i<last;i++) set(i,(i)%TiL,(i)/TiL,false);
}

void physics::setSolid(unsigned first,unsigned last)
{
	for(unsigned i=first;i<=last;i++) tile[i-1].solid=true;
}

void physics::add(int tx,int ty,bool solid,float fric)
{
	set(num,tx,ty,solid,fric);
	num++;
}

void physics::add(int tx,int ty,bool solid)
{
	set(num,tx,ty,solid);
	num++;
}

void physics::drawLevel(float scrX,float scrY)
{
	unsigned tX0=floor(scrX);
	unsigned tY0=floor(scrY);
	unsigned tXn=ceil(scrX+32);
	unsigned tYn=ceil(scrY+24);
	for(int i=tX0;i<=tXn;i++)
	{
		for(int j=tY0;j<tYn;j++)
		{
			draw(i,j,map[(mapSY-1-j)*mapSX+i]-1);//
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

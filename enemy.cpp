
#include "enemy.h"

enemy::enemy(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
{
	texture=tex;
	dir=dirrection;
	health=6;
	x=x_in;
	y=y_in;
	sizeX=1.6;
	sizeY=1.1;
	world=world_in;
	deathAnim =new animation(texture,0.25,0,2,1,4,2,ONCE);
	walkAnim  =new animation(texture,0.25,0,0,1,4,5,LOOP);
	curAnim = walkAnim;
	shootAnim  =new animation(texture,0.25,0,1,1,4,3,ONCE);
	grenade.burstAnim = new animation(texture,0.125,0,7,1,4,2,ONCE);
	grenade.flyAnim = new animation(texture,0.25,0.125,0,6,1,1,2,ONCE);
	grenade.damage=1;
	grenade.falling=true;
	grenade.spriteX=-0.75;
	grenade.spriteY=-0.5;
	grenade.foe=true;
}	
	
		//
inline void enemy::self_destruct()
{
	if (curAnim!=deathAnim)
	{
		health=0;
		curAnim=deathAnim;
		speedX=0;
		y-=0.5;
		dir=-1;
	}
}

void enemy::update(player* p1)
{
	if (health>0){
		if (speedY<=0)
		{
			if (world->collide(x+0.05,y)||world->collide(x+sizeX-0.05,y))y=floor(y+1);
	
		}
		if(reload>0)reload--;
		if (shotLeft>0)
		{
			speedX=0;
			curAnim=shootAnim;
			if(heat<=0)
			{
			 	heat=10;
			 	world->addShot(x+sizeX*0.5,y+sizeY-0.4,0.4*dir,0.4f,&grenade);
			 	shotLeft--;
			}
			else heat--;
				
		}
		else
		{
			speedX=1;
			curAnim=walkAnim;
		}
		x+=0.05*speedX*dir;
		float hurt=world->hit(x,y,x+sizeX,y+sizeY,false);
		if(hurt>0) hit=true;
		health-=hurt;
		if (((dir>0)&&(world->collide(x+sizeX,y)||!world->collide(x+sizeX,y-1) ))
			||((dir<0)&&(world->collide(x,y)||!world->collide(x,y-1) ))) 
			dir=-dir;
		if (health<=0) self_destruct();
		float y1=y+sizeY*0.5;
		
		if (p1->collide(x+0.1,y1)||p1->collide(x+sizeX-0.1,y1)) p1->hurt(0.7f);
		if ((floor(p1->getY())==floor(y))&&(floor(p1->getFrontX()-x)==3*dir)&&(reload<=0)) 
		{
			reload=60;
			shotLeft=4;
		}
	}
}
		
		
void enemy::draw()
{
	float x1,x2;
	if (dir>0)
	{
		x2=x+sizeX;
		x1=x2-2;
	}
	else
	{
		x1=x;
		x2=x1+2;
	}
	if (hit) glColor3f(1,0.5,0);
	curAnim->draw(-dir,x1,y,x2,y+2);
	hit=false;
}


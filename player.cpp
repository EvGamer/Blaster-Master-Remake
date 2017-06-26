#include "player.h"

void player::move(int dirrection)
{
	if(control)
	{
		dir=dirrection;
		if (speedX<1)speedX+=0.1;
		if (curAnim==standAnim)curAnim=walkAnim;
		curAnim->unfreeze();
	}
}

void player::jump()
{
	if(control)
	{
		jumpPressed--;
		if (ground)
		{
			speedY=jumpHeight;
			jumpPressed=20;
			ground=false;
			limit=y+3;
			curAnim=(jumpAnim);
			gunY=0.6;
		}
		else 
		{
			jumpPressed--;
		}
	}
}

void player::hurt(float damage)
{
	if (inv<=0)
	{
		inv=45;
		speedX*=0.5;
		speedY+=1;
		health-=damage;
		damage=0;
	}	
}

void player::shoot()
{//if(control)
	if (wpnHeat<=0)
	{
		world->addShot(gunX,y+gunY,0.3*dir,0,&blaster);
		wpnHeat=15;
	}
}

void player::continueFalling()
{
	if (world->collide(x+0.05,y)||world->collide(x+sizeX-0.05,y))
	{
		y=floor(y+1);
		speedY=0;
		ground=true;
		curAnim=standAnim;
		gunY=0.4;
	}
	else {ground=false; curAnim=fallAnim;}	
}

void player::update()
{
	if (curAnim!=deathAnim)
	{	
		world->hit(x,y,x+sizeX,y+sizeY,true);
		if (inv>0)inv--;
		if (wpnHeat>0) wpnHeat--;
		y+=jumpSpeed*speedY;
		
		if ((y>limit)&&jumpPressed>0){speedY=0;jumpPressed=0;}
		if (speedY<=0) continueFalling();
		
		x+=dir*speedX*0.11;
		//if (!(world->collide(x,y-1)&&world->collide(x+sizeX,y-1))){ground=false;}
		speedY-=jumpSpeed*jumpHeight*0.5/world->getGravity();
		speedX-=0.05;
		
		if (speedX<0){speedX=0;};
		if (dir>0)
		{
			if (world->collide(x+sizeX,y)) {x=floor(x+2)-sizeX;speedX=0;}
			gunX=x+sizeX;
		}
		else
		{
			if (world->collide(x,y)) {x=floor(x+1);speedX=0;}
			gunX=x;
		}
			
		if (speedY>0)
		{
			if ((world->collide(x,y+sizeY))||(world->collide(x+sizeX-0.1f,y+sizeY)))//
			{y=floor(y+sizeY)-sizeY;speedY=0;};
		};
		
		if ((speedX>0.3)&&ground) {	curAnim=walkAnim; curAnim->unfreeze();}
		
		tada = world->hit(x-0.5,y,x+sizeX+0.5,y+sizeY+0.5,true);
		if(tada!=0) hurt(tada);
		if(health<=0)
		{
			if(ground)
			{
				curAnim=deathAnim;
				control=false;
				speedX=0;
			}
			else{inv=2;}
		}
	}
}	

void player::revive()
{
	deathAnim->unfreeze();
	curAnim=standAnim;
	health=8;
	x=startX;
	y=startY;
	control=true;
}

void player::drawGizmo()//debug option. Displays collision box.
{
	glColor3f(0,1,1);
	glBegin(GL_QUADS);
		glVertex2f(floor(x),floor(y));
		glVertex2f(floor(x),ceil(y+sizeY));
		glVertex2f(ceil(x+sizeX),ceil(y+sizeY));
		glVertex2f(ceil(x+sizeX),floor(y));
	glEnd();
}

void player::draw()
{
	float x1,x2;
	if(dir<0) {x1=x;	   x2=x+2;}
	else 	  {x2=x+sizeX; x1=x2-2;};
	if (curAnim!=deathAnim)
	{
		
		switch(inv%3)
		{
			case 0:
				glColor3f(1,1,1);
			break;
			case 1:
				glColor3f(0.5,1,0.7);
			break;
			case 2:
				glColor3f(1,0.3,0);
			break;
		}
	
		curAnim->setCol(curFrame);//если текущую анимацию заменили, она будет начинаться с того кадра, на котором закончила другая
		curFrame=curAnim->draw(-dir,x1,y,x2,y+2);
		curAnim->freeze();		
	}
	else curAnim->draw(-1,x1-0.5,y,x1+3.5,y+4);
}	

player::player(float x_in,float y_in,GLuint *textank,GLuint *texshoot,physics *world_link)
{
	world=world_link;
	x=x_in;
	y=y_in;
	startX=x;
	startY=y;
	dir=1;
	speedX=0;
	speedY=0;
	sizeX=26.0/16.0;
	sizeY=18.0/16.0;
	control=true;
	//настройка анимации
	texture=textank;
//	texture= new GLuint;
//	*texture=loadTexture("Sprites\\SOPHIA.tga");
	standAnim = new animation(texture,0.125,0,0,1,4,1,LOOP);
	walkAnim = new animation (texture,0.125,0,0,3,4,3,LOOP);
	jumpAnim = new animation (texture,0.125,0,3,1,4,2,LOOP);
	fallAnim = new animation (texture,0.125,0,2,1,4,2,LOOP);
	deathAnim = new animation (texture,0.25,0,2,1,3,3,ONCE);
	//настройка оружия
	blaster.texture = texshoot; 
	blaster.burstAnim = new animation(blaster.texture,0.25f,3,0,4,1,2,ONCE);
	blaster.flyAnim = new animation(blaster.texture,0.5f,0.25f,0,0,1,1,0,LOOP);
	blaster.damage = 1.5;
	blaster.foe = false;
	blaster.spriteX=-1.5;
	blaster.spriteY=-0.4;
	blaster.falling=false;
}

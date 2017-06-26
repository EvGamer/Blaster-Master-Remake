
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb/stb_image.h"
#include "stdafx.h"
#include "tile.h"

class enemy;

class player
{
	protected:
		GLuint *texture;
		float health=8;
		bool control;
		bool roll;
		bool ground=true;
		float startX;
		float startY;
		float x;
		float y;
		float sizeX;
		float sizeY;
		char dir;
		float jumpHeight =4;
		float jumpSpeed =0.08;
		float speedX;
		float speedY;
		float gunY;
		float gunX;
		float tada;
		int jumpPressed;
		int inv=0;
		float limit;
		int wpnHeat=0;
		unsigned curFrame;
		animation *standAnim;
		animation *walkAnim;
		animation *jumpAnim;
		animation *fallAnim;
		animation *curAnim;
		animation *deathAnim;
		shotType blaster;
	   	//unsigned char ca;
	   	//shotType blaster;
	   	physics *world;
	   	
	public:
		
		
		
		void move(int dirrection)
		{if(control){
			if (dir!=dirrection)
			dir=dirrection;
			if (speedX<1)speedX+=0.1;
			if (curAnim==standAnim)curAnim=walkAnim;
			curAnim->unfreeze();
		}}
		
	
		void jump()
		{if(control)
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
		
		void hurt(float damage)
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
		
		inline float getFrontX()
		{
			return gunX;
		}
		
		inline float getX()
		{
			return x;
		}
		inline float getY()
		{
			return y;
		}
		
		void shoot()
		{//if(control)
		{
			if (wpnHeat<=0)
			{
				world->addShot(gunX,y+gunY,0.3*dir,0,&blaster);
				wpnHeat=15;
			}
		}
		}
		
		void update()
		{if (curAnim!=deathAnim)
		{
			
			world->hit(x,y,x+sizeX,y+sizeY,true);
			if (inv>0)inv--;
			if (wpnHeat>0) wpnHeat--;
			y+=jumpSpeed*speedY;
			
			if ((y>limit)&&jumpPressed>0){speedY=0;jumpPressed=0;}
			if (speedY<=0)
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
			
			x+=dir*speedX*0.11;
			//if (!(world->collide(x,y-1)&&world->collide(x+sizeX,y-1))){ground=false;}
			speedY-=jumpSpeed*jumpHeight*0.5/world->gravity;
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
			if ((speedX>0.3)&&ground)
			{
				curAnim=walkAnim;
				curAnim->unfreeze();
			}
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
				else
				{
					inv=2;
				}
			}
		}
		}		
		
		inline bool is_dead()
		{
			deathAnim->is_end();
		}
		
		void revive()
		{
			deathAnim->unfreeze();
			curAnim=standAnim;
			health=8;
			x=startX;
			y=startY;
			control=true;
		}
		
		void drawGizmo()
		{
			glColor3f(0,1,1);
			glBegin(GL_QUADS);
				glVertex2f(floor(x),floor(y));
				glVertex2f(floor(x),ceil(y+sizeY));
				glVertex2f(ceil(x+sizeX),ceil(y+sizeY));
				glVertex2f(ceil(x+sizeX),floor(y));
			glEnd();
		}
		
		inline bool collide(float ox,float oy)
		{
			return (ox>x)&&(ox<x+sizeX)&&(oy>y)&&(oy<y+sizeX);
		}
		
	//	void shoot()
	//	{
	//		float tempX=x;
	//		if (dir>0) {tempX+=sizeX;}
			//world->addShot(&blaster,tempX,y+gunY);
	//	}
		
		void draw()
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
		
		inline float hull()
		{
			if (health>0) return health;
			else return 0;
		}
		
		
		
		player(float x_in,float y_in,GLuint *textank,GLuint *texshoot,physics *world_link)
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
		
		~player()
		{
			delete standAnim;
			delete walkAnim;
			delete jumpAnim;
			delete fallAnim;
			delete blaster.burstAnim;
			delete blaster.flyAnim;
		}
		
};

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
		enemy(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
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
		
		inline bool is_dead()
		{
			return deathAnim->is_end();
		}
		
		
		//
		inline void self_destruct()
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
		
		void update(player* p1)
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
				if (
					  ((dir>0)&&(world->collide(x+sizeX,y)||!world->collide(x+sizeX,y-1) ))
					||((dir<0)&&(world->collide(x,y)||!world->collide(x,y-1) ))
					) dir=-dir;
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
		
		
		void draw()
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
		
};






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
		
		enemyList(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
		{
			node *buf=new node;
			buf->link=new enemy(x_in,y_in,dirrection,tex,world_in);
			buf->next=NULL;
			buf->prev=NULL;
			first=buf;
			last=buf;
		}
		
		enemyList()
		{
			first=NULL;
			last=NULL;
		}
		
		void add(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
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
		
		void destroy(node *trash)
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
		
		void update(player *p1)
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
		
		void draw()
		{
			node *cur=first;
			while (cur!=NULL)
			{
				cur->link->draw();
				cur=cur->next;
			}
		}
		
		void self_destruct()
		{
			node *cur=first;
			while (cur!=NULL)
			{
				cur->link->self_destruct();
				cur=cur->next;
			}
		}
		
	
		~enemyList()
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
};

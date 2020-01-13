

#include "../../engine/World/World.h"

class Player
{
protected:
	GLuint *m_texture;
	float m_health = 8;
	bool m_control;
	bool m_roll;
	bool m_ground = true;
	float m_startX;
	float m_startY;
	float m_x;
	float m_y;
	float m_sizeX;
	float m_sizeY;
	char m_dir;
	float m_jumpHeight = 4;
	float m_JumpSpeed = 0.08;
	float m_speedX;
	float m_speedY;
	float m_cannonY;
	float m_cannonX;
	float m_hitDamage;
	int m_timeJumpPressed;
	int m_deathClock = 0;
	float m_jumpLimit;
	int m_weaponHeat = 0;
	unsigned m_curFrame;
	Animation *m_standAnim;
	Animation *m_walkAnim;
	Animation *m_jumpAnim;
	Animation *m_fallAnim;
	Animation *curAnim;
	Animation *m_deathAnim;
	shotType m_blaster;
	//unsigned char ca;
	//shotType m_blaster;
	World *m_world;

public:
	Player(float x_in, float y_in, GLuint *textank, GLuint *texshoot, World *world_link);

	void move(int dirrection);
	void jump();
	void hurt(float damage);
	inline float getFrontX() { return m_cannonX; }
	inline float getX() { return m_x; }
	inline float getY() { return m_y; }
	void shoot();
	void continueFalling();
	void update();
	inline bool is_dead() { return m_deathAnim->is_end(); }
	void revive();
	void drawGizmo();
	inline bool collide(float ox, float oy) { return (ox > m_x) && (ox < m_x + m_sizeX) && (oy > m_y) && (oy < m_y + m_sizeX); }
	void draw();
	inline float hull()
	{
		if (m_health > 0)
			return m_health;
		return 0;
	}

	~Player()
	{
		delete m_standAnim;
		delete m_walkAnim;
		delete m_jumpAnim;
		delete m_fallAnim;
		delete m_blaster.burstAnim;
		delete m_blaster.flyAnim;
	}
};

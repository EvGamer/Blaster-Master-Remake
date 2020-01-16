#include "../Player/Player.h"

class Enemy {
 private:
  float m_health;
  bool m_hit;
  float m_x;
  float m_y;
  float m_sizeX;
  float m_sizeY;
  float m_speedX = 1;
  float m_speedY;
  char m_dir;
  int m_reload;
  int m_heat;
  char m_shotLeft = 0;
  Animation *m_deathAnim;
  Animation *m_walkAnim;
  Animation *m_shootAnim;
  Animation *m_curAnim;
  GLuint m_texture;
  World *m_world;
  MissleType m_grenade;

 public:
  Enemy(float a_x, float a_y, char a_dirrection, GLuint a_textureId, World *a_world);
  inline bool is_dead() { return m_deathAnim->is_end(); }
  void self_destruct();
  void update(Player *p1);
  void draw();
};

#include "Player.h"

const float CANNON_Y = 0.6;
const float CANNON_Y_FALLING = 0.4;
const float CANNON_Y_JUMPING = 0.6;
const float JUMP_HEIGHT = 3;

void Player::move(int dirrection) {
  if (m_control) {
    m_dir = dirrection;
    if (m_speedX < 1) m_speedX += 0.1;
    if (curAnim == m_standAnim) curAnim = m_walkAnim;
    curAnim->unfreeze();
  }
}

void Player::jump() {
  if (m_control) {
    m_timeJumpPressed--;
    if (m_ground) {
      m_speedY = m_jumpHeight;
      m_timeJumpPressed = 20;
      m_ground = false;
      m_jumpLimit = m_y + JUMP_HEIGHT;
      curAnim = (m_jumpAnim);
      m_cannonY = CANNON_Y;
    } else {
      m_timeJumpPressed--;
    }
  }
}

void Player::hurt(float damage) {
  if (m_deathClock <= 0) {
    m_deathClock = 45;
    m_speedX *= 0.5;
    m_speedY += 1;
    m_health -= damage;
    damage = 0;
  }
}

void Player::shoot() {  // if(m_control)
  if (m_weaponHeat <= 0) {
    m_world->addMissle(m_cannonX, m_y + m_cannonY, 0.3 * m_dir, 0, &m_blaster);
    m_weaponHeat = 15;
  }
}

void Player::continueFalling() {
  if (m_world->collide(m_x + 0.05, m_y) ||
      m_world->collide(m_x + m_sizeX - 0.05, m_y)) {
    m_y = floor(m_y + 1);
    m_speedY = 0;
    m_ground = true;
    curAnim = m_standAnim;
    m_cannonY = CANNON_Y_FALLING;
  } else {
    m_ground = false;
    curAnim = m_fallAnim;
  }
}

void Player::update() {
  if (curAnim != m_deathAnim) {
    m_world->hit(m_x, m_y, m_x + m_sizeX, m_y + m_sizeY, true);
    if (m_deathClock > 0) m_deathClock--;
    if (m_weaponHeat > 0) m_weaponHeat--;
    m_y += m_JumpSpeed * m_speedY;

    if ((m_y > m_jumpLimit) && m_timeJumpPressed > 0) {
      m_speedY = 0;
      m_timeJumpPressed = 0;
    }
    if (m_speedY <= 0) continueFalling();

    m_x += m_dir * m_speedX * 0.11;
    // if
    // (!(m_world->collide(m_x,m_y-1)&&m_world->collide(m_x+m_sizeX,m_y-1))){m_ground=false;}
    m_speedY -= m_JumpSpeed * m_jumpHeight * 0.5 / m_world->getGravity();
    m_speedX -= 0.05;

    if (m_speedX < 0) {
      m_speedX = 0;
    };
    if (m_dir > 0) {
      if (m_world->collide(m_x + m_sizeX, m_y)) {
        m_x = floor(m_x + 2) - m_sizeX;
        m_speedX = 0;
      }
      m_cannonX = m_x + m_sizeX;
    } else {
      if (m_world->collide(m_x, m_y)) {
        m_x = floor(m_x + 1);
        m_speedX = 0;
      }
      m_cannonX = m_x;
    }

    if (m_speedY > 0) {
      if ((m_world->collide(m_x, m_y + m_sizeY)) ||
          (m_world->collide(m_x + m_sizeX - 0.1f, m_y + m_sizeY)))  //
      {
        m_y = floor(m_y + m_sizeY) - m_sizeY;
        m_speedY = 0;
      };
    };

    if ((m_speedX > 0.3) && m_ground) {
      curAnim = m_walkAnim;
      curAnim->unfreeze();
    }

    m_hitDamage = m_world->hit(m_x - 0.5, m_y, m_x + m_sizeX + 0.5,
                               m_y + m_sizeY + 0.5, true);
    if (m_hitDamage != 0) hurt(m_hitDamage);
    if (m_health <= 0) {
      if (m_ground) {
        curAnim = m_deathAnim;
        m_control = false;
        m_speedX = 0;
      } else {
        m_deathClock = 2;
      }
    }
  }
}

void Player::revive() {
  m_deathAnim->unfreeze();
  curAnim = m_standAnim;
  m_health = 8;
  m_x = m_startX;
  m_y = m_startY;
  m_control = true;
}

void Player::drawGizmo()  // debug option. Displays collision box.
{
  glColor3f(0, 1, 1);
  glBegin(GL_QUADS);
  glVertex2f(floor(m_x), floor(m_y));
  glVertex2f(floor(m_x), ceil(m_y + m_sizeY));
  glVertex2f(ceil(m_x + m_sizeX), ceil(m_y + m_sizeY));
  glVertex2f(ceil(m_x + m_sizeX), floor(m_y));
  glEnd();
}

void Player::draw() {
  float x1, x2;
  if (m_dir < 0) {
    x1 = m_x;
    x2 = m_x + 2;
  } else {
    x2 = m_x + m_sizeX;
    x1 = x2 - 2;
  };
  if (curAnim != m_deathAnim) {
    switch (m_deathClock % 3) {
      case 0:
        glColor3f(1, 1, 1);
        break;
      case 1:
        glColor3f(0.5, 1, 0.7);
        break;
      case 2:
        glColor3f(1, 0.3, 0);
        break;
    }

    curAnim->setCol(m_curFrame);
    // Preserving wheel position between animations

    m_curFrame = curAnim->draw(-m_dir, x1, m_y, x2, m_y + 2);
    curAnim->freeze();
  } else
    curAnim->draw(-1, x1 - 0.5, m_y, x1 + 3.5, m_y + 4);
}

Player::Player(float a_x, float a_y, GLuint *a_textureId,
               GLuint *a_missleTextureId, World *a_world) {
  m_x = a_x;
  m_y = a_y;
  m_dir = 1;
  m_cannonY = CANNON_Y;
  m_cannonX = m_x;
  m_hitDamage = 0;
  m_jumpLimit = m_y + JUMP_HEIGHT;
  m_timeJumpPressed = 0;
  m_roll = false;
  m_world = a_world;
  m_startX = m_x;
  m_startY = m_y;
  m_speedX = 0;
  m_speedY = 0;
  m_sizeX = 26.0 / 16.0;
  m_sizeY = 18.0 / 16.0;
  m_control = true;

  // Animation
  m_texture = a_textureId;
  m_standAnim = new Animation(m_texture, 0.125, 0, 0, 1, 4, 1, LOOP);
  m_walkAnim = new Animation(m_texture, 0.125, 0, 0, 3, 4, 3, LOOP);
  m_jumpAnim = new Animation(m_texture, 0.125, 0, 3, 1, 4, 2, LOOP);
  m_fallAnim = new Animation(m_texture, 0.125, 0, 2, 1, 4, 2, LOOP);
  m_deathAnim = new Animation(m_texture, 0.25, 0, 2, 1, 3, 3, ONCE);
  curAnim = m_standAnim;
  m_curFrame = 0;

  // Weapon
  m_blaster.texture = a_missleTextureId;
  m_blaster.burstAnim =
      new Animation(m_blaster.texture, 0.25f, 3, 0, 4, 1, 2, ONCE);
  m_blaster.flyAnim =
      new Animation(m_blaster.texture, 0.5f, 0.25f, 0, 0, 1, 1, 0, LOOP);
  m_blaster.damage = 1.5;
  m_blaster.foe = false;
  m_blaster.spriteX = -1.5;
  m_blaster.spriteY = -0.4;
  m_blaster.falling = false;
}

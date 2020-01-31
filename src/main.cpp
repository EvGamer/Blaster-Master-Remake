/**************************
 * Includes
 *
 **************************/
//

#include "entities/Enemy.h"
#include "entities/EnemyFactory.h"
#include "engine/World.h"
#include <list>

const unsigned COORD_UNIT = 32;
const unsigned WINDOW_WIDTH = 1024;
const unsigned WINDOW_HEIGHT = 768;
constexpr unsigned TILE_COLUMNS = WINDOW_WIDTH / COORD_UNIT;
constexpr unsigned TILE_ROWS = WINDOW_HEIGHT / COORD_UNIT;

unsigned int key;

bool keyJump = false;
bool keyMove = false;
bool keyShoot = false;
bool keyRestart = false;
char dir = 1;
unsigned char n = 0;

/*********************
 * Function Decl
 *
 **************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam);  //
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

//��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int iCmdShow) {  //
  WNDCLASS wc;
  HWND hWnd;
  HDC hDC;
  HGLRC hRC;
  MSG msg;
  BOOL bQuit = FALSE;

  /* register window class */
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "GLSample";
  RegisterClass(&wc);

  /* create main window */
  hWnd = CreateWindow(
    "GLSample", "Blaster Master",
    WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
    0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
    NULL, NULL, hInstance, NULL
  );

  /* enable OpenGL for the window */
  EnableOpenGL(hWnd, &hDC, &hRC);
  World world("maps/Area3.tmx");
  world.setGlobalFriction(1);
  world.setGravity(1);
  world.init();
  float camX = 8;
  float camY = 7;

  GLuint texHealthBar = loadTexture("Sprites\\HealthBar.tga");
  GLuint texMessage = loadTexture("Sprites\\Message.tga");
  GLuint texVictory = loadTexture("Sprites\\Victory.tga");
  GLuint texBack = loadTextureL("Sprites\\Background.bmp");  //

  // testdraw();
  // DrawTile(0,1,0,0,world.get_texture());r//

  /* program main loop */
  while (!bQuit)  //
  {
    /* check for messages */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      /* handle or dispatch messages */
      if (msg.message == WM_QUIT) {
        bQuit = TRUE;
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    } else {
      /* OpenGL animation code goes here */
      world.applyCamera();
      // attempt to make camera move by whole pixels
      if (keyMove) {
        world.player->move(dir);
      }
      if (keyJump) world.player->jump();
      if (keyShoot) world.player->shoot();
      world.update();  //
      world.draw();
      // world.player->drawGizmo();
      world.player->update();
      
      for (auto& enemy : world.enemies) {
        enemy.update(*world.player);
        enemy.draw();
      }
      world.enemies.remove_if([](Enemy &e){
        return e.isDead();
      });
      world.player->draw();
      // drawing healthBar
      const float HBx = 0;
      const float HBy = 5;
      const float HBx1 = HBx + 2;
      const float HBy1 = HBy + 4;
      const float bl = 25 / 64;
      float rate = (1 - bl) * ceil(world.player->getHealth() + 8) * 0.0625 - 0.125;  //
      glLoadIdentity();
      drawSprite(texHealthBar, HBx, HBy, HBx1, HBy1, 0, 0, 0.5, 1);
      drawSprite(texHealthBar, HBx, HBy, HBx1, HBy + 4 * rate, 0.5, 1 - rate,
                 1, 1);
      if (world.player->isDead()) {
        glColor3f(1, 1, 1);
        drawSprite(texMessage, 15, 15, 23, 23, 0, 0, 1, 1);
        glColor3f(1, 0, 0);
        if (keyRestart) {
          world.init();
        }
      }
      SwapBuffers(hDC);
      glClear(GL_COLOR_BUFFER_BIT);
      Sleep(10);
    }
  }

  /* shutdown OpenGL */
  DisableOpenGL(hWnd, hDC, hRC);

  /* destroy the window explicitly */
  DestroyWindow(hWnd);

  return msg.wParam;
}

/********************
 * Window Procedure
 *
 ********************/
///
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_CREATE:
      return 0;
    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;

    case WM_DESTROY:
      return 0;

    case WM_KEYUP:
      switch (wParam) {
          // case 0x41
        case VK_LEFT:
          keyMove = false;
          dir = -1;
          return 0;
          // case 0x44:
        case VK_RIGHT:  //
          keyMove = false;
          dir = 1;
          return 0;
        case VK_UP:
          keyJump = false;
          return 0;
        case VK_NUMPAD0:
          keyShoot = false;
          return 0;
        case VK_RETURN:
          keyRestart = false;
          return 0;
      };
    case WM_KEYDOWN:
      switch (wParam)  //
      {
        case VK_ESCAPE:
          PostQuitMessage(0);
          return 0;
        case VK_LEFT:
          keyMove = true;
          dir = -1;
          return 0;
          // case 0x44:..
        case VK_RIGHT:  //
          keyMove = true;
          dir = 1;
          return 0;
        case VK_UP:
          keyJump = true;
          return 0;
        case VK_NUMPAD0:  //
          keyShoot = true;
          return 0;
        case VK_RETURN:
          keyRestart = true;
          return 0;
      }
      return 0;  //

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
}
//

/*******************
 * Enable OpenGL

 *******************/
//

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
  PIXELFORMATDESCRIPTOR pixelFormat;
  int iFormat;

  /* get the device context (DC) */
  *hDC = GetDC(hWnd);

  /* set the pixel format for the DC */  //
  ZeroMemory(&pixelFormat, sizeof(pixelFormat));
  pixelFormat.nSize = sizeof(pixelFormat);
  pixelFormat.nVersion = 1;
  pixelFormat.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                         PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE);
  pixelFormat.iPixelType = PFD_TYPE_RGBA;
  pixelFormat.cColorBits = 24;
  pixelFormat.cDepthBits = 16;
  pixelFormat.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat(*hDC, &pixelFormat);
  SetPixelFormat(*hDC, iFormat, &pixelFormat);
  /* create and enable the render context (RC) */
  *hRC = wglCreateContext(*hDC);
  wglMakeCurrent(*hDC, *hRC);
  glLoadIdentity();
  gluOrtho2D(0, TILE_COLUMNS, 0, TILE_ROWS);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
};

/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);
}

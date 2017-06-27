/**************************
 * Includes
 *
 **************************///


#include "enemylist.h"

unsigned int key;

bool keyJump=false;
bool keyMove=false;
bool keyShoot=false;
bool keyRestart=false;
char dir=1;
unsigned char n=0;

/*********************
 * Function Decl
 *
 **************************/
 
void spawnEnemies(enemyList *jerks,GLuint *texEnemy,physics *Area)
{
	jerks->add(22,11,-1,texEnemy,Area);
    jerks->add(24,16,1,texEnemy,Area);
    jerks->add(30,16,1,texEnemy,Area);
    jerks->add(12,24,-1,texEnemy,Area);
    jerks->add(18,20,-1,texEnemy,Area);
    jerks->add(19,28,1,texEnemy,Area);
    jerks->add(27,32,1,texEnemy,Area);
    jerks->add(35,36,1,texEnemy,Area);
    jerks->add(43,43,-1,texEnemy,Area);
    jerks->add(22,44,-1,texEnemy,Area);
    jerks->add(9,49,1,texEnemy,Area);
    jerks->add(15,53,-1,texEnemy,Area);
    jerks->add(29,49,-1,texEnemy,Area);
    jerks->add(36,53,-1,texEnemy,Area);
    jerks->add(46,49,-1,texEnemy,Area);
    jerks->add(52,40,1,texEnemy,Area);
    jerks->add(52,32,-1,texEnemy,Area);
    jerks->add(52,24,-1,texEnemy,Area);
    jerks->add(52,16,-1,texEnemy,Area);
    jerks->add(61,36,-1,texEnemy,Area);
    jerks->add(61,28,-1,texEnemy,Area);
    jerks->add(61,20,-1,texEnemy,Area);
    
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);//
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

 
//þþ
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{//
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
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "Blaster Master", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 1024, 768,
      NULL, NULL, hInstance, NULL);

	


    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    physics Area3("Sprites\\A3a.tga",16);
    Area3.setGlobalFriction(1);
    Area3.setGravity(1);
    Area3.setDefault(1,255);
    Area3.setSolid(6,8);
    Area3.setSolid(6+16,8+16);
    Area3.setSolid(6+16*5,8+16*5);
    Area3.setSolid(4+16*9,5+16*9);
    Area3.setSolid(4+16*10,5+16*10);
    float camX=8;
	float camY=7;
    float m=0;
 

	GLuint texSophia=loadTexture("Sprites\\SOPHIA.tga");
	GLuint texBlaster=loadTexture("Sprites\\Shot.tga");
	GLuint texEnemy=loadTexture("Sprites\\enemy.tga");
	GLuint texHealthBar=loadTexture("Sprites\\HealthBar.tga");
	GLuint texMessage=loadTexture("Sprites\\Message.tga");
	GLuint texVictory=loadTexture("Sprites\\Victory.tga");
	GLuint texBack=loadTextureL("Sprites\\Background.bmp");//
	enemyList jerks;
	spawnEnemies(&jerks,&texEnemy,&Area3);
    player sophia(10,12,&texSophia,&texBlaster,&Area3);
    
	//testdraw();
	//DrawTile(0,1,0,0,Area3.get_texture());r//
	        
    /* program main loop */
    while (!bQuit)//
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            float sx,sy;
            sx=sophia.getX();
            sy=sophia.getY();
            if(m>0)m-=0.05*m;
            float camS =camY + 8 - sy;
            float camN =camY + 16 - sy;
           	float camW =camX + 10 - sx;
            float camE =camX + 20 -sx;
            if ((camS>0)&&(camY>5)) {camY-=camS;m+=0.005-m*0.2;}
            if ((camN<0)&&(camY<51)){ camY-=camN;m+=0.005-m*0.2;}
            if ((camW>0)&&(camX>4)) {camX-=camW;m+=0.005-m*0.2;}
            if ((camE<0)&&(camX<38)) {camX-=camE;m+=0.005-m*0.2;}
            glLoadIdentity();
            drawSprite(&texBack,0,0,32,32,camX/64-m,-camY/64-m,camX/64+1+m,-camY/64+1+m);
            glTranslatef(-camX/16,-camY/12,0);
            if (keyMove) {sophia.move(dir);	}
			if (keyJump) sophia.jump();
			if (keyShoot)sophia.shoot();
			Area3.update();//
			Area3.drawLevel(camX,camY);
			//sophia.drawGizmo();
    		sophia.update();
    		jerks.update(&sophia);
    		jerks.draw();
    		sophia.draw();
    		//drawing healthBar
    		const float HBx=0;
    		const float HBy=5;
    		const float HBx1=HBx+2;
    		const float HBy1=HBy+4;
    		const float bl=25/64;
    		float rate=(1-bl)*ceil(sophia.hull()+8)*0.0625-0.125;//
    		glLoadIdentity();
    		drawSprite(&texHealthBar,HBx,HBy,HBx1,HBy1,0,0,0.5,1);
    		drawSprite(&texHealthBar,HBx,HBy,HBx1,HBy+4*rate,0.5,1-rate,1,1);
    		if (sophia.is_dead())
    		{
    			jerks.self_destruct();
				glColor3f(1,1,1);
    			drawSprite(&texMessage,15,15,23,23,0,0,1,1);
				glColor3f(1,0,0);
    			if (keyRestart)
    			{
    				sophia.revive();
    				
    				spawnEnemies(&jerks,&texEnemy,&Area3);
    			}
    		}
    		if ((sx>=53)&&(sx<=56)&&(sy>=7)&&(sy<=9))
    		{
    			drawSprite(&texVictory,15,15,23,23,0,0,1,1);;
    			if (keyRestart)
    			{
    				jerks.self_destruct();
    				sophia.revive();
    				spawnEnemies(&jerks,&texEnemy,&Area3);
    			}
    		}
            SwapBuffers (hDC);
            glClear(GL_COLOR_BUFFER_BIT);
            Sleep (10);
        }
    }


    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/
///
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;
	
	case WM_KEYUP: 
		switch (wParam)
    	{
            		//case 0x41
        case VK_LEFT:
           		keyMove=false;
				dir=-1;
        		return 0;
          		  	//case 0x44:
        case VK_RIGHT://
          		keyMove=false;
         		dir=1;
        		return 0;
        case VK_UP:
            	keyJump=false;
       			return 0;
       	case VK_NUMPAD0:
        		keyShoot=false;
        		return 0;
        case VK_RETURN:
    			keyRestart=false;
    			return 0;
		};
    case WM_KEYDOWN:
        switch (wParam)//
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        case VK_LEFT:
            keyMove=true;
			dir=-1;
            return 0;
            		//case 0x44:..
        case VK_RIGHT://
            keyMove=true;
            dir=1;
        	return 0;
    	case VK_UP:
			keyJump=true;
            return 0;
        case VK_NUMPAD0://
        	keyShoot=true;
        	return 0;
    	case VK_RETURN:
    		keyRestart=true;
    		return 0;
        }	
        return 0;//

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}
//

/*******************
 * Enable OpenGL
 
 *******************///

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC *///
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);
    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
	glLoadIdentity(); 
	gluOrtho2D(0,32,0,24);
	glMatrixMode(GL_PROJECTION); 
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

};
 
/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

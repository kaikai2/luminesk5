#include <cstdio>
#include <glCurvedAni.h>
#include "gl/gl.h"
//#include <tnl.h>
//#include <tnlNetObject.h>

#include "effect/sound.h"
#include "logic/lumines.h"
#include "skin/luskin.h"

#pragma comment(lib, "bass.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
using namespace cAni;

// define NEED_FPS will display fps 
//#define NEED_FPS

Lumines *g_pGame = NULL;


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        //g_pSys->Resize(480, 272);
        return S_OK;
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return 0;
        }
    default:
        if (g_pGame && g_pGame->msgHandle(hWnd, msg, wParam, lParam))
            return true;
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

class DataDisplayer
{
public:
    DataDisplayer()
    {
        for (int i=0;i<100;i++)
        {
            data[i] = 0.f;
        }
        last = 0;
        min = 0;
        max = 0;
        scale = 1.f;
    }
    void push(float _data)
    {
        last = (last+1)%100;
        if (_data > 100)
            _data = 100;
        data[last] = _data;
        min = max = 0;
        for (int i=1;i<100;i++)
        {
            if (data[i]>data[max])max = i;
            if (data[i]<data[min])min = i;
        }
    }
    void render()
    {
        int i;
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glLoadIdentity();
        glTranslatef(3,68,0);
        glScalef(1,2,1);
        glBegin(GL_LINES);
        glColor3f(.3f,.3f,.3f);
        for (i = 0; i < 10; i ++)
        {
            glVertex2d(0, i*10);
            glVertex2d(100, i*10);
        }
        glColor3f(0.f,1.f,.3f);
        glVertex2d(0, 100-60);
        glVertex2d(100, 100-60);
        glColor3f(1.f,1.f,.3f);
        glVertex2d(0, 100);
        glVertex2d(100, 100);
        glEnd();     
        // glTranslatef(0, 240,0);
        glColor3f(1,0,0);
        glBegin(GL_LINE_STRIP);
        for (i = (last+1)%100; i != last; i = (i+1)%100)
        {
            glVertex2d((100 + i - last)%100, min(100, max(0, 100 - data[i])));
        }
        glEnd();
        
    }
    int last;
    float data[100];
    int min, max;
    float scale;
};
#ifndef SAFE_DELETE
#define SAFE_DELETE(a) if (!a);else{delete (a);(a) = 0;}
#endif
void exitGameRelease()
{
    //SAFEDELETE(g_pfont);
    //SAFEDELETE(g_pscorefont);
    SAFE_DELETE(g_pGame);
    //SAFEDELETE(g_pTexres);
    EventManager::releaseInstance();
    Sound::releaseInstance();
}
void check()
{
    MessageBox(0,"check point", "lumines", MB_OK);
}
class IWindow
{
public:
    virtual void createWindow(int width, int height) = 0;
};
class MSWindowsWindow : public IWindow
{
public:
    MSWindowsWindow(HINSTANCE hInstance) : m_hInstance(hInstance), m_hWnd(0)
        , m_hDC(0)
        , m_hGlRC(0)
    {
    }
    virtual void createWindow(int width, int height)
    {
        m_hWnd = InitWindow(m_hInstance, width, height);
        CreateOpengl(width, height);
    }
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC m_hDC;
    HGLRC m_hGlRC;
private:
    HWND InitWindow(HINSTANCE hInstance, int width, int height)
    {
        WNDCLASSEX wcex;
        HWND hWnd;
        wcex.cbSize         = sizeof(WNDCLASSEX);
        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = (WNDPROC)WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInstance;
        wcex.hIcon          = NULL;
        wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = NULL;
        wcex.lpszClassName  = "lumines";
        wcex.hIconSm        = NULL;

        RegisterClassEx(&wcex);
        int dx = GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
        int dy = GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
        hWnd = CreateWindow(
            wcex.lpszClassName, "lumines@pc, the remake",
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, 0, width + dx, height + dy, NULL, NULL, hInstance, NULL
            );
        ShowWindow(hWnd, SW_SHOW);
        return hWnd;
    }
    bool CreateOpengl(int width, int height)
    {
        int bpp = 32;

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,                            
            PFD_DRAW_TO_WINDOW |        
            PFD_SUPPORT_OPENGL |   PFD_SWAP_LAYER_BUFFERS|     
            PFD_DOUBLEBUFFER,            
            PFD_TYPE_RGBA,                
            bpp,            
            0, 0, 0, 0, 0, 0,
            0,                
            0,                
            0,                
            0, 0, 0, 0,        
            16,                        //16 bit Z-Buffer
            8,          // 8 bit Stencil buffer
            0,
            PFD_MAIN_PLANE,    
            0,                
            0, 0, 0            
        };
        GLuint PixelFormat;
        m_hDC = ::GetDC(m_hWnd);
        if (m_hDC == 0)
            return false;

        if(!(PixelFormat = ::ChoosePixelFormat(m_hDC,&pfd)))
            return false;

        if(!::SetPixelFormat(m_hDC,PixelFormat, &pfd))
            return false;
        if (m_hGlRC == NULL)
        {
            m_hGlRC = ::wglCreateContext(m_hDC);
            if (m_hGlRC == NULL)
                return false;
        }
        if (!::wglMakeCurrent(m_hDC, m_hGlRC))
            return false;

        glMatrixMode(GL_PROJECTION);    /* prepare for and then */ 
        glLoadIdentity();               /* define the projection */
        glOrtho(0, width, height, 0, -100.0f, 100.0);/* transformation */
        glMatrixMode(GL_MODELVIEW);  /* back to modelview matrix */
        glLoadIdentity();
        glViewport(0, 0, width, height);      /* define the viewport */
        return true;
    }
};
int WINAPI WinMain(HINSTANCE hinstance, 
                   HINSTANCE hprevinstance, 
                   LPSTR lpcmdline, int ncmdshow)
{
    //auxInitDisplayMode(AUX_RGBA | AUX_DOUBLE);
    //auxInitPosition(0, 0, 480 + 5, 272 + 24);
    //auxInitWindow("CurvedAni|gl port demo");
    //auxKeyFunc(AUX_A, keyA)
    //auxIdleFunc(RenderFunc);
    MSWindowsWindow window(hinstance);
    glCurvedAniSystem system;
    //glRenderer renderer;
    //system.attach(&renderer);
    atexit(exitGameRelease);
    Sound* pSound = Sound::getInstance();
    if (pSound == NULL || !pSound->init())
    {
        //exitGameRelease();
        return -1;
    }
    // pSound->loadSkin("skin.ini");
    window.createWindow(480, 272);
    //window.createWindow(800, 600);
    HWND hWnd = window.m_hWnd;
    EventManager *pEventManager = EventManager::getInstance();
    DataDisplayer fpsdisp;
    cAni::iAnimResManager *pAnimResManager = system.createAnimResManager();
    g_pGame = new Lumines(*pAnimResManager);
    //auxMainLoop(RenderFunc);

    MSG msg;
    
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
#if defined(_DEBUG) || defined(NEED_FPS)
    DWORD t, t2 = 0, frame = 0;
    int fps = 0, curfps = 0;
#endif
    //extern void initbspline();
    //initbspline();
    //g_pfont = new Font;
    //g_pfont->init(g_pSys, "Impact", IDM_BOLD);
    //g_pscorefont = new Font;
    //g_pscorefont->init(g_pSys, "Times New Roman", IDM_CHARACTER);

    //Font *fontlist[2]={g_pfont,g_pscorefont,};
//    glEnable(GL_DEPTH_TEST);

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else //if (g_pSys->CheckFrameRateTime())
        {
            DWORD now = timeGetTime();
            static DWORD lastTime = timeGetTime() - 1;
            if (now - lastTime > 16)
            {
                pEventManager->tick();
                g_pGame->step();
#if defined(_DEBUG) || defined(NEED_FPS)
                frame++;
                if (frame%10 == 0)
                {
                    t = now;//timeGetTime();
                    //*
                    float tfps;
                    if (t - t2)
                    {
                        tfps = 10000.f / (t - t2);
                        fpsdisp.push(tfps);
                    }
                    t2 = t;
                    /*
                    curfps = int(tfps);
                    if (fps != curfps)
                    {
                        fps = curfps;
                        char str[64];
                        sprintf(str,"ogl2d.lumines@ %d fps",fps);
                        ::SetWindowText(hWnd, str);
                    }//*/
                }
#endif
                lastTime = now;


                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
                glLoadIdentity();
                g_pGame->render();
#if defined(_DEBUG) || defined(NEED_FPS)
                fpsdisp.render();
#endif
                glFlush();
                wglSwapLayerBuffers(window.m_hDC, WGL_SWAP_MAIN_PLANE);
                //::SwapBuffers(window.m_hDC);
                glFinish();
            }
            else
            {
                int d = 10 - now - lastTime;
                if (d > 0)
                {
                    Sleep(d);
                }
            }
        }
    }
    //system.dettach(&renderer);
    system.cAni::iSystem::release(pAnimResManager);
    return 0;
}

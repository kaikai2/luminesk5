#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdio>

#include "system.h"
#import <timegettime.h>
namespace ogl2d
{

struct SystemList
{
    ::ogl2d::System* m_pSystem;
    bool operator == (HWND hWnd) const
    {
        return m_pSystem->GetHWnd() == hWnd;
    }
    bool operator == (::ogl2d::System* pSystem) const
    {
        return m_pSystem == pSystem;
    }
};

static ::std::vector< SystemList > s_Systems;

void System::ShutDown()
{
    for (::std::vector< SystemList >::iterator sys = s_Systems.begin(); sys != s_Systems.end(); ++sys)
    {
        delete sys->m_pSystem;
    }
    s_Systems.clear();
}

void  callback_reshape(GLsizei w, GLsizei h)
{

    glMatrixMode(GL_PROJECTION);    /* prepare for and then */ 
    glLoadIdentity();               /* define the projection */
    glOrthof(0, w, h, 0, -10.0f, 10.0);/* transformation */

    glMatrixMode(GL_MODELVIEW);  /* back to modelview matrix */
    glLoadIdentity();
    glViewport(0, 0, w, h);      /* define the viewport */

}

System* System::GetInstance(HWND hWnd)
{
    System* pSys = NULL;
    ::std::vector< SystemList >::iterator sys = ::std::find(s_Systems.begin(), s_Systems.end(), hWnd);
    if (sys == s_Systems.end())
    {
        if (s_Systems.empty())
        {
    //        extern void initbspline();
    //        initbspline();
        }
		
        pSys = new System(21);
        SystemList node;
        node.m_pSystem = pSys;
        s_Systems.push_back(node);
    }
    else
    {
        pSys = sys->m_pSystem;
    }

    return pSys;
}

void System::ReleaseInstance(System*& pSystem)
{
    if (pSystem)
    {
        delete pSystem; 
        ::std::vector< SystemList >::iterator sys = ::std::find(s_Systems.begin(), s_Systems.end(), pSystem);
        assert(sys != s_Systems.end());
        s_Systems.erase(sys);
        pSystem = NULL; 
    }
}

System::~System()
{
    Release();
}

System::System(HWND hWnd):
    lastTextureId(-1), m_hWnd(hWnd), m_hDC(0), m_hRC(0), m_bWindowValid(false), m_bFullScreen(false)
#ifdef _ESGL 
		, eglDisplay(0)
#endif
{
}

void System::Release()                                // Properly Kill The Window
{
    if (m_bFullScreen)                                        // Are We In Fullscreen Mode?
    {
                                       // Show Mouse Pointer
    }

	if (m_hRC)                                            // Do We Have A Rendering Context?
    {
                                             // Set RC To NULL
    }


    if (m_hDC)
    {                    // Are We Able To Release The DC
        m_hDC = NULL;                                        // Set DC To NULL
    }
/*
    if (hWnd && !DestroyWindow(hWnd))                    // Are We Able To Destroy The Window?
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;                                        // Set hWnd To NULL
    }

    if (!UnregisterClass("OpenGL",hInstance))            // Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;                                    // Set hInstance To NULL
    }*/

}

bool System::Create(int width, int height, bool bFullScreen)
{
    int bpp = 32;

    if (bFullScreen)                                             // Attempt Fullscreen Mode?
    {
        
//    SetFrameRate();
	}

    return true;
}

void System::Active()
{
    assert(m_hDC && m_hRC);

}





void System::Resize(int width, int height)
{
    callback_reshape(width, height);
}

void System::BeginScene()
{
    //glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void System::Flush(bool withClear)
{
    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0,0,-5);
    normalPass.Render(lastTextureId);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    glTranslatef(0,0,-0.5);
    
    alphaPass.Render(lastTextureId);
    */
    glFlush();

    if (withClear)
    {
        BeginScene();
    }

}

void System::Pop(Clip *clip, int needAlpha)
{
    (needAlpha?alphaPass:normalPass).PopClip(clip);
}

void System::Push(Clip *clip, int needAlpha)
{
    (needAlpha?alphaPass:normalPass).PushClip(clip);
}

bool System::CheckFrameRateTime()
{
    DWORD now =timeGetTime() & 0x1fffffff; // ºöÂÔ¸ß3Î»
    DWORD next = (m_LastFrameBeginTime + m_FrameRateDelay)>>3;
    if (now >= next)
    {
        m_LastFrameBeginTime += m_FrameRateDelay;
        return true;
    }
    return false;
}

void System::SetFrameRate(int rate)
{
    m_LastFrameBeginTime =timeGetTime() << 3;
    m_FrameRateDelay = 8000 / rate;
}


}; // namespace ogl2d
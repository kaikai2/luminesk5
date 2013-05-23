


#import <Winmain.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <EAGLView.h>

using namespace ogl2d;

//extern Texture *block_tex; // in block.cpp
// define NEED_FPS will display fps 
//#define NEED_FPS

System *g_pSys = NULL;
TextureResource *g_pTexres = NULL;

Font *g_pfont = NULL;
Font *g_pscorefont = NULL;
Lumines * g_pGame;
EventManager *pEventManager;




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
        
        
    }
    int last;
    float data[100];
    int min, max;
    float scale;
};

void exitGameRelease()
{
    SAFEDELETE(g_pfont);
    SAFEDELETE(g_pscorefont);
    SAFEDELETE(g_pGame);
    SAFEDELETE(g_pTexres);
    EventManager::releaseInstance();
    System::ReleaseInstance(g_pSys);
    System::ShutDown();
    Sound::releaseInstance();
};

int  WinMain()
{
    atexit(exitGameRelease);
    Sound* pSound = Sound::getInstance();
    if (pSound == NULL || !pSound->init())
    {
        //exitGameRelease();
        return -1;
    }
    // pSound->loadSkin("skin.ini");
    g_pSys = System::GetInstance();
    g_pSys->Create(320, 480);//useless zhu
    pEventManager = EventManager::getInstance();
    DataDisplayer fpsdisp;
    g_pTexres = new TextureResource;
    if (g_pTexres == NULL)
    {
        //exitGameRelease();
        return -1;
    }
    //block_tex = g_pTexres->NewTexture("block.tga");
    
#if defined(_DEBUG) || defined(NEED_FPS)
    DWORD t, t2 = 0, frame = 0;
#endif
    int fps = 0, curfps = 0;
    extern void initbspline();
    initbspline();
    g_pfont = new Font;
    g_pfont->init(g_pSys, "Impact", IDM_BOLD);
    g_pscorefont = new Font;
    g_pscorefont->init(g_pSys, "Times New Roman", IDM_CHARACTER);

    Font *fontlist[2]={g_pfont,g_pscorefont,};
    g_pGame = new Lumines(fontlist);
//    glEnable(GL_DEPTH_TEST);
	/*
    while (1)
    {
        if (g_pSys->CheckFrameRateTime())
        {
            DWORD now = ::timeGetTime();
            pEventManager->tick();
            g_pGame->step();

            static DWORD lastTime = ::timeGetTime() - 1;
            if (now - lastTime > 10)
            {
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
                    //*
                    curfps = int(tfps);
                    if (fps != curfps)
                    {
                        fps = curfps;
                        char str[64];
                        sprintf(str,"OPENGL ES.lumines@ %d fps",fps);


                       //zhu ::SetWindowText(hWnd, str);
                    }//
                }
#endif
                lastTime = now;

            }
        }
 
    }
	*/
    return 0;
}

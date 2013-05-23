#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#import "face2d.h"
#import "clip.h"
#import <timegettime.h>
#import <OpenGLES/ES1/gl.h>
// zhu
# define HWND int
# define HDC int
# define HGLRC int
typedef unsigned short WORD;
typedef unsigned long DWORD;


namespace ogl2d
{
#define OGL2D_DEFAULT_FRAME_RATE 60
class System 
{
public:
    static System* GetInstance(HWND hWnd=21);//zhu
    static void ReleaseInstance(System*& pSystem);
    static void ShutDown();

    // ����opengl����
    bool Create(int width, int height, bool bFullScreen = false);

    // ����opengl����
    void Release();

    // ��ʼ��ͼ
    void BeginScene();

    // ������ͼ�����
    // @param bool  withClear   ĩβ����BeginScene
    void Flush(bool withClear = true);

    // �޳�ĳ��ͼԪ
    void Pop(Clip *clip, int needAlpha = 0);

    // ���ĳ��ͼԪ
    void Push(Clip *clip, int needAlpha = 0);

    // �ı�opengl�����߼���С�����أ�
    void Resize(int width, int height);

    // ���ǰopengl����
    void Active();

    // ���֡������ʱ���Ƿ񵽴�
    bool CheckFrameRateTime();


    // ����֡��
    void SetFrameRate(int rate = OGL2D_DEFAULT_FRAME_RATE);

    HWND GetHWnd() const;
    HDC GetDC() const;
private:
    System(HWND hWnd);
    virtual ~System();

    struct RenderNode
    {
        Face2d *unit;
        int x, y;
        friend int operator < (RenderNode &n1, RenderNode &n2)
        {
            return *n1.unit < *n2.unit;
        }
    };
       
    // ������Ⱦ����ͨ��Ⱦ��ʹ��GL_BLEND����alpha��ʱ����Ҫ����GL_BLEND
    ClipPass normalPass, alphaPass;

    unsigned int lastTextureId;
    HWND    m_hWnd;
    HDC     m_hDC;
	HGLRC   m_hRC;
    bool    m_bWindowValid;   // �Ƿ񴴽��˴���
    bool    m_bFullScreen;    // ���Ѿ������ˣ���ô�Ƿ���ȫ����

    DWORD m_FrameRateDelay;   // ֡�����Ƶĵȴ�ʱ�䣨��λΪ1/8���룩  
    DWORD m_LastFrameBeginTime; // ���һ֡�Ŀ�ʼʱ�䣨��λΪ1/8���룩
};

inline HWND System::GetHWnd() const 
{
    return m_hWnd;
}
inline HDC System::GetDC() const
{
    return m_hDC;
}



}; // namespace ogl2d
#endif // #ifndef __SYSTEM_H_
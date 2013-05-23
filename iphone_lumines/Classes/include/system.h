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

    // 创建opengl窗口
    bool Create(int width, int height, bool bFullScreen = false);

    // 销毁opengl窗口
    void Release();

    // 开始绘图
    void BeginScene();

    // 结束绘图并输出
    // @param bool  withClear   末尾调用BeginScene
    void Flush(bool withClear = true);

    // 剔除某个图元
    void Pop(Clip *clip, int needAlpha = 0);

    // 添加某个图元
    void Push(Clip *clip, int needAlpha = 0);

    // 改变opengl窗口逻辑大小（象素）
    void Resize(int width, int height);

    // 激活当前opengl窗口
    void Active();

    // 检测帧数限制时间是否到达
    bool CheckFrameRateTime();


    // 设置帧率
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
       
    // 两次渲染，普通渲染不使用GL_BLEND，带alpha的时候需要开启GL_BLEND
    ClipPass normalPass, alphaPass;

    unsigned int lastTextureId;
    HWND    m_hWnd;
    HDC     m_hDC;
	HGLRC   m_hRC;
    bool    m_bWindowValid;   // 是否创建了窗口
    bool    m_bFullScreen;    // 若已经创建了，那么是否是全屏？

    DWORD m_FrameRateDelay;   // 帧数限制的等待时间（单位为1/8毫秒）  
    DWORD m_LastFrameBeginTime; // 最后一帧的开始时间（单位为1/8毫秒）
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
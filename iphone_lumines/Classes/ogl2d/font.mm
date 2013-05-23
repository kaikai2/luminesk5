// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>
#import <cassert>
#import "system.h"
#import "font.h"

namespace ogl2d
{
    Font::Font()
    {

    }
    void Font::init(System *pSys, const char *fontname, unsigned int uID)
    {

        /*LOGFONT lf;
        if (fontname == NULL)
        {
            fontname = "Times New Roman";
        }
        ZeroMemory(&lf, sizeof(lf)); 
        lf.lfHeight = 20; 
        strncpy(lf.lfFaceName, fontname, 32);
        
        switch (uID) 
        { 
        case IDM_BOLD: 
            lf.lfWeight = FW_HEAVY; 
            break; 
            
        case IDM_ITALIC: 
            lf.lfItalic = TRUE; 
            break; 
            
        case IDM_UNDERLINE: 
            lf.lfUnderline = TRUE; 
            break; 
        } 
        HFONT font = CreateFontIndirect(&lf); 
        SelectObject(pSys->GetDC(), font);//GetStockObject(SYSTEM_FIXED_FONT)); 
		DeleteObject(font);
		 */
    }
    Font::~Font()
    {
		
    }
    void Font::textOut(const char *str) const
    {
        assert(str);
        GLsizei len = (GLsizei)strlen(str);
        // now draw the characters in a string 
//        glListBase(base); 
    //    glCallLists(len, GL_BYTE, str); 
    }


struct FontMapNode
{
//    RECT pic;
    int width;
};

    bool FontMap::Build(const char *fontfilename, const char *fontname, 
        unsigned int size, unsigned int uId)
    {
    return false;
    }

}
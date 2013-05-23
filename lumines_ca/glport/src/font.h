#ifndef OGL2D_FONT_H
#define OGL2D_FONT_H

#include <windows.h>
#include <gl/gl.h>

#include <map>
#include <string>

#define IDM_CHARACTER 10 
#define IDM_BOLD      11 
#define IDM_ITALIC    12 
#define IDM_UNDERLINE 13 

namespace glport
{
    class Font
    {
    public:
        Font();
        virtual ~Font();
        void init(HDC dc, const char *fontname, unsigned int uId);
        void textOut(const char *str) const;
        
    private:
        GLuint base;
        GLYPHMETRICSFLOAT agmf[128]; 
    };

    class FontMap
    {
    public:
        FontMap(HDC dc);
        virtual ~FontMap();

        //static bool Build(const char *fontfilename, const char *fontname, unsigned int size, unsigned int uId);

        Font *GetFont(const char *fontfilename);
	private:
		std::map<std::string, Font*> mapFonts;
		HDC dc;
    };
}
#endif

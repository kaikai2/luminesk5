#ifndef OGL2D_FONT_H
#define OGL2D_FONT_H
//#import <ogl2d.h>


#define IDM_CHARACTER 10 
#define IDM_BOLD      11 
#define IDM_ITALIC    12 
#define IDM_UNDERLINE 13 

namespace ogl2d
{
    class Font
    {
    public:
        Font();
        virtual ~Font();
        void init(System *pSys, const char *fontname, unsigned int uId);
        void textOut(const char *str) const;
        
    private:
        GLuint base;
  //      GLYPHMETRICSFLOAT agmf[128]; 

    };

    class FontMap
    {
    public:
        FontMap();
        virtual ~FontMap();

        static bool Build(const char *fontfilename, const char *fontname, unsigned int size, unsigned int uId);

        bool Load(const char *fontfilename);
    };
}
#endif
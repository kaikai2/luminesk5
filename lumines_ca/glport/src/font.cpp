#include "font.h"
#include <cassert>
using std::string;
namespace glport
{
    Font::Font()
    {
        base = glGenLists(128);
    }
    void Font::init(HDC dc, const char *fontname, unsigned int uID)
    {
        //pSys->Active();
        //*
        LOGFONT lf;
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
        SelectObject(dc, font);//GetStockObject(SYSTEM_FIXED_FONT)); 
        if (!wglUseFontOutlines(dc, 0, 128, base, 0.0f, 0.1f,  
            WGL_FONT_LINES, agmf))
        {
            // create the bitmap display lists 
            // we're making images of glyphs 0 thru 255 
            // the display list numbering starts at 1000, an arbitrary choice 
            if (!wglUseFontBitmaps(dc, 0, 128, base))
            {
                MessageBox(0, fontname, "Failed to load font:", MB_OK);
            }
        }
        DeleteObject(font);
    }
    Font::~Font()
    {
        glDeleteLists(base, 128);
    }
    void Font::textOut(const char *str) const
    {
        assert(str);
        GLsizei len = (GLsizei)strlen(str);
        // now draw the characters in a string 
        glListBase(base); 
        glCallLists(len, GL_BYTE, str); 
    }

	struct FontMapNode
	{
		RECT pic;
		int width;
	};

	FontMap::FontMap(HDC _dc) : dc(_dc)
	{
	}
	FontMap::~FontMap()
	{
	}
  //  bool FontMap::Build(const char *fontfilename, const char *fontname, 
  //      unsigned int size, unsigned int uId)
  //  {
		//mapFonts.
		//return false;
  //  }
	Font* FontMap::GetFont(const char *fontfilename)
	{
		if (dc == 0)
		{
			dc = wglGetCurrentDC();
			if (dc == 0)
			{
				return 0;
			}
		}
		string ffn = fontfilename;
		std::map<std::string, Font*>::iterator it = mapFonts.find(ffn);
		if (it != mapFonts.end())
		{
			return it->second;
		}

		Font *f = new Font;
		f->init(dc, fontfilename, 0);
		mapFonts[ffn] = f;
		return f;
	}
}

#import <cstdio>
#import <cassert>
#import <cstring>



#import "pic.h"


namespace ogl2d
{

namespace image
{

typedef struct FileTypeName
{
    char* name;
    Ogl2dPicType type;
}FileTypeName;

FileTypeName s_filetypenames[] = 
{
    "tga", OGL2D_PIC_TGA,
    "jpg", OGL2D_PIC_JPEG,
    "jpeg", OGL2D_PIC_JPEG,
    "bmp", OGL2D_PIC_BMP,
    "gif", OGL2D_PIC_GIF,
    "png", OGL2D_PIC_PNG,
    NULL, OGL2D_PIC_UNKNOWN,
};

Pic::Pic():width(0), height(0), nchannels(0), buf(NULL), type(OGL2D_PIC_UNKNOWN)
{
    
}

Pic::~Pic()
{
    Release();
}

Ogl2dPicType Pic::PictureType(const char* filename)
{
    assert(filename);
    int i;
    // check extend name for default type.
    const char* extname = filename + strlen(filename);
    while (extname > filename && *extname != '.')
        extname--;
    if (*extname != '.')
        return OGL2D_PIC_UNKNOWN;

    extname++;
    for (i=0; s_filetypenames[i].name; i++)
    {
        if (!strcmp(s_filetypenames[i].name, extname))
            return s_filetypenames[i].type;
    }
    
    return OGL2D_PIC_UNKNOWN;
}

void Pic::Load(const char* filename, Ogl2dPicType _type)
{
	CGImageRef spriteImage;
	CGContextRef spriteContext;
	GLubyte *spriteData;
    Release();
    type = _type;
    switch(type)
    {
    case OGL2D_PIC_JPEG:
        assert(0 && "JPEG not surport now");
        break;
    case OGL2D_PIC_BMP:
        assert(0 && "BMP not surport now");
        break;
    case OGL2D_PIC_TGA:
		assert(0 && "TGA not surport now");       
        break;
    case OGL2D_PIC_PNG:
        {
			nchannels=4;
			
			// Creates a Core Graphics image from an image file
			spriteImage = [ UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:filename] ].CGImage;
			// Get the width and height of the image
			width = CGImageGetWidth(spriteImage);
			height = CGImageGetHeight(spriteImage);
			// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
			// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.
			
			if(spriteImage) 
			{
				// Allocated memory needed for the bitmap context
				spriteData = (GLubyte *) malloc(width * height * 4);
				// Uses the bitmatp creation function provided by the Core Graphics framework. 
				spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
				// After you create the context, you can draw the sprite image to the context.
				CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
				// You don't need the context at this point, so you need to release it to avoid memory leaks.
				CGContextRelease(spriteContext);
				buf=spriteData;
			}
            
        }
        break;
    default:
        assert(0 && "unknown format");
        break;
    }
}

void Pic::Release()
{
    width = 0;
    height = 0;
    nchannels = 0;
    switch(type)
    {
    case OGL2D_PIC_JPEG:
        free(buf);
        break;
    case OGL2D_PIC_BMP:
        assert(0 && "BMP not surport now");
        break;
    case OGL2D_PIC_TGA:
        break;
    case OGL2D_PIC_PNG:
        break;
    case OGL2D_PIC_UNKNOWN:
        break;
    default:
        assert(0 && "unknown format");
        break;
    }
}
        
};
};
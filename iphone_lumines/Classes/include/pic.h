#ifndef __PICTURE_H_
#define __PICTURE_H_
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>


enum Ogl2dPicType
{
    OGL2D_PIC_UNKNOWN = 0,
    OGL2D_PIC_JPEG,
    OGL2D_PIC_BMP,
    OGL2D_PIC_TGA,
    OGL2D_PIC_GIF,
    OGL2D_PIC_PNG,
};

namespace ogl2d
{
    namespace image
    {
        
        class Pic
        {
        public:
            Pic();
            virtual ~Pic();
            void Release();

            static Ogl2dPicType PictureType(const char* filename);

            void Load(const char* filename, Ogl2dPicType type);
            
            unsigned short width, height;
            unsigned short nchannels;
            unsigned char *buf;
            Ogl2dPicType type;
        };
        
    };
};
#endif
#ifndef __OGL_2D_
#define __OGL_2D_

// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>
//#import <gl/glut.h>
//#import <gl/glaux.h>

//#pragma comment(lib, "glut.lib")
//#pragma comment(lib, "glaux.lib")
// timeGetTime()

#import "texture.h"
#import "system.h"
#import "anim.h"
#import "pic.h"
#import "font.h"
#ifndef SAFEDELETE
#define SAFEDELETE(a) if((a)==NULL){}else{delete (a); (a)=NULL;}
#endif


#endif // __OGL_2D_
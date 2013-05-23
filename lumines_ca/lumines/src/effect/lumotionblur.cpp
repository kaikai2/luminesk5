#include <glCurvedAni.h>
#include "gl/gl.h"
#include "lumotionblur.h"


void CreateRenderTexture(UINT textureArray[], int size, int channels, int type, int textureID)										
{
	// Create a pointer to store the blank image data
	unsigned int *pTexture = NULL;											

	// Allocate and init memory for the image array and point to it from pTexture
	pTexture = new unsigned int [size * size * channels];
	memset(pTexture, 0, size * size * channels * sizeof(unsigned int));	

	// Register the texture with OpenGL and bind it to the texture ID
	glGenTextures(1, &textureArray[textureID]);								
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);					
	
	// Create the texture and store it on the video card
	glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);						
	
	// Set the texture quality
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Since we stored the texture space with OpenGL, we can delete the image data
	delete [] pTexture;																					
}

LuMotionBlur::LuMotionBlur():tex(NULL)
{
}
void LuMotionBlur::init(glTexture *_tex)
{
    tex = _tex;
}
LuMotionBlur::~LuMotionBlur()
{
}
void LuMotionBlur::step() // call me after all that need to be in blur scene has been drawed.
{
    if (tex == NULL)
        return;

    glPushMatrix();
    glLoadIdentity();
	//Shrink the viewport by the current viewport size.  Hit 1, 2, or 3 to change the size.
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
	//glViewport(0, 0, tex->GetWidth(), tex->GetHeight());								

    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->getGlTexture());
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512,512, 0);
    
	//glViewport(viewport[0], viewport[1],viewport[2],viewport[3]);
    glPopMatrix();
}
void LuMotionBlur::render()
{
    if (tex == NULL)
        return;
	// This function was created to blend the rendered texture over top of the screen
	// to create the recursive blur effect.  What happens is we start out by turning
	// off depth testing, setting our blending mode, then binding the texture of our 
	// rendered textured over the QUAD that is about to be created.  Next, we set our 
	// alpha level to %90 of full strength.  This makes it so it will slowly disappear.
	// Before rendering the QUAD, we want to go into ortho mode.  This makes it easier
	// to render a QUAD over the full screen without having to deal with vertices, but
	// 2D coordinates.  Once we render the QUAD, we want to go back to perspective mode.
	// We can then turn depth testing back on and turn off texturing.

	// Push on a new stack so that we do not interfere with the current matrix
	glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float hw = viewport[2]*.5f;
        float hh = viewport[3]*.5f;
        glTranslatef(hw+0.02f,hh-0.018f,0);
        glScalef(1.015f,1.015f,1);
        static float a = 0, da = 1.0f;
        a += da;
        if (a > 5) da = -1;
        if (a<-5) da = 1;
        glRotatef(a,0,0,1);
        //glScalef(.9,.9,.9);
        // glTranslatef(-hw,-hh,0);
    //*
        glEnable(GL_TEXTURE_2D);
		// Turn off depth testing so that we can blend over the screen
		glDisable(GL_DEPTH_TEST);			

		// Set our blend method and enable blending
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
		glEnable(GL_BLEND);	

		// Bind the rendered texture to our QUAD
		glBindTexture(GL_TEXTURE_2D, tex->getGlTexture());		

		// Decrease the alpha value of the blend by %10 so it will fade nicely
		glColor4f(1, 1, 1, .99f);

		// Switch to 2D mode (Ortho mode)
		// OrthoMode(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		// Display a 2D quad with our blended texture
		glBegin(GL_QUADS);

			// Display the top left point of the 2D image
			glTexCoord2f(0.0f, viewport[3]/512.f);	glVertex2f(-hw, -hh);

			// Display the bottom left point of the 2D image
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(-hw, hh);

			// Display the bottom right point of the 2D image
			glTexCoord2f(viewport[2]/512.f, 0.0f);	glVertex2f(hw, hh);

			// Display the top right point of the 2D image
			glTexCoord2f(viewport[2]/512.f, viewport[3]/512.f);	glVertex2f(hw, -hh);

		// Stop drawing 
		glEnd();
		
		// Let's set our mode back to perspective 3D mode.
		//PerspectiveMode();

		// Turn depth testing back on and texturing off.  If you do NOT do these 2 lines of 
		// code it produces a cool flash effect that might come in handy somewhere down the road.
		//glEnable(GL_DEPTH_TEST);						
		//glDisable(GL_BLEND);		
//*/
	// Go back to our original matrix
	glPopMatrix();
}

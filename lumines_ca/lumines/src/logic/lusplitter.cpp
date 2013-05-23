#include <glCurvedAni.h>
#include "gl/gl.h"
#include "lusplitter.h"
#include "lumines.h"
#include "hand.h"
LuSplitter::LuSplitter():m_Width(0), m_Pos(0), m_pLeftHand(NULL), m_pRightHand(NULL)
{
}
//LuSplitter::~LuSplitter(){}

void LuSplitter::init(Lumines *_lumines, Hand *leftHand, Hand *rightHand, int width, int pos)
{
    m_pLumines = _lumines;
    m_pLeftHand = leftHand;
    m_pRightHand = rightHand;

    m_Width = width;
    m_Pos = pos;
    updateHandLimit();
}
void LuSplitter::updateHandLimit()
{
    m_pLeftHand->setLimit(0, m_Pos);
    m_pRightHand->setLimit(m_Pos, m_Width);
}
void LuSplitter::moveLeft()
{
    if (m_Pos > 4)
    {
        m_Pos--;
        updateHandLimit();
    }
}
void LuSplitter::moveRight()
{
    if (m_Pos < m_Width - 4)
    {
        m_Pos++;
        updateHandLimit();
    }
}

void LuSplitter::render() const
{
    int x = m_Pos * size;
    int height = 10 * size;
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glLineWidth(1.f);

    glColor4f(1,.2f,0,1.0f);
    glBegin(GL_LINES);
    glVertex3d(x-2,0,0.9f);
    glVertex3d(x-2,height+1,0.9f);
    glEnd();
    glColor4f(0,1,.2f,1);
    glBegin(GL_LINES);
    glVertex3d(x+2,0,0.9f);
    glVertex3d(x+2,height+1,0.9f);
    glEnd();

    glPopMatrix();

}

void LuSplitter::step()
{
    int throughCount;
    if (m_pLumines->getScannerX() <= getSplitPos())
    {
        m_lastLeftThroughCount = throughCount = m_pLumines->getThroughCount();
        m_pLeftHand->setThroughCount(throughCount);
    }
    else
    {
        throughCount = m_pLumines->getThroughCount() - m_lastLeftThroughCount;
        m_pRightHand->setThroughCount(throughCount);
    }
}

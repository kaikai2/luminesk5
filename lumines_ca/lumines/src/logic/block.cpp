#include <glCurvedAni.h>
#include "gl/gl.h"
#include <cstdlib>
#include "block.h"
#include "../skin/luskin.h"

using namespace cAni;

extern LuSkin g_luSkin;

const BlockSet s_rotmap[16][4] =
{
    BS_AAAA, BS_AAAA, BS_AAAA, BS_AAAA, 
    BS_AAAB, BS_AABA, BS_BAAA, BS_ABAA,
    BS_AABA, BS_BAAA, BS_ABAA, BS_AAAB,
    BS_AABB, BS_BABA, BS_BBAA, BS_ABAB,
    BS_ABAA, BS_AAAB, BS_AABA, BS_BAAA,
    BS_ABAB, BS_AABB, BS_BABA, BS_BBAA, 
    BS_ABBA, BS_BAAB, BS_ABBA, BS_BAAB, 
    BS_ABBB, BS_BABB, BS_BBBA, BS_BBAB,
    BS_BAAA, BS_ABAA, BS_AAAB, BS_AABA,
    BS_BAAB, BS_ABBA, BS_BAAB, BS_ABBA,
    BS_BABA, BS_BBAA, BS_ABAB, BS_AABB,
    BS_BABB, BS_BBBA, BS_BBAB, BS_ABBB,
    BS_BBAA, BS_ABAB, BS_AABB, BS_BABA,
    BS_BBAB, BS_ABBB, BS_BABB, BS_BBBA,
    BS_BBBA, BS_BBAB, BS_ABBB, BS_BABB, 
    BS_BBBB, BS_BBBB, BS_BBBB, BS_BBBB, 
};

void block::rand()
{
    type = ::rand()%2?BT_A:BT_B;
    special = ::rand()%100 == 0;
    united = false;
    leader = false;
    destory = false;
    flood = false;
    floodleader = false;
}

bool block::step()
{
    bool moved = false;
    if (type == BT_NONE)
        return false;

    int vx, vy;
    if (x != destx)
    {
        vx = min(abs(destx - x), speed);
        if (destx < x)
            x -= vx;
        else
            x += vx;
        moved = true;
    }
    if (y != desty)
    {
        vy = min(abs(desty - y), speed);
        if (desty < y)
            y -= vy;
        else
            y += vy;
        moved = true;
    }
    return moved;
}

void block::render(int pass, const cAni::Rect *clipper) const
{
    if (type == BT_NONE)
        return;
    short size = LUMINES_BLOCK_SIZE + 2;
    short halfsize = size / 2;
    float time = iSystem::GetInstance()->getTime();
    int frameTime = int(time * 60);
    const SkinPack *skin_pack = NULL;

	SkinPack::AnimType at = SkinPack::AT_BlockB;
	if (type == BT_A)
	{
		at = SkinPack::AT_BlockA;
    }
    glClipState cs;
    Rect &tclipper = cs.clip;
    if (clipper != NULL)
    {
        tclipper = *clipper;
        tclipper.leftTop -= cAni::Point2s(x + halfsize, y + halfsize);
        tclipper.rightBottom -= cAni::Point2s(x + halfsize, y + halfsize);
    }
    //else
    //{
    //    tclipper.leftTop = cAni::Point2s(-halfsize, -halfsize);
    //    tclipper.rightBottom = cAni::Point2s(halfsize, halfsize);
    //}
    glPushMatrix();
    //glTranslated(x + halfsize, y + halfsize, 0);
    cs.position = Point2f(x + halfsize, y + halfsize);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    iAnimation *anim = iSystem::GetInstance()->createAnimation();
    if (pass == 0)
    {
        for(int i=0;i<2;i++)
        {
            skin_pack = g_luSkin.getSkin(i==0);
            if (!skin_pack->valid)
                continue;
            anim->setAnimData(skin_pack->anim[at], 0);
            float alpha = g_luSkin.getSkinChangeStep();
            if (i == 1)
            {
                glColor4f(1,1,1,alpha);
                glTranslated(0,0,1);
            }
            else
                glColor4f(1,1,1,1);

            //block_tex->DrawRect(a, tclipper);
            anim->render(frameTime, &cs);
            if (special)
            {
				anim->setAnimData(skin_pack->anim[SkinPack::AT_Special], 0);
                //block_tex->DrawRect(a, tclipper);
                anim->render(frameTime, &cs);
            }
            if (flood && !destory)
            {
				anim->setAnimData(skin_pack->anim[SkinPack::AT_SpecialConnected], 0);
                //block_tex->DrawRect(a, tclipper);
                anim->render(frameTime, &cs);
            }
        }
    }
    else if (pass == 1 && destory)
    {
        for(int i=0;i<2;i++)
        {
            skin_pack = g_luSkin.getSkin(i==0);
            if (!skin_pack->valid)
                continue;
            anim->setAnimData(skin_pack->anim[SkinPack::AT_Activated], 0);
            float alpha = g_luSkin.getSkinChangeStep();
            if (i == 1)
            {
                glColor4f(1,1,1,alpha);
                glTranslated(0,0,1);
            }
            else
                glColor4f(1,1,1,1);
            anim->render(frameTime, &cs);
        }
    }
    iSystem::GetInstance()->release(anim);
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void block::drawleader() const
{
    if (type == BT_NONE )// /*|| !leader*/ || destory)
        return;

    iAnimation *anim = iSystem::GetInstance()->createAnimation();
    anim->setAnimLoop(false);
    float time = iSystem::GetInstance()->getTime();
    int frameTime = int(time * 60) - leaderStartTime;
    short size = (LUMINES_BLOCK_SIZE+1)*2+4;
    short halfsize = size/2 - 1;

	SkinPack::AnimType at = SkinPack::AT_UnionB;
	if (type == BT_A)
	{
		at = SkinPack::AT_UnionA;
	}
    const SkinPack *skin_pack = NULL;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glPushMatrix();
    glTranslated(x + halfsize, y + halfsize, y * 0.51f);
    for(int i=0;i<2;i++)
    {
        skin_pack = g_luSkin.getSkin(i==0);
        if (!skin_pack->valid)
            continue;
        anim->setAnimData(skin_pack->anim[at], 0);
        float alpha = g_luSkin.getSkinChangeStep();
        if (i == 1)
        {
            glColor4f(1,1,1,alpha);
            glTranslated(0,0,1);
        }
        else
            glColor4f(1,1,1,1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        anim->render(frameTime, 0);
    }
    iSystem::GetInstance()->release(anim);
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

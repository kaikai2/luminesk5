

#import "block.h"
#import "luskin.h"




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

void block::render(int pass, const ogl2d::Rect *clipper) const
{
    if (type == BT_NONE)
        return;
    int size = LUMINES_BLOCK_SIZE + 2;
    int halfsize = size / 2;

    const SkinPack *skin_pack = NULL;
    const Texture *block_tex = NULL;

    ogl2d::Rect a;    
    a.left = 0;
    a.right = a.left + size;
    a.top = (type == BT_A)?0:size;
    a.bottom = a.top + size;
    ogl2d::Rect tclipper;
    if (clipper != NULL)
    {
		tclipper=*clipper;

		tclipper.left -= x + halfsize;
        tclipper.right -= x + halfsize;
        tclipper.top -= y + halfsize;
        tclipper.bottom -= y + halfsize;
    }
    else
    {
        tclipper.left = -halfsize;
        tclipper.right = halfsize;
        tclipper.top = -halfsize;
        tclipper.bottom = halfsize;
    }
    glPushMatrix();
    glTranslatef(x + halfsize, y + halfsize, 0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (pass == 0)
    {
        for(int i=0;i<2;i++)
        {
            skin_pack = g_luSkin.getSkin(i==0);
            if (!skin_pack->valid)
                continue;
            block_tex = (const Texture *)skin_pack->block_tex;
            float alpha = g_luSkin.getSkinChangeStep();
            if (i == 1)
            {
                glColor4f(1,1,1,alpha);
                glTranslatef(0,0,1);
            }
            else
                glColor4f(1,1,1,1);
            block_tex->DrawRect(a, tclipper);
            if (special)
            {
                a.top = size*2;
                a.bottom = a.top + size;
                block_tex->DrawRect(a, tclipper);
            }
            if (flood && !destory)
            {
                a.top = 3*size;
                a.bottom = a.top + size;
                block_tex->DrawRect(a, tclipper);
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
            block_tex = skin_pack->block_tex;
            float alpha = g_luSkin.getSkinChangeStep();
            a.top = 4*size;
            a.bottom = a.top + size;
            if (i == 1)
            {
                glColor4f(1,1,1,alpha);
                glTranslatef(0,0,1);
            }
            else
                glColor4f(1,1,1,1);
            block_tex->DrawRect(a, tclipper);
        }
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void block::drawleader() const
{
    if (type == BT_NONE )// /*|| !leader*/ || destory)
        return;
    int size = (LUMINES_BLOCK_SIZE+1)*2+4;
    int halfsize = size/2 - 1;
    ogl2d::Rect a; 
    a.left = 64 - size;
    a.right = a.left + size;
    a.top = (type == BT_A)?0:size;
    a.bottom = a.top + size;
    const SkinPack *skin_pack = NULL;
    const Texture *block_tex = NULL;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glPushMatrix();
    glTranslatef(x + halfsize, y + halfsize, y * 0.51f);
    for(int i=0;i<2;i++)
    {
        skin_pack = g_luSkin.getSkin(i==0);
        if (!skin_pack->valid)
            continue;
        block_tex = skin_pack->block_tex;
        float alpha = g_luSkin.getSkinChangeStep();
        if (i == 1)
        {
            glColor4f(1,1,1,alpha);
            glTranslatef(0,0,1);
        }
        else
            glColor4f(1,1,1,1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        block_tex->DrawRect(a);
    }
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

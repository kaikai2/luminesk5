#pragma warning(disable:4786)
#import <cstdio>
#import "anim.h"

using namespace std;

#define LOCUSANIM_ACCEPTED_VERSION 0x0101

typedef unsigned short WORD;
typedef unsigned long DWORD;

#define MAX_PATH 999


namespace ogl2d
{

struct LocusAni_Head
{
    WORD sig;  // always be 'LA'
    WORD version;
    DWORD anilength;
    char des[256];
    DWORD offset_texture_table;
    DWORD offset_face_table;
    DWORD offset_element_table;
};
struct LocusAni_TextureTable
{
    DWORD size;  // of texname
    char relativePath[MAX_PATH];
    //char texname[1]; // filenames skip by a zero each, with 2 zeros ended
};
struct LocusAni_FaceTable
{
    DWORD count; // of faces
    struct _Face2d
    {
        DWORD texture_id;
        Rect rect;
    }/* faces[1] */;  // dummy struct of faces
};
struct LocusAni_LocusTable
{
    DWORD count; // of locus
    struct _Locus
    {
        enum
        {
            LT_LINEAR_d1 = 0x00,
            LT_B_SPLINE_d1,
            LT_STEP_d1,
            LT_LINEAR_d2 = 0x10,
            LT_B_SPLINE_d2,
            LT_STEP_d2,
            LT_LINEAR_d3 = 0x20,
            LT_B_SPLINE_d3,
            LT_STEP_d3,
            LT_LINEAR_f1 = 0x30,
            LT_B_SPLINE_f1,
            LT_STEP_f1,
            LT_LINEAR_f2 = 0x40,
            LT_B_SPLINE_f2,
            LT_STEP_f2,
            LT_LINEAR_f3 = 0x50,
            LT_B_SPLINE_f3,
            LT_STEP_f3,
        };
        union
        {
            DWORD type;
            struct
            {
                DWORD locusType:4;
                DWORD dataType:12;
            };
        } type;

    };
};
struct LocusAni_ElementTable
{
    DWORD count; // of elements
    struct _ElementHead
    {
        DWORD size;
        enum {
            EH_Locus_Face   = 0x01,
            EH_Locus_Pos    = 0x02,
            EH_Locus_Zoom   = 0x04,
            EH_Locus_Rotate = 0x08,
            EH_Locus_Color  = 0x10,
            EH_Locus_Alpha  = 0x20,
        };
        DWORD flag;
        // locus data go here
    }; // dummy struct of elements
};

static const char *locus_anim_read_text_read_line(FILE *fp)
{
    static char buf[1024];
    while(!feof(fp))
    {
        char *c;
        fgets(buf, sizeof(buf), fp);
        c = buf + strlen(buf);
        if (c == buf)
            continue; // empty line
        c[-1] = 0; // remove the last (and only) '\n' charactor
        for (c = buf; *c; c++) // ltrim
            if (*c != ' ' && *c != '\t')
                break;
        if (*c && *c != '#') 
            return c;
        // skip empty line
    }
    return NULL;
}

// read text formatted AnimData from FILE
bool locus_anim_read_text(FILE *text, TextureResource *res, AnimData **ppAnimData)
{
    bool ret;
    assert(ppAnimData != NULL);
    if (ppAnimData == NULL)
        return false;
    int i,j, faceStartIndex = 0;
    vector< TexturePtr > texs;
    AnimData *pAnimData = new AnimData;
    if (pAnimData == NULL)
        return false;
    assert(pAnimData != NULL);
    pAnimData->m_length = -1;
    
    const char *line;
    int imgcount;
    if ((line = locus_anim_read_text_read_line(text)) == NULL ||
        sscanf(line, "%d", &imgcount) != 1 || 
        imgcount < 0)
    {
        delete pAnimData;
        return false;
    }
    for(j = 0; j < imgcount; j++)
    {
        TexturePtr tex;
        if ((line = locus_anim_read_text_read_line(text)) == NULL ||
            (tex = res->GetTexture(line)) == NULL) // texture will free when TexturePtr has free
        {
            delete pAnimData;
            return false;
        }
        texs.push_back(tex);
    }
    int facecount;
    if ((line = locus_anim_read_text_read_line(text)) == NULL ||
        sscanf(line, "%d", &facecount) != 1 ||
        facecount < 0)
    {
        delete pAnimData;
        return false;
    }

    for(j = 0; j < facecount; j++)
    {
        int left, top, right, bottom;
        int faceid;
        if ((line = locus_anim_read_text_read_line(text)) == NULL ||
            sscanf(line, "%d%d%d%d%d", &faceid, &left, &top, &right, &bottom) != 5 ||
            faceid < 0 || faceid >= imgcount)
        {
            delete pAnimData;
            return false;
        }
        Rect r(left, right, top, bottom);
        Face2d *face = new Face2dRgb(texs[faceid], &r);
        assert(face != NULL);
        pAnimData->m_Faces.push_back(face);
    }

    int elementCount;
    int animLength;
    if ((line = locus_anim_read_text_read_line(text)) == NULL ||
        sscanf(line, "%d%d", &elementCount, &animLength) != 2 ||
        elementCount < 0 || animLength < 0)
    {
        delete pAnimData;
        return false;
    }
    pAnimData->m_length = animLength;

    ret = true;
    for (i = 0; i < elementCount; i++)
    {
        if ((line = locus_anim_read_text_read_line(text)) == NULL)
        {
            delete pAnimData;
            return false;
        }
        char locusbuf[32];
        LocusFace2dPtr face = NULL;
        Locus2fPtr pos = NULL;
        Locus2fPtr zoom = NULL;
        Locus1fPtr rotate = NULL;
        Locus3fPtr color = NULL;
        Locus1fPtr alpha = NULL;
        strncpy(locusbuf, line, sizeof(locusbuf));
        for (const char *c = "FPZRCA"; *c; c++)
        {
            if (strchr(locusbuf, *c) == NULL)
                continue;
            int datacount;
            int maxtime = -1;
            int type;
            if ((line = locus_anim_read_text_read_line(text)) == NULL ||
                sscanf(line, "%d%d", &datacount, &type) != 2 ||
                type < 0 || type > 2)
            {
                ret = false;
                break;
            }
            switch(*c)
            {
            case 'F':
                if (type != 2)
                {
                    ret = false;
                    break;
                }
                face = new LocusFace2dStep;
                assert(face != NULL);
                for(j = 0; j < datacount; j++)
                {
                    int time, faceid;
                    if ((line = locus_anim_read_text_read_line(text)) == NULL ||
                        sscanf(line, "%d%d", &time, &faceid) != 2 ||
                        time < 0 || faceid >= facecount)
                    {
                        ret = false;
                        break;
                    }
                    assert(time >= 0 && faceid < facecount);
                    face->addData(time, pAnimData->m_Faces[faceid]);
                    if (maxtime < time)
                        maxtime = time;
                }
                face->m_time = maxtime;
                break;
            case 'P':
            case 'Z':
                {
                    Locus2fPtr *d = *c == 'P' ? &pos : &zoom;
                    switch(type)
                    {
                    case LT_LINEAR:   /// 线性插值轨迹
                        *d = new LocusLinear2f;
                        break;
                    case LT_B_SPLINE: /// b样条插值轨迹
                        *d = new LocusBspline2f;
                        break;
                    case LT_STEP:
                        *d = new LocusStep2f;
                        break;
                    default:
                        ret = false;
                    }
                    if (!ret)
                        break;
                    for (j = 0; j < datacount; j++)
                    {
                        int time;
                        float x, y;
                        if ((line = locus_anim_read_text_read_line(text)) == NULL ||
                            sscanf(line, "%d%f%f", &time, &x, &y) != 3)
                        {
                            ret = false;
                            break;
                        }
                        (*d)->addData(time, Point2f(x, y));
                        if (maxtime < time)
                            maxtime = time;
                    }
                    if (ret)
                        (*d)->m_time = maxtime;
                }
                break;
            case 'R':
            case 'A':
                {
                    Locus1fPtr *d = *c == 'R' ? &rotate : &alpha;
                    switch(type)
                    {
                    case LT_LINEAR:   /// 线性插值轨迹
                        *d = new LocusLinear1f;
                        break;
                    case LT_B_SPLINE: /// b样条插值轨迹
                        *d = new LocusBspline1f;
                        break;
                    case LT_STEP:
                        *d = new LocusStep1f;
                        break;
                    default:
                        ret = false;
                    }
                    if (!ret)
                        break;
                    for (j = 0; j < datacount; j++)
                    {
                        int time;
                        float x;
                        if ((line = locus_anim_read_text_read_line(text)) == NULL ||
                            sscanf(line, "%d%f", &time, &x) != 2)
                        {
                            ret = false;
                            break;
                        }
                        (*d)->addData(time, x);
                        if (maxtime < time)
                            maxtime = time;
                    }
                    if (ret)
                        (*d)->m_time = maxtime;
                }
                break;
            case 'C':
                switch(type)
                {
                case LT_LINEAR:   /// 线性插值轨迹
                   color = new LocusLinear3f;
                    break;
                case LT_B_SPLINE: /// b样条插值轨迹
                    color = new LocusBspline3f;
                    break;
                case LT_STEP:
                    color = new LocusStep3f;
                    break;
                default:
                    ret = false;
                }
                if (!ret)
                    break;
                for (j = 0; j < datacount; j++)
                {
                    int time;
                    float x, y, z;
                    if ((line = locus_anim_read_text_read_line(text)) == NULL ||
                        sscanf(line, "%d%f%f%f", &time, &x, &y, &z) != 4)
                    {
                        ret = false;
                        break;
                    }
                    color->addData(time, Point3f(x, y, z));
                    if (maxtime < time)
                        maxtime = time;
                }
                if (ret)
                    color->m_time = maxtime;
                break;
            }
            // if (pAnimData->m_length < maxtime) pAnimData->m_length = maxtime;
        }
        if (!ret)
        {
            delete face;
            delete pos;
            delete zoom;
            delete rotate;
            delete color;
            delete alpha;
            delete pAnimData;
            return false;
        }
        pAnimData->m_AnimElements.push_back(AnimElement(face, pos, zoom, rotate, color, alpha));
    }
    *ppAnimData = pAnimData;
    return true;
}
bool locus_anim_write_text(FILE *text, const TextureResource *res, const AnimData *ppAnimData)
{
    return false;
}

void locus_anim_free(AnimData *&pAnimData)
{
    delete pAnimData;
    pAnimData = NULL;
}
#ifdef SDL_H
// read binary formatted AnimData from sdl_rwops
bool locus_anim_read(SDL_RWops *sdl_rwops, TextureResource *res, AnimData **ppAnimData)
{
    assert(ppAnimData != NULL);
    if (ppAnimData == NULL)
        return false;
    LocusAni_Head head;
    SDL_RWseek(sdl_rwops, 0, SEEK_SET);
    SDL_RWread(sdl_rwops, &head, sizeof(head), 1);
    if (head.sig != 'LA')
        return false;
    if (head.version != LOCUSANIM_ACCEPTED_VERSION)
        return false;

    DWORD size, i;

    // load the textures by TextureTable
    vector< TexturePtr > textures;
    SDL_RWseek(sdl_rwops, head.offset_texture_table, SEEK_SET);
    SDL_RWread(sdl_rwops, &size, sizeof(DWORD), 1);
    char *buf = new char[size];
    LocusAni_TextureTable textureTable;
    SDL_RWread(sdl_rwops, &textureTable.relativePath, sizeof(textureTable.relativePath), 1);
    SDL_RWread(sdl_rwops, buf, sizeof(char), size);
    for (i = 0; i < size && buf[i]; i++)
    {
        int len = strlen(buf + i);
        string filename = textureTable.relativePath;
        filename += buf + i;
        TexturePtr tex = res->GetTexture(filename.c_str());
        if (tex == NULL)
        {
            delete [] buf;
            return false;
        }
        textures.push_back(tex);
        i += len;
    }
    delete [] buf;

    // load the faces by FaceTable
    AnimData *anim = new AnimData;
    vector< Face2d* > &faces = anim->m_Faces;
    SDL_RWseek(sdl_rwops, head.offset_face_table, SEEK_SET);
    SDL_RWread(sdl_rwops, &size, sizeof(DWORD), 1);
    for (i = 0; i < size; i++)
    {
        LocusAni_FaceTable::_Face2d facedata;
        SDL_RWread(sdl_rwops, &facedata, sizeof(facedata), 1);
        if (facedata.texture_id < textures.size())
        {
            faces.push_back(new Face2dRgba(textures[facedata.texture_id], &facedata.rect));
        }
        else
        {
            /* this has been done in anim's detor
            for(Face2d **face = faces.begin(); face != faces.end(); ++face)
                delete *face;
            */
            delete anim;
            return false;
        }
    }

    // load the locus data by LocusTable
    SDL_RWread(sdl_rwops, &size, sizeof(DWORD), 1);
    for(i = 0; i < size; i++)
    {
        DWORD flag;
        DWORD nextpos;
        SDL_RWread(sdl_rwops, &nextpos, sizeof(DWORD), 1);
        SDL_RWread(sdl_rwops, &flag, sizeof(DWORD), 1);

        LocusFace2dPtr face = NULL;
        Locus2fPtr pos = NULL;
        Locus2fPtr zoom = NULL;
        Locus1fPtr rotate = NULL;
        Locus3fPtr color = NULL;
        Locus1fPtr alpha = NULL;
        if (flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Face)
        {
            Locus< int > *temp = GetLocus(sdl_rwops, int(0));
            switch(temp->getLocusType())
            {
            //case LT_LINEAR:face = new LocusFace2dLinear;break;
            case LT_STEP:
                face = new LocusFace2dStep;
                break;
            }
            if (face == NULL)
            {
                assert(0);
                delete temp;
                break;
            }
            for(int i=0;;i++)
            {
                int time, id;
                if (!temp->getData(i, time, id))
                    break;
                if (id < 0 || id >= (int)faces.size())
                    break;
                face->addData(time, faces[id]);
            }
            delete temp;
        }
        flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Pos && (pos = GetLocus(sdl_rwops, Point2f()));
        flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Zoom && (zoom = GetLocus(sdl_rwops, Point2f()));
        flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Rotate && (rotate = GetLocus(sdl_rwops, float(0)));
        flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Color && (color = GetLocus(sdl_rwops, Point3f()));
        flag & LocusAni_ElementTable::_ElementHead::EH_Locus_Alpha && (alpha = GetLocus(sdl_rwops, float(0)));

        anim->m_AnimElements.push_back(AnimElement(face, pos, zoom, rotate, color, alpha));

        //m_AnimElements.push_back(anim);
        SDL_RWseek(sdl_rwops, nextpos, SEEK_SET);
    }
    if (i < size)
    {
        delete anim;
        return false;
    }
    anim->m_length = head.anilength;
    *ppAnimData = anim;
    return true;
}

bool locus_anim_write(SDL_RWops *sdl_rwops, TextureResource *res,  const AnimData *pAnimData)
{
    int i;
    assert(pAnimData != NULL);
    if (pAnimData == NULL)
        return false;
    LocusAni_Head head;
    SDL_RWseek(sdl_rwops, 0, SEEK_SET);
    
    // first time write, data not finished
    head.sig = 0;
    head.version = 0;
    SDL_RWwrite(sdl_rwops, &head, sizeof(head), 1); 
    
    head.sig = 'LA';
    head.version = LOCUSANIM_ACCEPTED_VERSION;
    head.offset_texture_table = SDL_RWtell(sdl_rwops);
    head.anilength = pAnimData->m_length;
    // fetch all filenames
    vector< string > filenames;
    vector< Face2d* >::const_iterator face;
    for (face = pAnimData->m_Faces.begin();
        face != pAnimData->m_Faces.end(); ++face)
    {
        int c = (*face)->getTextureCount();
        if (c != 1)
        {
            assert(c == 1); // this version only accept single textures
            SDL_RWseek(sdl_rwops, 0, SEEK_SET);
            return false;
        }
        for(i = 0; i < c; i++)
        {
            const char *filename = res->GetFileName((*face)->getTexture(i));
            assert(filename != NULL);
            filenames.push_back(string(filename));
        }
    }

    // sort and remove duplicated filenames
    sort(filenames.begin(), filenames.end());
    string lastfilename;
    vector< string >::iterator fname;
    for (fname = filenames.begin(); fname != filenames.end();)
    {
        if (lastfilename == *fname)
        {
            lastfilename = *fname;
            fname = filenames.erase(fname);
        }
        else
            lastfilename = *fname++;
    }

    // compare filenames and get their common pathname
    string pathname;
    for (fname = filenames.begin(); fname != filenames.end(); ++fname)
    {
        if (pathname.size() == 0)
        {
            // make sure at least ONE byte of the file name will be in the texture table
            pathname = fname->substr(0, fname->length() - 1);
        }
        else
        {
            string::iterator ch;
            const char *filename = fname->c_str();
            for (ch = pathname.begin(); ch != pathname.end() && *filename; ++ch, ++filename)
            {
                if (*ch != *filename)
                    break;
            }
            pathname.erase(ch, pathname.end());
            if (pathname.size() == 0)
                break;
        }
    }
    LocusAni_TextureTable textureTable;
    if (pathname.size() >= sizeof(textureTable.relativePath) - 1)
    {
        SDL_RWseek(sdl_rwops, 0, SEEK_SET);
        return false;
    }
    textureTable.size = 0;
    // save the textures by TextureTable
    vector< TexturePtr > textures;
    int pos = SDL_RWtell(sdl_rwops);
    SDL_RWwrite(sdl_rwops, &textureTable, sizeof(LocusAni_TextureTable), 1);
    for (fname = filenames.begin(); fname != filenames.end(); ++fname)
    {
        int len = fname->size() + 1 - pathname.size();
        SDL_RWwrite(sdl_rwops, fname->c_str() + pathname.size(), sizeof(char), len);
        textureTable.size += len;
    }
    const char tail_zero = 0;
    SDL_RWwrite(sdl_rwops, &tail_zero, sizeof(char), 1);
    head.offset_face_table = SDL_RWtell(sdl_rwops);

    // seek back and write the texture table data
    SDL_RWseek(sdl_rwops, pos, SEEK_SET);
    strncpy(textureTable.relativePath, pathname.c_str(), sizeof(textureTable.relativePath));
    SDL_RWwrite(sdl_rwops, &textureTable, sizeof(LocusAni_TextureTable), 1);

    // seek to face table
    SDL_RWseek(sdl_rwops, head.offset_face_table, SEEK_SET);

    // write face count, and all faces' data
    DWORD faceCount = pAnimData->m_Faces.size();
    SDL_RWwrite(sdl_rwops, &faceCount, sizeof(DWORD), 1);
    for (face = pAnimData->m_Faces.begin();
        face != pAnimData->m_Faces.end(); ++face)
    {
        int c = (*face)->getTextureCount();
        for(i = 0; i < c; i++)
        {
            const char *filename = res->GetFileName((*face)->getTexture(i));
            vector< string >::const_iterator file = find(filenames.begin(), filenames.end(), filename);
            if (file == filenames.end())
            {
                SDL_RWseek(sdl_rwops, 0, SEEK_SET);
                return false;
            }
            DWORD texid = file - filenames.begin();
            SDL_RWwrite(sdl_rwops, &texid, sizeof(DWORD), 1);
            SDL_RWwrite(sdl_rwops, &(*face)->getRect(i), sizeof(Rect), 1);
        }
    }

    // save the file-pos of elemete table 
    head.offset_element_table = SDL_RWtell(sdl_rwops);
    DWORD elementCount = pAnimData->m_AnimElements.size();
    SDL_RWwrite(sdl_rwops, &elementCount, sizeof(DWORD), 1);
    DWORD nextpos = SDL_RWtell(sdl_rwops);
    for (vector< AnimElement >::const_iterator element = pAnimData->m_AnimElements.begin();
        element != pAnimData->m_AnimElements.end(); ++element)
    {
        DWORD beginpos = nextpos;
        SDL_RWwrite(sdl_rwops, &nextpos, sizeof(DWORD), 1); // write a dword
        DWORD flag = 0;
        element->m_pFace2d && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Face);
        element->m_pPos && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Pos);
        element->m_pZoom && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Zoom);
        element->m_pRot && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Rotate);
        element->m_pColor && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Color);
        element->m_pAlpha && (flag |= LocusAni_ElementTable::_ElementHead::EH_Locus_Alpha);
        SDL_RWwrite(sdl_rwops, &flag, sizeof(DWORD), 1); // write flag

        // write locus data here
        if (element->m_pFace2d != NULL)
        {
            LocusStep< int > temp;
            for (int i = 0; ; i++)
            {
                int time;
                Face2d *face;
                if (!element->m_pFace2d->getData(i, time, face))
                    break;
                vector< Face2d* >::const_iterator iface = find(pAnimData->m_Faces.begin(), pAnimData->m_Faces.end(), face);
                if (iface == pAnimData->m_Faces.end())
                {
                    SDL_RWseek(sdl_rwops, 0, SEEK_SET);
                    return false;
                }
                vector< int >::size_type size = iface - pAnimData->m_Faces.begin();
                temp.addData(time, size);
            }
            PutLocus(sdl_rwops, &temp);
        }
        element->m_pPos && PutLocus(sdl_rwops, element->m_pPos);
        element->m_pZoom && PutLocus(sdl_rwops, element->m_pZoom);
        element->m_pRot && PutLocus(sdl_rwops, element->m_pRot);
        element->m_pColor && PutLocus(sdl_rwops, element->m_pColor);
        element->m_pAlpha && PutLocus(sdl_rwops, element->m_pAlpha);

        // set next file pos
        nextpos = SDL_RWtell(sdl_rwops);
        SDL_RWseek(sdl_rwops, beginpos, SEEK_SET); // go back to the dword
        SDL_RWwrite(sdl_rwops, &nextpos, sizeof(DWORD), 1); // and write the nextPos
        SDL_RWseek(sdl_rwops, nextpos, SEEK_SET); // go forward
    }

    // finally write the head
    SDL_RWseek(sdl_rwops, 0, SEEK_SET);
    SDL_RWwrite(sdl_rwops, &head, sizeof(head), 1);
    SDL_RWseek(sdl_rwops, nextpos, SEEK_SET);
    return true;
}

#endif // SDL_H
TextureResource s_AnimTextureRes;

}; // namespace ogl2d
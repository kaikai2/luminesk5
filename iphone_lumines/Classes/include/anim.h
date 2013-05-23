#ifndef __ANIM_H_
#define __ANIM_H_

#import <vector>
#import <cassert>
#import <string>

#import "clip.h"
#import "locus.h"
// #import "point1d.h" it is no need, float is ok
#import "point2d.h"
#import "point3d.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace ogl2d
{
#pragma warning(push)
#pragma warning(disable:4786)

typedef Locus< float > Locus1f;
typedef Locus1f* Locus1fPtr;
typedef LocusLinear< float > LocusLinear1f;
typedef LocusBspline< float > LocusBspline1f;
typedef LocusStep< float > LocusStep1f;

typedef Locus< Point2f > Locus2f;
typedef Locus2f* Locus2fPtr;
typedef LocusLinear< Point2f > LocusLinear2f;
typedef LocusBspline< Point2f > LocusBspline2f;
typedef LocusStep< Point2f > LocusStep2f;

typedef Locus< Point3f > Locus3f;
typedef Locus3f* Locus3fPtr;
typedef LocusLinear< Point3f > LocusLinear3f;
typedef LocusBspline< Point3f > LocusBspline3f;
typedef LocusStep< Point3f > LocusStep3f;
#pragma warning(pop)
/**
* Since Face2d is a part of anim, why not just add it as a locus ?
*/
typedef Locus< Face2d* > LocusFace2d;
typedef LocusFace2d* LocusFace2dPtr;
typedef LocusStep< Face2d* > LocusFace2dStep;     // Simple clip change
typedef LocusLinear< Face2d* > LocusFace2dLinear; // Linear-blended clip change
// no need to use bspline in locusFace2d, the effect is very similar to linear ones


/// AnimElement
/// @note smallest single part of an animation
struct AnimElement
{
    AnimElement(LocusFace2dPtr face, 
        Locus2fPtr pos = NULL, Locus2fPtr zoom = NULL, Locus1fPtr rotate = NULL,
        Locus3fPtr color = NULL, Locus1fPtr alpha = NULL);

    void render(int time, const Rect *cliprect = NULL) const;
    enum eIndex{
        Face,
        Pos,
        Zoom,
        Rot,
        Color,
        Alpha,
    };
    LocusFace2dPtr m_pFace2d;
    Locus2fPtr m_pPos;
    Locus2fPtr m_pZoom;
    Locus1fPtr m_pRot;
    Locus3fPtr m_pColor;
    Locus1fPtr m_pAlpha;
    int m_LocusStartTime[6]; // elements may have the same locus only differs in the start time
    //int m_time;   // anim length
};

/// AnimData 
/// @note set of AnimElements and the total info of the animation
class AnimData 
{
public:
    friend class Animation;

    /// dtor, faces need to be released
    virtual ~AnimData();

    /// render
    /// @param  time    [in] frame time
    /// @param  cliprect [in] clipper rectangle (not implemented yet)
    void render(int time, const Rect *cliprect = NULL) const;

    ::std::vector< AnimElement > m_AnimElements;
    ::std::vector< Face2d* > m_Faces;
    ::std::vector< LocusBase * > m_Locus;
    int m_length;
};

/// Animation
/// @note playing instance of the AnimData
class Animation
{
public:
    typedef ::std::vector<const AnimData *>::size_type AnimId;
    enum
    {
        DefaultAnimId = ~0,
    };
    Animation(AnimId animIdCount = 1)
    {
        init(animIdCount);
    }
    void setAnimData(const AnimData *pAnimData, AnimId aniId);
    bool startAnim(int curtime, AnimId aniId = DefaultAnimId);

    bool checkEnd(int curtime) const;
    void setAnimLoop(bool bLoop);
    AnimId getCurAnim() const{return m_CurAnimId;}
   // void setAnimTimer(int time);
    
    void render(int time, const Rect *cliprect) const;

    void setPos(float x, float y)
    {
        m_Pos.x = x;
        m_Pos.y = y;
    }
    void getPos(float &x, float &y) const
    {
        x = m_Pos.x;
        y = m_Pos.y;
    }
protected:
    void init(AnimId animIdCount);
    const AnimData *getCurAnimData() const
    {
        assert(m_CurAnimId == DefaultAnimId || m_CurAnimId < m_AnimDatas.size());
        return m_CurAnimId == DefaultAnimId ? NULL : m_AnimDatas[m_CurAnimId];
    }

    AnimId m_CurAnimId;
    ::std::vector<const AnimData *> m_AnimDatas;

    int m_startTime;    // start time in game timing
    Point2f m_Pos;      // anim position 

    // int m_curTime;  // current time in animation timing
    bool m_play;
    bool m_loop;  // is loop ?
};

/// AnimResManager
/// @note   manage AnimData as resources
class AnimResManager
{
public:
    AnimResManager(TextureResource *pTexRes);
    virtual ~AnimResManager();

    /// getAnimData
    /// @param aniFileName  [in] main file name of ani file, no suffix needed
    const AnimData* getAnimData(const char *aniFileName);
private:
    struct Pair
    {
        Pair(AnimData *_pAnimData, const char *_anifilename)
            :pAnimData(_pAnimData),anifilename(_anifilename)
        {
        }
        AnimData* pAnimData;
        ::std::string anifilename;
        bool operator == (const char*name) const
        {
            return anifilename == name;
        }
    };
    ::std::vector< Pair > m_AnimDatas;
    TextureResource * const m_pTexRes;
};

// read text formatted AnimData from FILE
bool locus_anim_read_text(FILE *text, TextureResource *res, AnimData **ppAnimData);
bool locus_anim_write_text(FILE *text, const TextureResource *res, const AnimData *ppAnimData);
void locus_anim_free(AnimData *&pAnimData);
/*
// read binary formatted AnimData from sdl_rwops
bool locus_anim_read(SDL_RWops *sdl_rwops, TextureResource *res, AnimData **ppAnimData);
bool locus_anim_write(SDL_RWops *sdl_rwops, TextureResource *res, const AnimData *pAnimData);
*/

}; // namespace ogl2d

#endif __ANIM_H_
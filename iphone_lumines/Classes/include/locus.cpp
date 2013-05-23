/*
NOTICE: This file do be under the %ogl2dlib-sdl%/import directory.
 it may be importd by the locus.h file when 'export' is not a keyword
 in your compiler
*/
#ifndef LOCUS_CPP
#define LOCUS_CPP

#import "locus.h"

#import "bspline.h"
#import "bitstream.h"

#ifndef USE_EXPORT_KEYWORD
   #define export /*nothing*/
#else
template class Locus<int>;
template class LocusLinear<int>;
template class LocusBspline<int>;
template class LocusStep<int>;
#endif

/*
    export template functions' implementations go in the same file 
    where they have been declared.
*/
namespace ogl2d
{

export template< class Point >
void Locus< Point >::addData(int time, const Point &p)
{
    m_data.push_back(Node(time, p));
    m_time < time && (m_time = time);
    m_bNeedSort = true;
}

export template< class Point >
bool Locus< Point >::getData(unsigned int index, int &time, Point &result)
{
    if (m_data.size() <= index)
        return false;
    
    if (m_bNeedSort)
    {
        ::std::sort(m_data.begin(), m_data.end());
        m_bNeedSort = false;
    }

    time = m_data[index].m_time;
    result = m_data[index].m_p;
    return true;
}

export template< class Point >
bool Locus< Point >::setData(unsigned int index, int time, const Point &p)
{
    if (index >= m_data.size())
        return false;

    if (!m_bNeedSort)
    {
        if (index > 0 && m_data[index-1].time >= time ||
            index < m_data.size() - 1 && m_data[index+1].time <= time)
        {
            m_bNeedSort = true;
        }
    }
    m_data[index].time = time;
    m_data[index].m_p = p;

    m_time < time && (m_time = time);
    return true;
}

export template< class Point >
Locus< Point >::Locus():LocusBase()
{
}

export template< class Point >
void Locus< Point >::loadData(const char *data)
{
    assert(data);
    //zhu assert(getLocusType() == *(int *)data);
    data += sizeof(int);
    m_time = *(int *)data;
    data += sizeof(int);
    unsigned int size = *(int *)data;
    data += sizeof(int);
    assert(size <= LOCUS_MAX_NODE_COUNT);
    Node node;
    m_data.assign(size, node);
    m_data.clear();

    while(size--)
    {
        node = *(Node *)data;
        data += sizeof(Node);
        m_data.push_back(node);
    }
    if (!m_data.empty())
    {
        ::std::sort(m_data.begin(), m_data.end());
        m_time = m_data.back().m_time;
    }
    m_bNeedSort = false;
}

export template< class Point >
void Locus< Point >::loadData(bitstream *bs)
{
    LocusType type;
    assert(bs);
    U32 size;
    bs->read(&type);
    bs->read(&m_time);
    assert(type == getLocusType());
    bs->read(&size);
    assert(size <= LOCUS_MAX_NODE_COUNT);
    m_data.clear();
    
    Node node;
    while(size--)
    {
        bs->read(&node);
        m_data.push_back(node);
    }
    if (!m_data.empty())
    {
        ::std::sort(m_data.begin(), m_data.end());
        m_time = m_data.back().m_time;
    }
    m_bNeedSort = false;
}


	export template< class Point >
	LocusLinear< Point >::LocusLinear():Locus< Point >()
{
}
 
export template< class Point >
int LocusLinear< Point >::getInterval(int time, Point &result)
{
	//this->m_data.size()=0;
    if (time >= this->m_time || time < 0 || this->m_data.size() == 0)
        return false;

    if (this->m_bNeedSort)
    {
        ::std::sort(this->m_data.begin(), this->m_data.end());
        this->m_bNeedSort = false;
    }

    if (time <= this->m_data.front().m_time) // before first control point
    {
        result = this->m_data.front().m_p;
        return true;
    }
    if (time >= this->m_data.back().m_time) // after last control point
    {
        result = this->m_data.back().m_p;
        return true;
    }
    
    //typename ::std::vector < Node >::iterator left = lower_bound(this->m_data.begin() + 1,this-> m_data.end(), time);
    //if (left == this->m_data.end())
    //    return false;

    //int delta = left->m_time - left[-1].m_time;
   // result = left[-1].m_p;

   // result += Point((left->m_p - left[-1].m_p) * ((float)(time - left[-1].m_time) / delta));

    return true;
}

export template< class Point >
LocusBspline< Point >::LocusBspline():Locus< Point >()
{
}

export template< class Point >
int LocusBspline< Point >::getInterval(int time, Point &result)
{
    if (time >= this-> m_time || time < 0 ||this-> m_data.size() < 4)
        return false;

    if (this-> m_bNeedSort)
    {
		::std::sort(this->m_data.begin(), this->m_data.end());
        this->m_bNeedSort = false;
    }

    typename ::std:: vector< Node >::iterator left;
    if (time <= this->m_data.front().m_time) // before first control point
    {
    //    left =this-> m_data.begin() + 1;
        time = this->m_data.front().m_time;
    }
    else if (time >= (this->m_data.end() - 2)->m_time) // after last control point
    {
     //   left = this->m_data.end() - 2;
        time = (this->m_data.end() - 2)->m_time;
    }
    else
    {
     //   left = ::std::lower_bound(this->m_data.begin(), this->m_data.end(), time);
     //   if (left > (this->m_data.end() - 2))
     //       return false;
    }

    float dd = (float)left[-1].m_time;
    dd = (time - dd) / (left->m_time - dd);

    Point d[4];
    d[0] = (left-1)->m_p;
    d[1] = left++->m_p;
    d[2] = left++->m_p;
    d[3] = left->m_p;

    bspline_d(dd); // calculate g_dd[4]
    result = Point(d[0] * g_dd[0] + d[1] * g_dd[1] + d[2] * g_dd[2] + d[3] * g_dd[3]);
    return true;
}

export template< class Point >
LocusStep< Point >::LocusStep():Locus< Point >()
{
}
export template< class Point >
	int LocusStep< Point >::getInterval(int time, Point &result)
	{
    if (time >= this->m_time || time < 0 || this->m_data.size() == 0)
        return false;

    if (this->m_bNeedSort)
    {
        ::std::sort(this->m_data.begin(), this->m_data.end());
        this->m_bNeedSort = false;
    }

    if (time <= this->m_data.front().m_time) // before first control point
    {
        result =this-> m_data.front().m_p;
        return true;
    }
    if (time >= this->m_data.back().m_time) // after last control point
    {
        result = this->m_data.back().m_p;
        return true;
    }
    //typename ::std:: vector< Node >::iterator left = ::std::lower_bound(this->m_data.begin(), this->m_data.end(), time);
    //if (left ==this-> m_data.end())
    //    return false;

    //result = left->m_p;
    return true;
}


#ifdef SDL_H
export template< class Point >
void Locus< Point >::loadData(SDL_RWops *sdl_rwops)
{
    LocusType type;
    assert(sdl_rwops);
    U32 size;
    SDL_RWread(sdl_rwops, &type, sizeof(type), 1);
    assert(type == getLocusType());
    SDL_RWread(sdl_rwops, &m_time, sizeof(m_time), 1);
    SDL_RWread(sdl_rwops, &size, sizeof(size), 1);
    assert(size <= LOCUS_MAX_NODE_COUNT);
    m_data.clear();
    
    Node node;
    while(size--)
    {
        SDL_RWread(sdl_rwops, &node, sizeof(node), 1);
        m_data.push_back(node);
    }
    if (!m_data.empty())
    {
        ::std::sort(m_data.begin(), m_data.end());
        m_time = m_data.back().m_time;
    }
    m_bNeedSort = false;
}

export template< class Point >
void Locus< Point >::saveData(SDL_RWops *sdl_rwops)
{
    LocusType type = getLocusType();
    SDL_RWwrite(sdl_rwops, &type, sizeof(type), 1);
    SDL_RWwrite(sdl_rwops, &m_time, sizeof(m_time), 1);
    U32 size = m_data.size();
    SDL_RWwrite(sdl_rwops, &size, sizeof(size), 1);
    
    for (::std::vector< Node >::const_iterator node = m_data.begin();
        node != m_data.end(); ++node)
    {
        SDL_RWwrite(sdl_rwops, node, sizeof(Node), 1);
    }
}
export template< class Point >
Locus< Point >* GetLocus(SDL_RWops *sdl_rwops, Point)
{
    Locus< Point >* locus = NULL;
    LocusType type;
    SDL_RWread(sdl_rwops, &type, sizeof(DWORD), 1);
    SDL_RWseek(sdl_rwops, -(int)sizeof(DWORD), SEEK_CUR);
    switch(type)
    { 
    case LT_LINEAR:
        locus = new LocusLinear< Point >;
        locus->loadData(sdl_rwops);
        break;
    case LT_B_SPLINE:
        locus = new LocusBspline< Point >;
        locus->loadData(sdl_rwops);
        break;
    case LT_STEP:
        locus = new LocusStep< Point >;
        locus->loadData(sdl_rwops);
        break;
    }
    return locus;
}

export template< class Point >
bool PutLocus(SDL_RWops *sdl_rwops, Locus< Point >* locus)
{
    locus->saveData(sdl_rwops);
    return true;
}
#endif

} // namespace ogl2d

#endif // LOCUS_CPP
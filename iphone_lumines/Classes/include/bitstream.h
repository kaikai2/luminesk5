#ifndef __BIT_STREAM_H_
#define __BIT_STREAM_H_

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



namespace ogl2d
{
    
    typedef unsigned char U8;
    typedef signed char S8;
    typedef unsigned short U16;
    typedef signed short S16;
    typedef unsigned long U32;
    typedef signed long S32;
    #if 0
    #ifdef _MSVC
    typedef __int64 S64;
    #else
    typedef long long S64;
    #endif
    #endif    
    typedef float F32;
    typedef double F64;
    typedef long double F80;
    
    /// bitstream
    /// @note   ±ÈÌØÁ÷»º³å
    class bitstream
    {
    public:
        
        template< class Type > bool write(const Type& var)
        {
            // assert(mBuffer);
            if (mBuffer == NULL)
                return false;
            
            // assert(mCurPos < mLength);
            if (mCurPos >= mLength)
                return false;
            
            U32 len = min(sizeof(Type), mLength - mCurPos);
            memcpy(mBuffer + mCurPos, (void*)&var, len);
            mCurPos += len;
            
            return true;
        }
        
        template< class Type > bool read(Type* var)
        {
            // assert(mBuffer);
            if (mBuffer == NULL)
                return false;
            
            // assert(mCurPos <= mLength);
            if (mCurPos >= mLength)
                return false;
            
            U32 len = min(sizeof(Type), mLength - mCurPos);
            memcpy((void*)var, mBuffer + mCurPos, len); 
            mCurPos += len;
            
            return true;
        }
        
        /// turn to head
        void rewind();
        
        /// turn to postion
        void seek(U32 pos);
        
        bitstream(U32 size);
        virtual ~bitstream();
    protected:
        U8* mBuffer;
        U32 mLength;
        U32 mCurPos;
    };
    
    inline void bitstream::rewind()
    {
        seek(0);
    }
    
    inline void bitstream::seek(U32 pos)
    {
        if (pos > mLength)
            mCurPos = mLength;
        else
            mCurPos = pos;
    }
    
} // ogl2d
#endif // __BIT_STREAM_H_

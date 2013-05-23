#import "bitstream.h"

namespace ogl2d
{

bitstream::bitstream(U32 size):mLength(size)
{
    mBuffer = new U8[size];
    rewind();
}

bitstream::~bitstream()
{
    delete [] mBuffer;
}

} // namespace ogl2d
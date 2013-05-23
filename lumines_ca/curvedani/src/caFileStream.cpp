#include <cassert>
#include <cstdio>
#include "caFileStream.h"

namespace cAni
{
    FileStream::~FileStream()
    {
        if (fp)
        {
            fclose((FILE*)fp);
        }
    }

    bool FileStream::openFile(const char *filename, const char *mode)
    {
        if (fp)
        {
            fclose((FILE*)fp);
        }
        fp = fopen(filename, mode);
        return fp != 0;
    }
    bool FileStream::write(const void *buf, size_t len)
    {
        return fp && fwrite(buf, len, 1, (FILE*)fp) == 1;
    }
    bool FileStream::read(void *buf, size_t len)
    {
        return fp && fread(buf, len, 1, (FILE*)fp) == 1;
    }
    /// seek to position
    void FileStream::seek(size_t pos)
    {
        assert(fp);
        if (0 != fseek((FILE*)fp, (long)pos, SEEK_SET))
            assert(0);
    }
    /// seek with an offset
    void FileStream::seekOffset(int offset)
    {
        assert(fp);
        if (0 != fseek((FILE*)fp, (long)offset, SEEK_CUR))
            assert(0);
    }
    size_t FileStream::length() const
    {
        assert(fp);
        long pos = ftell((FILE*)fp);
        fseek((FILE*)fp, 0, SEEK_END);
        long endpos = ftell((FILE*)fp);
        fseek((FILE*)fp, pos, SEEK_SET);
        return size_t(endpos);
    }
    size_t FileStream::tell() const
    {
        assert(fp);
        return ftell((FILE*)fp);
    }
}

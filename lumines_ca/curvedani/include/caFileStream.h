#ifndef CANI_FILESTREAM_H
#define CANI_FILESTREAM_H

#include "curvedani.h"
namespace cAni
{

    /// FileStream �ļ���,�ֽ�����һ�ּ�ʵ��
    /// ֱ��ʹ��ϵͳ�ļ���д�ӿ�
    class FileStream : public iByteStream
    {
    public:
        FileStream() : fp(0)
        {

        }
        virtual ~FileStream();
        bool openFile(const char *filename, const char *mode);

        virtual bool write(const void *buf, size_t len);
        virtual bool read(void *buf, size_t len);
        /// seek to position
        virtual void seek(size_t pos);
        /// seek with an offset
        virtual void seekOffset(int offset);
        virtual size_t length() const;
        virtual size_t tell() const;
    protected:
        void *fp;
    };

}

#endif//CANI_
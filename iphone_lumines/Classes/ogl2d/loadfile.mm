#import <cstdio>
#import <cassert>

#import "loadfile.h"

namespace ogl2d
{

namespace Utilities
{
/// DecodeJPGFileToGeneralBuffer
/// @note ���ļ����õ��ڴ��
/// @return int �ɹ��õ�TRUE / ʧ�ܵõ�FALSE
/// @param  const char* lpszPathName    �ļ���[in]
/// @param  char** buf         �ڴ��ָ��ĵ�ַ[out]
/// @param  unsigned long* size         �ڴ���С      [out]
int LoadGeneralBinaryFile(
    const char* lpszPathName,
    unsigned char** buffer,
    unsigned long* size)
{
    FILE* fp = fopen(lpszPathName, "rb");
    fpos_t pos;
    unsigned long len;

    assert(buffer);
    if (fp == NULL)
        return false;

    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &pos);
    fseek(fp, 0, SEEK_SET);
  
    assert(pos <= 0x200000); // less than 2m
    len = (unsigned long)pos;
    unsigned char* buf = new unsigned char[len];
    if (fread(buf, len, 1, fp) != 1)
    {
        fclose(fp);
        CleanupGeneralBuffer(&buf);
        return false;
    }
    fclose(fp);

    *size = len;
    *buffer = buf;
    return true;
}
/// CleanupGeneralBuffer
/// @note LoadGeneralBinaryFile
/// @param  unsigned char** buffer      �ڴ���ַ
void CleanupGeneralBuffer(unsigned char** buffer)
{
    assert(buffer);

    if (*buffer != NULL)
    {
        delete [] *buffer;
        *buffer = NULL;
    }
}

};   // Utilities
    
};   // ogl2d
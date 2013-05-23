#ifndef _LOAD_FILE_H
#define _LOAD_FILE_H

namespace ogl2d
{
    namespace Utilities
    {
        /// LoadGeneralBinaryFile
        /// @note ���ļ����õ��ڴ��
        /// @return int �ɹ��õ�TRUE / ʧ�ܵõ�FALSE
        /// @param  const char* lpszPathName    �ļ���[in]
        /// @param  unsigned char** buf         �ڴ��ָ��ĵ�ַ[out]
        /// @param  unsigned long* size         �ڴ���С      [out]
        int LoadGeneralBinaryFile(
            const char* lpszPathName,
            unsigned char** buf,
            unsigned long* size);
        
        /// CleanupGeneralBuffer
        /// @note LoadGeneralBinaryFile
        /// @param  unsigned char** buffer      �ڴ���ַ
        void CleanupGeneralBuffer(unsigned char** buffer);

    };   // Utilities
    
};   // ogl2d
#endif // _LOAD_FILE_H


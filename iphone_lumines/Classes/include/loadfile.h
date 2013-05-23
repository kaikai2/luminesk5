#ifndef _LOAD_FILE_H
#define _LOAD_FILE_H

namespace ogl2d
{
    namespace Utilities
    {
        /// LoadGeneralBinaryFile
        /// @note 打开文件，得到内存块
        /// @return int 成功得到TRUE / 失败得到FALSE
        /// @param  const char* lpszPathName    文件名[in]
        /// @param  unsigned char** buf         内存块指针的地址[out]
        /// @param  unsigned long* size         内存块大小      [out]
        int LoadGeneralBinaryFile(
            const char* lpszPathName,
            unsigned char** buf,
            unsigned long* size);
        
        /// CleanupGeneralBuffer
        /// @note LoadGeneralBinaryFile
        /// @param  unsigned char** buffer      内存块地址
        void CleanupGeneralBuffer(unsigned char** buffer);

    };   // Utilities
    
};   // ogl2d
#endif // _LOAD_FILE_H


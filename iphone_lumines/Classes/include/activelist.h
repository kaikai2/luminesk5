/*
* namespace ogl2d::AbstractBase
*
*   ActiveList ActiveListNode
* 活跃链表，自动保持正确链接的链表类
*/

#ifndef _ACTIVELIST_H
#define _ACTIVELIST_H



namespace ogl2d
{
    namespace AbstractBase
    {
        
        class ActiveList;
        
        /// ActiveListNode
        /// @note   自动链表结点
        class ActiveListNode
        {
        public:
            ActiveListNode();
            virtual ~ActiveListNode();
            
            /// 断开与链表的连接
            void Remove();
            /// -----------------------------------------
        private:
            friend class ActiveList;
            ActiveListNode *next, *prep;
        };
        
        /// ActiveList
        /// @note   自动链表
        class ActiveList
        {
        public:
            ActiveList();
            virtual ~ActiveList();
            
            /// 删除所有结点
            void Release();
            
            /// 插入结点
            void Insert(ActiveListNode *node);
            /// -----------------------------------------
        private:
            ActiveListNode head; // dedicated head node
        };
        
    } //AbstractBase
    
} //ogl2d

#endif // _ACTIVELIST_H
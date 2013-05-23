/*
* namespace ogl2d::AbstractBase
*
*   ActiveList ActiveListNode
* ��Ծ�����Զ�������ȷ���ӵ�������
*/

#ifndef _ACTIVELIST_H
#define _ACTIVELIST_H



namespace ogl2d
{
    namespace AbstractBase
    {
        
        class ActiveList;
        
        /// ActiveListNode
        /// @note   �Զ�������
        class ActiveListNode
        {
        public:
            ActiveListNode();
            virtual ~ActiveListNode();
            
            /// �Ͽ������������
            void Remove();
            /// -----------------------------------------
        private:
            friend class ActiveList;
            ActiveListNode *next, *prep;
        };
        
        /// ActiveList
        /// @note   �Զ�����
        class ActiveList
        {
        public:
            ActiveList();
            virtual ~ActiveList();
            
            /// ɾ�����н��
            void Release();
            
            /// ������
            void Insert(ActiveListNode *node);
            /// -----------------------------------------
        private:
            ActiveListNode head; // dedicated head node
        };
        
    } //AbstractBase
    
} //ogl2d

#endif // _ACTIVELIST_H
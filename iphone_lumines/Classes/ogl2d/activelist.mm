#import <cassert>
#import <activelist.h>

namespace ogl2d
{
namespace AbstractBase
{

ActiveListNode::ActiveListNode():next(NULL),prep(NULL)
{

}

ActiveListNode::~ActiveListNode()
{
    Remove();
}
    
/// 断开与链表的连接
void ActiveListNode::Remove()
{
    if (next)
    {
        next->prep = prep;
        next = NULL;
    }
    if (prep)
    {
        prep->next = next;
        prep = NULL;
    }
}

ActiveList::ActiveList()
{
}

ActiveList::~ActiveList()
{
    Release();
}

/// 删除所有结点
void ActiveList::Release()
{
    while (head.next)
    {
        delete head.next;
    }
}

/// 插入结点
void ActiveList::Insert(ActiveListNode *node)
{
    assert(node);

    node->Remove();

    node->prep = &head;
    if (head.next)
    {
        head.next->prep = node;
    }
    node->next = head.next;
    head.next = node;

}

}
}
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
    
/// �Ͽ������������
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

/// ɾ�����н��
void ActiveList::Release()
{
    while (head.next)
    {
        delete head.next;
    }
}

/// ������
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
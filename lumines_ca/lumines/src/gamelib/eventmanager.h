#ifndef _EVENT_H_
#define _EVENT_H_

/*******************************************************
*  事件传递模块
********************************************************
*  单发送者－单接收者模型
*  发送者必须继承Sender类，通过Sender::send方法把具体消息包数据发送给一个接收者
*  接收者必须继承Receiver类，当发现自己的Receive::m_TodoEvent数组不为空时，需要及时处理这些数据
*
*   EventManager负责所有的事件数据内容的空间管理，并管理未到达事件触发时间的事件。直到分发给对应接收者为止
*   
*   Receiver接收到EventManager的事件后将事件加入自己的队列，在空闲时取出事件处理，处理完可以通知EventManager释放该事件
*   
********************************************************/

#include <cassert>
#include <vector>
#include <list>

/// 事件内容数据包体积上限
#define MAX_EVENT_DATA_SIZE 0x100
#define TIME_EVENT_INFINITE ((DWORD)~0)

typedef unsigned long DWORD;

/// 发送者Id
typedef DWORD ESenderId;

/// 接收者Id
typedef DWORD EReceiverId;

/// class Event
/// @note   事件类，这个类包含了事件的消息内容及数据
class Event
{
public:
    /// getSize
    /// @note   获得数据块大小
    /// @return unsigned short    字节数
    unsigned short getSize() const;

    /// getBuffer
    /// @note   获得数据快地址
    /// @return const char*     数据块地址（不允许对数据块做释放操作）
    const char* getBuffer() const;
	struct Compare
	{
		bool operator()(const Event *e1, const Event *e2)
		{
			return e1->m_eventTime > e2->m_eventTime;
		}
	};
    //--------------------------------------
private:
    friend class EventManager;
    friend class Receiver;
    friend struct Compare;

    /// 不允许EventManager之外的模块创建和释放Event对象
    Event(ESenderId sender, EReceiverId receiver, unsigned short size, unsigned long bufferid, DWORD time, unsigned long loopTime = 1, DWORD loopDelay = 1);
    ~Event(); // no need to virtual this event destructor, to avoid the payload of vfptr

    void Release() const;

    DWORD m_eventTime;      // timeGetTime() 该事件最早触发的时间（有可能被耽误一段时间）
    DWORD m_loopDelay;      // 如果是循环发送，那么此值表示2次事件触发的时间间隔。
    ESenderId   m_Sender;   // 发送者id
    EReceiverId m_Receiver; // 接收者id
    unsigned long m_loopTime;// 循环发送，如果为 ~0 表示无限重复
    unsigned long m_bufferid;// 数据块块号
    unsigned short m_size;    // 数据块大小
    //const char* m_data;   // 数据块地址
};

/// Sender
/// @note   事件发送者基类
class Sender
{
public:
    Sender();
    virtual ~Sender();

    /// send
    /// @note   发送事件
    /// @param  EReceiverId receiver    接收者id
    /// @param  const void *msgdata     数据块内容（发送的是该数据块的复本）
    /// @param  unsigned short msgsize             数据块大小
    /// @param  DWROD timeAfterNow = 0    延时接收，默认为不延时
    /// @param  unsigned long loopTime            循环事件（定时器）
    /// @see    Event
    void send(EReceiverId receiver, const void *msgdata, unsigned short msgsize, DWORD timeAfterNow = 0, unsigned long loopTime = 1, DWORD loopDelay = 1);

    /// getSenderId
    /// @note   获得发送者id
    /// @return ESenderId 发送者id
    ESenderId getSenderId() const;
		
	struct Compare
	{
		bool operator()(const Sender *sender1, const Sender *sender2)
		{
			return sender1->getSenderId() < sender2->getSenderId();
		}
		bool operator()(const Sender *sender, ESenderId id)
		{
			return sender->getSenderId() < id;
		}
		bool operator()(ESenderId id, const Sender *sender)
		{
			return id < sender->getSenderId();
		}
	};
    //---------------------------------------------
private:
    ESenderId m_SenderId;
};

struct TodoEvent
{
    TodoEvent(Event* pEvent, int bRelease) : m_pEvent(pEvent), m_bRelease(bRelease)
    {
    }
    Event* m_pEvent;
    int m_bRelease; // 是否允许删除
};

/// Receiver
/// @note   事件接收者基类
class Receiver
{
public:
    Receiver();
    virtual ~Receiver();

    /// getReceiverId
    /// @note   获得接收者id
    /// @return EReceiverId 接收者id
    EReceiverId getReceiverId() const;

    /// clearAllEvents
    /// @note   通知 EventManager 把所有与本接收者有关的消息都删除。
    void clearAllEvents();
	struct Compare
	{
		bool operator()(const Receiver *receiver1, const Receiver *receiver2)
		{
			return receiver1->getReceiverId() < receiver2->getReceiverId();
		}
		bool operator()(const Receiver *receiver, EReceiverId id)
		{
			return receiver->getReceiverId() < id;
		}
		bool operator()(EReceiverId id, const Receiver *receiver)
		{
			return id < receiver->getReceiverId();
		}
	};
protected:
    /// getEvent
    /// @note   提取一个事件
    /// @return Event*  事件地址，如果为NULL表示已经取完
    const Event* getEvent();
    //------------------------
private:
    friend class EventManager;

    void release();

    /// addEvent
    /// @noet   由EventManager 发送事件的入口
    void addEvent(const TodoEvent& todoEvent);

    
    ::std::list< TodoEvent > m_TodoEvent; // received events
    TodoEvent m_LastEvent;
    //::std::list< Event* > 

    EReceiverId m_ReceiverId;
};

/// EventManager
/// @note   事件管理器，只有唯一的一个实例
class EventManager
{
public:
    /// tick
    /// @note   heart beat of the EventManager instance, call this function in a main loop
    void tick();
    
    /// GetInstance
    /// @note   static function, get an instance of EventManager
    static EventManager* getInstance();

    /// ReleaseInstance
    /// @note   static function, release the instance of EventManager
    ///         all events will be drop, buffers will be released
    static void releaseInstance();

    /// getBuffer
    /// @note   get the buffer address with a buffer id
    /// @return const char*     address of the buffer, can not be modified, NULL if bufferid is invalid
    const char* getBuffer(unsigned int bufferid);
    //-------------------------------------------------------
private:

    void send(ESenderId sid, EReceiverId rid, const char *msgdata, unsigned short msgsize, DWORD timeAfterNow, unsigned long loopTime = 1, DWORD loopDelay = 1);

    friend class Sender;
    friend class Receiver;
    friend class Event;
    ESenderId registe(const Sender &sender);
    EReceiverId registe(const Receiver &receiver);
    void unregiste(const Sender &sender, ESenderId id);
    void unregiste(const Receiver &receiver, EReceiverId id);

    ESenderId getFreeSenderId();
    EReceiverId getFreeReceiverId();

    EventManager();
    virtual ~EventManager();

    ::std::vector< Event* > m_EventHeap;
    
    Sender* getSender(ESenderId sid);
    Receiver* getReceiver(EReceiverId rid);

    /// clearAllEvents
    /// @note   删除所有 rid 的消息（包括循环消息）
    void clearAllEvents(EReceiverId rid);

    ::std::vector< Sender* > m_SenderList;
    ::std::vector< Receiver* >  m_ReceiverList;
    struct Buffer // 8 bytes
    {
        char *buf;                // 4 bytes
        unsigned short size;      // 2 bytes
        unsigned short ref;       // 1 bytes
//        unsigned char  reserved;  // 1 bytes
        bool operator == (const char *pBuf)
        {
            return buf == pBuf;
        }
    };
    Buffer& getFreeBuffer(unsigned short size); // find a reasonable buffer or allocate one
    void refBuffer(unsigned long bufferid);    // add ref
    void releaseBuffer(unsigned long bufferid);// release ref

    ::std::vector< Buffer > m_Buffers;
    ESenderId   m_LastSenderId;   // 最后一次分配的id
    EReceiverId m_LastReceiverId; // 最后一次分配的id

#ifdef _DEBUG
    unsigned long buffersize;
#endif

};

inline unsigned short Event::getSize() const
{
    assert(m_size <= MAX_EVENT_DATA_SIZE);
    return m_size;
}

inline ESenderId Sender::getSenderId() const
{
    return m_SenderId;
}

inline EReceiverId Receiver::getReceiverId() const
{
    return m_ReceiverId;
}
#endif

#ifndef _EVENT_H_
#define _EVENT_H_

/*******************************************************
*  �¼�����ģ��
********************************************************
*  �������ߣ���������ģ��
*  �����߱���̳�Sender�࣬ͨ��Sender::send�����Ѿ�����Ϣ�����ݷ��͸�һ��������
*  �����߱���̳�Receiver�࣬�������Լ���Receive::m_TodoEvent���鲻Ϊ��ʱ����Ҫ��ʱ������Щ����
*
*   EventManager�������е��¼��������ݵĿռ����������δ�����¼�����ʱ����¼���ֱ���ַ�����Ӧ������Ϊֹ
*   
*   Receiver���յ�EventManager���¼����¼������Լ��Ķ��У��ڿ���ʱȡ���¼��������������֪ͨEventManager�ͷŸ��¼�
*   
********************************************************/

#include <cassert>
#include <vector>
#include <list>

/// �¼��������ݰ��������
#define MAX_EVENT_DATA_SIZE 0x100
#define TIME_EVENT_INFINITE ((DWORD)~0)

typedef unsigned long DWORD;

/// ������Id
typedef DWORD ESenderId;

/// ������Id
typedef DWORD EReceiverId;

/// class Event
/// @note   �¼��࣬�����������¼�����Ϣ���ݼ�����
class Event
{
public:
    /// getSize
    /// @note   ������ݿ��С
    /// @return unsigned short    �ֽ���
    unsigned short getSize() const;

    /// getBuffer
    /// @note   ������ݿ��ַ
    /// @return const char*     ���ݿ��ַ������������ݿ����ͷŲ�����
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

    /// ������EventManager֮���ģ�鴴�����ͷ�Event����
    Event(ESenderId sender, EReceiverId receiver, unsigned short size, unsigned long bufferid, DWORD time, unsigned long loopTime = 1, DWORD loopDelay = 1);
    ~Event(); // no need to virtual this event destructor, to avoid the payload of vfptr

    void Release() const;

    DWORD m_eventTime;      // timeGetTime() ���¼����紥����ʱ�䣨�п��ܱ�����һ��ʱ�䣩
    DWORD m_loopDelay;      // �����ѭ�����ͣ���ô��ֵ��ʾ2���¼�������ʱ������
    ESenderId   m_Sender;   // ������id
    EReceiverId m_Receiver; // ������id
    unsigned long m_loopTime;// ѭ�����ͣ����Ϊ ~0 ��ʾ�����ظ�
    unsigned long m_bufferid;// ���ݿ���
    unsigned short m_size;    // ���ݿ��С
    //const char* m_data;   // ���ݿ��ַ
};

/// Sender
/// @note   �¼������߻���
class Sender
{
public:
    Sender();
    virtual ~Sender();

    /// send
    /// @note   �����¼�
    /// @param  EReceiverId receiver    ������id
    /// @param  const void *msgdata     ���ݿ����ݣ����͵��Ǹ����ݿ�ĸ�����
    /// @param  unsigned short msgsize             ���ݿ��С
    /// @param  DWROD timeAfterNow = 0    ��ʱ���գ�Ĭ��Ϊ����ʱ
    /// @param  unsigned long loopTime            ѭ���¼�����ʱ����
    /// @see    Event
    void send(EReceiverId receiver, const void *msgdata, unsigned short msgsize, DWORD timeAfterNow = 0, unsigned long loopTime = 1, DWORD loopDelay = 1);

    /// getSenderId
    /// @note   ��÷�����id
    /// @return ESenderId ������id
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
    int m_bRelease; // �Ƿ�����ɾ��
};

/// Receiver
/// @note   �¼������߻���
class Receiver
{
public:
    Receiver();
    virtual ~Receiver();

    /// getReceiverId
    /// @note   ��ý�����id
    /// @return EReceiverId ������id
    EReceiverId getReceiverId() const;

    /// clearAllEvents
    /// @note   ֪ͨ EventManager �������뱾�������йص���Ϣ��ɾ����
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
    /// @note   ��ȡһ���¼�
    /// @return Event*  �¼���ַ�����ΪNULL��ʾ�Ѿ�ȡ��
    const Event* getEvent();
    //------------------------
private:
    friend class EventManager;

    void release();

    /// addEvent
    /// @noet   ��EventManager �����¼������
    void addEvent(const TodoEvent& todoEvent);

    
    ::std::list< TodoEvent > m_TodoEvent; // received events
    TodoEvent m_LastEvent;
    //::std::list< Event* > 

    EReceiverId m_ReceiverId;
};

/// EventManager
/// @note   �¼���������ֻ��Ψһ��һ��ʵ��
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
    /// @note   ɾ������ rid ����Ϣ������ѭ����Ϣ��
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
    ESenderId   m_LastSenderId;   // ���һ�η����id
    EReceiverId m_LastReceiverId; // ���һ�η����id

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

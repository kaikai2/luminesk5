// define USE_SDL if sdl is used

#import <algorithm>
#import <cassert>
#import <vector>

#ifdef USE_SDL
#import <sdl.h>
#else
// zhu #import <windows.h>
#endif

#ifdef _DEBUG
#import <cstdio>
#endif

#ifndef SAFEDELETE
#define SAFEDELETE(a) \
    if(!(a))\
    {       \
    }       \
    else    \
    {       \
        delete (a); \
        (a) = NULL; \
    }
#endif
#import <eventmanager.h>
using namespace std;




static EventManager* s_pManager = NULL;

Event::Event(
    ESenderId sender, EReceiverId receiver, // from & to
    unsigned short size, unsigned long bufferid, // size and data slot id
    DWORD time, unsigned long loopTime, DWORD loopDelay): // delayTime & repeatCount & loopDelayCount
    m_size(size), m_bufferid(bufferid), m_eventTime(time), m_loopDelay(loopDelay),
    m_Sender(sender), m_Receiver(receiver),
    m_loopTime(loopTime)
{
}

Event::~Event()
{
    Release();
}

void Event::Release() const
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    pEventManager->releaseBuffer(m_bufferid);
}

const char* Event::getBuffer() const
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    return pEventManager->getBuffer(m_bufferid);
}

Sender::Sender()
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    m_SenderId = pEventManager->registe(*this);
}

Sender::~Sender()
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    pEventManager->unregiste(*this, m_SenderId);
}

void Sender::send(
    EReceiverId receiver,   // to whom
    const void* msgdata, unsigned short msgsize, // data & its size
    DWORD timeAfterNow, unsigned long loopTime, DWORD loopDelay) // delayTime & repeatCount
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    pEventManager->send(
        m_SenderId, receiver, (const char*)msgdata,msgsize, timeAfterNow, loopTime, loopDelay);
}

Receiver::Receiver():m_LastEvent(NULL, 0)
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    m_ReceiverId = pEventManager->registe(*this);
}

Receiver::~Receiver()
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    pEventManager->unregiste(*this, getReceiverId());

    clearAllEvents();//     release();
}

void Receiver::release()
{
    list< TodoEvent >::iterator event;
    for (event = m_TodoEvent.begin(); event != m_TodoEvent.end(); ++event)
    {
        // (*event)->Release();
        if (event->m_bRelease) // this is the only handle to the event, shoule be released
        {
            delete event->m_pEvent; // must delete it
        }
    }
    if (m_LastEvent.m_pEvent && m_LastEvent.m_bRelease)
    {
        delete m_LastEvent.m_pEvent;
    }
    m_LastEvent.m_pEvent = NULL;

    m_TodoEvent.clear();

}

void Receiver::clearAllEvents()
{
    EventManager* pEventManager = EventManager::getInstance();
    assert(pEventManager);
    pEventManager->clearAllEvents(getReceiverId());

    release();
}

void EventManager::clearAllEvents(EReceiverId rid)
{
    vector< Event* >::iterator iter, end;
    int count = 0;
    end = m_EventHeap.end();

    for (iter = m_EventHeap.begin(); iter < end; ++iter)
    {
        while(iter < end && (*iter)->m_Receiver == rid)
        {
            // (*iter)->Release(); // release the buffers
            delete *iter;
            *iter = *--end;
            count++;
        }
    }
    while (count--)
    {
        m_EventHeap.pop_back();
    }

    // rebuild the heap
	make_heap(m_EventHeap.begin(), m_EventHeap.end(), Event::Compare());
}

void Receiver::addEvent(const TodoEvent& todoEvent)
{
    m_TodoEvent.push_back(todoEvent);
}

const Event* Receiver::getEvent()
{
    TodoEvent todoEvent(NULL, 0);
    
    //EventManager* pEventManager = EventManager::getInstance();
    //assert(pEventManager);
    // pEventManager->tick();
    if (m_LastEvent.m_pEvent && m_LastEvent.m_bRelease)
    {
        delete m_LastEvent.m_pEvent;
    }
    m_LastEvent.m_pEvent = NULL;

    if (!m_TodoEvent.empty())
    {
        todoEvent = m_TodoEvent.front();
        m_TodoEvent.pop_front();
        m_LastEvent = todoEvent;
        // pEvent->Release();
    }
    if (todoEvent.m_pEvent)
    {
        assert(todoEvent.m_pEvent->m_size <= MAX_EVENT_DATA_SIZE);
    }
    return todoEvent.m_pEvent;
}

void EventManager::tick()
{
    
#ifdef USE_SDL
    DWORD now = SDL_GetTicks();
#else
    DWORD now = timeGetTime();
#endif
    while (m_EventHeap.size() && now >= m_EventHeap.front()->m_eventTime)// < 0x80000000)
    {
        pop_heap(m_EventHeap.begin(), m_EventHeap.end(), Event::Compare());
        Event* pEvent = m_EventHeap.back();

        assert(pEvent != NULL);

        assert(pEvent->m_loopTime != TIME_EVENT_INFINITE || pEvent->m_loopDelay > 0);

        Receiver* pReceiver = getReceiver(pEvent->m_Receiver);
        if (pReceiver == NULL) // receiver not found
        {
            m_EventHeap.pop_back();
            delete pEvent; // delete it
            continue;
        }

        if (pEvent->m_loopTime == TIME_EVENT_INFINITE || --pEvent->m_loopTime > 0)
        {
            // if the event is a loop event, and still need to be sent.
            //  push back it again
            pEvent->m_eventTime += pEvent->m_loopDelay;
            
            // refBuffer(pEvent->m_bufferid); // add ref

            push_heap(m_EventHeap.begin(), m_EventHeap.end(), Event::Compare());
            
            pReceiver->addEvent(TodoEvent(pEvent, 0)); // 0: can not release after event processed
        }
        else
        {
            // otherwise, pop it out
            m_EventHeap.pop_back();
            // only handle to the event is saved in receiver. the receiver should delete it when it processed
            
            pReceiver->addEvent(TodoEvent(pEvent, 1)); // 1: must release after event processed
        }
    }
}

EventManager* EventManager::getInstance()
{
    if (s_pManager == NULL)
    {
        s_pManager = new EventManager;
    }
    return s_pManager;
}

void EventManager::releaseInstance()
{
    SAFEDELETE(s_pManager);
}

EventManager::EventManager():m_LastSenderId(0), m_LastReceiverId(0)
{
#ifdef _DEBUG
    buffersize = 0;
#endif
}

EventManager::~EventManager()
{
    for (vector< Event* >::iterator event = m_EventHeap.begin();
        event != m_EventHeap.end(); ++event)
    {
        delete *event;
    }
    m_EventHeap.clear();
    for (vector< Buffer >::iterator buf = m_Buffers.begin();
    buf != m_Buffers.end(); ++buf
    )
    {
        delete [] buf->buf;
    }
    m_Buffers.clear();
}

ESenderId EventManager::getFreeSenderId()
{
    ESenderId id = m_LastSenderId + 1;
    if (id == 0) id = 1;

	vector< Sender* >::iterator left = lower_bound(
		m_SenderList.begin(), m_SenderList.end(), id, Sender::Compare());

    while (left != m_SenderList.end() && (*left)->getSenderId() == id)
    {
        ++left;
        ++id;
        if (id == 0)
        {
            id = 1;
            left = m_SenderList.begin();
        }
    }

    m_LastSenderId = id;
    return id;
}

EReceiverId EventManager::getFreeReceiverId()
{
    EReceiverId id = m_LastReceiverId + 1;
    if (id == 0)  // skip id = 0
        id = 1;
    
	vector< Receiver* >::iterator left = lower_bound(
        m_ReceiverList.begin(), m_ReceiverList.end(), id, Receiver::Compare());

    while (left != m_ReceiverList.end() && (*left)->getReceiverId() == id)
    {
        ++left;
        ++id;
        if (id == 0)
        {
            id = 1;
            left = m_ReceiverList.begin();
        }
    }

    m_LastReceiverId = id;
    return id;
}

ESenderId EventManager::registe(const Sender& sender)
{
    ESenderId id = getFreeSenderId();
    vector< Sender* >::iterator left = lower_bound(
        m_SenderList.begin(), m_SenderList.end(), id, Sender::Compare());
    m_SenderList.insert(left, const_cast< Sender* >(&sender));
    return id;
}

EReceiverId EventManager::registe(const Receiver& receiver)
{
    EReceiverId id = getFreeReceiverId();
    vector< Receiver* >::iterator left = lower_bound(
        m_ReceiverList.begin(), m_ReceiverList.end(), id, Receiver::Compare());
    m_ReceiverList.insert(left, const_cast<Receiver *>(&receiver));
    return id;
}

void EventManager::unregiste(const Sender& sender, ESenderId id)
{
    assert(m_SenderList.size() && "sender list empty");
    vector< Sender* >::iterator left = lower_bound(
        m_SenderList.begin(), m_SenderList.end(), id, Sender::Compare());
    assert(left != m_SenderList.end() && "sender list empty 2");
    assert(*left == &sender);
    m_SenderList.erase(left);
}

void EventManager::unregiste(const Receiver& receiver, EReceiverId id)
{
    assert(m_ReceiverList.size() && "receiver list empty");
    vector< Receiver* >::iterator left = lower_bound(
		m_ReceiverList.begin(), m_ReceiverList.end(), id, Receiver::Compare());
    assert(left != m_ReceiverList.end() && "receiver list empty");
    assert(*left == &receiver);
    m_ReceiverList.erase(left);
}

Sender* EventManager::getSender(ESenderId sid) 
{
    vector< Sender* >::iterator left = lower_bound(
        m_SenderList.begin(), m_SenderList.end(), sid, Sender::Compare());
    if (left == m_SenderList.end())
        return NULL;
    assert(*left != NULL);
    if ((*left)->getSenderId() == sid)
        return *left;

    return NULL;
}

Receiver* EventManager::getReceiver(EReceiverId rid) 
{
    vector< Receiver* >::iterator left = lower_bound(
        m_ReceiverList.begin(), m_ReceiverList.end(), rid, Receiver::Compare()
        );
    if (left == m_ReceiverList.end())
        return NULL;
    assert(*left != NULL);
    if ((*left)->getReceiverId() == rid)
        return *left;

    return NULL;
}

void EventManager::send(
    ESenderId sid, EReceiverId rid,  // from & to 
    const char* msgdata, unsigned short msgsize,  // data & its size
    DWORD timeAfterNow, unsigned long loopCount, DWORD loopDelay) // delayTime & repeatCount
{
    //vector< Event* >
    assert(msgdata && msgsize <= MAX_EVENT_DATA_SIZE);
    assert(loopCount != TIME_EVENT_INFINITE || loopDelay > 0);

    Sender* pSender = getSender(sid);
    assert(pSender != NULL);
    Receiver* pReceiver = getReceiver(rid);
    // assert(pReceiver != NULL);
    if (pReceiver == NULL)
        return;

    Buffer& buf = getFreeBuffer(msgsize);
    assert(buf.ref == 0); // assure that the buffer is not in use currently
    buf.ref = 1;
    unsigned int bufferid = *(unsigned int *)buf.buf;
    memcpy(buf.buf, msgdata, msgsize);
    
#ifdef USE_SDL
    Event *pEvent = new Event(
        sid, rid, msgsize, bufferid, timeAfterNow + SDL_GetTicks(), loopCount, loopDelay
        );
#else
    Event *pEvent = new Event(
        sid, rid, msgsize, bufferid, timeAfterNow + timeGetTime(), loopCount, loopDelay
        );
#endif
    m_EventHeap.push_back(pEvent);
    push_heap(m_EventHeap.begin(), m_EventHeap.end(), Event::Compare());

    // tick();
	/*
#ifdef _DEBUG
    char str[100];
    sprintf(str, "buffer count:%d total size:%d event count:%d\n", m_Buffers.size(), buffersize, m_EventHeap.size());
    OutputDebugString(str);
#endif
	*/
}

const char* EventManager::getBuffer(unsigned int bufferid)
{
    const char* buf = NULL;

    assert(bufferid < m_Buffers.size());

    buf = m_Buffers[bufferid].buf;
    assert(buf);

    return buf;
}

#define SIZE_UPPER_DWORD_BOUND(a) (((a)+3) & ~3)

EventManager::Buffer& EventManager::getFreeBuffer(unsigned short size)
{
    assert(size <= MAX_EVENT_DATA_SIZE);
    
    unsigned short bsize = SIZE_UPPER_DWORD_BOUND(size);

    assert(bsize >= 4);

    vector< Buffer >::iterator buf;
    for (buf = m_Buffers.begin(); buf != m_Buffers.end(); ++buf)
    {
        if (buf->ref == 0 && buf->size >= bsize)
            break;
    }
    if (buf != m_Buffers.end()) // found
    {
        *(unsigned int*)(buf->buf) = (unsigned int)(buf - m_Buffers.begin()); // buffer id
        return *buf;
    }
    else // no match, create one
    {
        Buffer buf;
        buf.ref = 0;
        buf.size = bsize;
        buf.buf = new char[bsize];
        *(unsigned int*)buf.buf = (unsigned int)m_Buffers.size(); // bufferid
        m_Buffers.push_back(buf);
#ifdef _DEBUG
        buffersize += bsize;
#endif
        return m_Buffers.back();
    }
}
void EventManager::refBuffer(unsigned long bufferid)
{
    assert(bufferid < m_Buffers.size());
   
    m_Buffers[bufferid].ref++; // = 0;
    assert(m_Buffers[bufferid].ref != 0xffff && "Fatal: refference too more times, i will not tell if someone is cheating...-_-||");
}
void EventManager::releaseBuffer(unsigned long bufferid)
{
    assert(bufferid < m_Buffers.size());
    // assure that the buffer is being used, 
    //  but not sure it is used by who release it
    // assert(m_Buffers[bufferid].ref == 1); 

    m_Buffers[bufferid].ref--;// = 0;
    assert(m_Buffers[bufferid].ref != 0xffff && "Fatal: release too more times, who is cheating ?");
    // assert(m_Buffers[bufferid].refCount == 0 && 
    //  "Fatal:refCount has been modified by other routine");
}
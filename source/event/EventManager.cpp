#include "EventManager.h"

namespace v3d
{
namespace event
{

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
    CEventManager::reset();
    m_receivers.clear();
}

void CEventManager::update()
{
    while (!m_events.empty())
    {
        const SInputEventPtr& event = m_events.front();

        if (!CEventManager::sendEvent(event))
        {
            //TODO: Event not found
        }

        m_events.pop();
    }
}

void CEventManager::pushEvent(const SInputEventPtr& event)
{
    m_events.push(event);
}

bool CEventManager::sendEvent(const SInputEventPtr& event)
{
    bool result = false;

    for (ReceiverList::const_iterator iter = m_receivers.begin(); iter != m_receivers.end(); ++iter)
    {
        if ((*iter).first == event->_eventType)
        {
            if (!(*iter).second.expired())
            {
                result = true;

                const IEventReceiverPtr& ptr = (*iter).second.lock();
                ptr->onEvent(event);
            }
        }
    }

    return result;
}

void CEventManager::reset()
{
    std::queue<SInputEventPtr> empty;
    std::swap(m_events, empty);
}

void CEventManager::attach(EInputEventType type, const IEventReceiverPtr& receiver)
{
    std::pair<EInputEventType, IEventReceiverWPtr> rcv = std::make_pair(type, receiver);

    auto predFind = [rcv](const ReceiverItem& item) -> bool
    {
        return (rcv.first == item.first && rcv.second.lock() == item.second.lock());
    };

    auto iter = std::find_if(m_receivers.begin(), m_receivers.end(), predFind);
    if (iter == m_receivers.end())
    {
        m_receivers.insert(rcv);
    }
}

void CEventManager::dettach(EInputEventType type, const IEventReceiverPtr& receiver)
{
    std::pair<EInputEventType, IEventReceiverWPtr> rcv = std::make_pair(type, receiver);

    auto predFind = [rcv](const ReceiverItem& item) -> bool
    {
        return (rcv.first == item.first && rcv.second.lock() == item.second.lock());
    };

    auto iter = std::find_if(m_receivers.begin(), m_receivers.end(), predFind);
    if (iter != m_receivers.end())
    {
        m_receivers.erase(iter);
    }
}

} //namespace event
} //namespace v3d

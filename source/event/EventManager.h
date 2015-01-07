#ifndef _V3D_EVENT_MANAGER_H_
#define _V3D_EVENT_MANAGER_H_

#include "common.h"

#include "Singleton.h"

#include "InputEvents.h"
#include "EventReceiver.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::multimap<EInputEventType, IEventReceiverWPtr>  ReceiverList;
    typedef std::pair<EInputEventType, IEventReceiverWPtr>      ReceiverItem;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CEventManager : public Singleton<CEventManager>
    {
    private:

        friend                      Singleton<CEventManager>;

        CEventManager();
        virtual                     ~CEventManager();

    public:

        void                        reset();

        void                        attach(EInputEventType type, const IEventReceiverPtr& receiver);
        void                        dettach(EInputEventType type, const IEventReceiverPtr& receiver);

        void                        pushEvent(const SInputEventPtr& event);
        bool                        sendEvent(const SInputEventPtr& event);

        virtual void                update();



    private:

        std::queue<SInputEventPtr>  m_events;
        ReceiverList                m_receivers;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_EVENT_MANAGER_H_

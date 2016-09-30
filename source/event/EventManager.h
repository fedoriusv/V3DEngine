#pragma once

#include "common.h"

#include "utils/Singleton.h"

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

    class CEventManager : public utils::TSingleton<CEventManager>
    {
    private:

        friend                      utils::TSingleton<CEventManager>;

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

} //namespace event
} //namespace v3d

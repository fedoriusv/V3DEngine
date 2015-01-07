#ifndef _V3D_IEVENT_RECEIVER_H_
#define _V3D_IEVENT_RECEIVER_H_

#include "common.h"
#include "event/InputEvents.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class IEventReceiver
    {
    protected:

        IEventReceiver() {};

    public:

        virtual         ~IEventReceiver() {};

        virtual bool    onEvent(const SInputEventPtr& event) = 0;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<IEventReceiver> IEventReceiverPtr;
    typedef std::weak_ptr<IEventReceiver>   IEventReceiverWPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_IEVENT_RECEIVER_H_
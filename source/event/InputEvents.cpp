#include "InputEvents.h"
#include "utils/Timer.h"

namespace v3d
{
namespace event
{

SInputEvent::SInputEvent()
    : _eventType(eUnknownInputEvent)
    , _priority(eNormal)
    , _timeStamp(utils::CTimer::getCurrentTime())
{
}

SInputEvent::~SInputEvent()
{
}

bool SInputEvent::operator<(const SInputEvent& event)
{
    return _priority < event._priority;
}

} //namespace event
} // namespace v3d

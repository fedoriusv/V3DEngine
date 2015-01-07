#include "InputEvents.h"

using namespace v3d;
using namespace v3d::event;

SInputEvent::SInputEvent()
    : _eventType(eUnknownInputEvent)
    , _priority(eNormal)
    , _timeStamp(0) //TODO: get current time, need CTimer
{
}

SInputEvent::~SInputEvent()
{
}

bool SInputEvent::operator<(const SInputEvent& event)
{
    return _priority < event._priority;
}
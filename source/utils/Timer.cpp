#include "Timer.h"
#ifdef _PLATFORM_WIN_
#   include <windows.h>
#endif //_PLATFORM_WIN_

using namespace utils;

s64 CTimer::getTicks()
{
#ifdef _PLATFORM_WIN_
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);

    return ticks.QuadPart;
#endif //_PLATFORM_WIN_
}

s64 CTimer::getTicksPerSecond()
{
#ifdef _PLATFORM_WIN_
    LARGE_INTEGER tickPerSeconds;
    QueryPerformanceFrequency(&tickPerSeconds);

    return tickPerSeconds.QuadPart;
#endif //_PLATFORM_WIN_
}

s64 CTimer::getCurrentTime()
{
    return (CTimer::getTicks() / CTimer::getTicksPerSecond());
}

CTimer::CTimer()
: m_lastTime(0)
, m_currentTime(0)
, m_time(0)
, m_isStopped(false)
{
    m_time = CTimer::getCurrentTime();
}

CTimer::~CTimer()
{
}

u32 CTimer::getTime() const
{
    if (CTimer::isStopped())
    {
        return m_lastTime;
    }

    return m_lastTime + (u32)(m_time - m_currentTime);
}

void CTimer::setTime(u32 time)
{
    m_time = CTimer::getCurrentTime();
    m_lastTime = time;
    m_currentTime = m_time;
}

void CTimer::start()
{
    if (!CTimer::isStopped())
    {
        CTimer::setTime(m_lastTime);
    }

    m_isStopped = false;
}

void CTimer::stop()
{
    if (!CTimer::isStopped())
    {
        m_lastTime = CTimer::getTime();
    }

    m_isStopped = true;
}

bool CTimer::isStopped() const
{
    return m_isStopped;
}
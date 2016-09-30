#include "Thread.h"

namespace v3d
{
namespace utils
{

Thread::Thread()
    : m_isRunning(false)
{
}

Thread::~Thread()
{
    ASSERT(!m_isRunning, "Thread::~Thread(): thread is running");
}

bool Thread::run(ThreadCallback callback, void* userData)
{
    if (m_isRunning)
    {
        return true;
    }

    m_callback = callback;
    m_userData = userData;
    m_isRunning = true;

    m_thread = std::thread(callback, userData);
    if (!m_thread.joinable())
    {
        Thread::terminate();
    }

    return m_isRunning;
}

void Thread::wait(bool terminate)
{
    if (m_isRunning)
    {
        if (terminate)
        {
            Thread::terminate();
        }
        m_thread.join();
        m_isRunning = false;
    }
}

void Thread::terminate()
{
    m_isRunning = false;
}

bool Thread::isRunning()
{
    return m_isRunning;
}

} //namespace utils
} //namespace v3d

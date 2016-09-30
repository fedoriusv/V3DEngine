#include "Thread.h"

namespace v3d
{
namespace utils
{

std::thread::id Thread::s_mainThreadId = Thread::getCurrentThread();

Thread::Thread()
    : m_isRunning(false)
    , m_callback(nullptr)
    , m_userData(nullptr)
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

const std::thread::id Thread::getThreadId() const
{
    return m_thread.get_id();
}

const std::thread::id Thread::getCurrentThread()
{
    return std::this_thread::get_id();
}

const std::thread::id Thread::getMainThreadId()
{
    return s_mainThreadId;
}

} //namespace utils
} //namespace v3d

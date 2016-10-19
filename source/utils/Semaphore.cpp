#include "Semaphore.h"

namespace v3d
{
namespace utils
{

Semaphore::Semaphore(u32 maxThreads)
    : m_countThreads(0)
    , m_maxThreads(maxThreads)
{
}

Semaphore::~Semaphore()
{
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    
    m_cv.wait(lock, [&]
    {
        return m_countThreads >= m_maxThreads;
    });
    --m_countThreads;
}

bool Semaphore::tryWait()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_countThreads >= m_maxThreads)
    {
        --m_countThreads;
        return true;
    }

    return false;
}

void Semaphore::signal()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    
    if (m_maxThreads > m_countThreads)
    {
        ++m_countThreads;
    }

    m_cv.notify_one();
}

} //namespace utils
} //namespace v3d

#include "Semaphore.h"

namespace v3d
{
namespace utils
{

Semaphore::Semaphore(u32 countThreads)
    : m_countThreads(countThreads)
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
        return m_countThreads > 0;
    });
    --m_countThreads;
}

bool Semaphore::tryWait()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_countThreads > 0)
    {
        --m_countThreads;
        return true;
    }

    return false;
}

void Semaphore::post()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    
    ++m_countThreads;
    m_cv.notify_one();
}

} //namespace utils
} //namespace v3d

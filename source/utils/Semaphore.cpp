#include "Semaphore.h"
#include "utils/Logger.h"

#include <windows.h>

namespace v3d
{
namespace utils
{

    HANDLE hSemaphore;

Semaphore::Semaphore(u32 maxThreads)
    : m_countThreads(0)
    , m_maxThreads(maxThreads)
{
    hSemaphore = CreateSemaphore( NULL, m_maxThreads, m_maxThreads,  NULL);
}

Semaphore::~Semaphore()
{
}

void Semaphore::wait()
{

    while (true)
    {
        DWORD dwWaitResult = WaitForSingleObject(hSemaphore, 1);
        if (dwWaitResult == WAIT_OBJECT_0)
        {
            break;
        }
    }
   /* std::unique_lock<std::mutex> lock(m_mutex);
    
    if (m_countThreads < m_maxThreads)
    {
        LOG_DEBUG("Semaphore::wait: lock thread");
    }
    m_cv.wait(lock, [&]
    {
        return m_countThreads >= m_maxThreads;
    });
    --m_countThreads;*/
}

bool Semaphore::tryWait()
{
   /* std::unique_lock<std::mutex> lock(m_mutex);

    if (m_countThreads >= m_maxThreads)
    {
        --m_countThreads;
        return true;
    }*/

    return false;
}

void Semaphore::signal()
{
    ReleaseSemaphore(hSemaphore, 1, NULL);

    /*std::unique_lock<std::mutex> lock(m_mutex);
    
    if (m_maxThreads > m_countThreads)
    {
        LOG_DEBUG("Semaphore::signal: release thread");
        ++m_countThreads;
    }

    m_cv.notify_one();*/
}

} //namespace utils
} //namespace v3d

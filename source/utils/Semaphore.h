#pragma once

#include "common.h"

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Semaphore
    {
    public:

        explicit Semaphore(u32 maxThreads);
        ~Semaphore();

        void wait();
        bool tryWait();
        void signal();

    private:

        u32                     m_countThreads;
        s32                     m_maxThreads;

        std::condition_variable m_cv;
        std::mutex              m_mutex;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

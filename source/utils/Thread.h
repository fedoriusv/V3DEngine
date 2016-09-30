#pragma once

#include "common.h"

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::function<void(void*)> ThreadCallback;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Thread
    {
    public:

        Thread();
        ~Thread();

        bool                            run(ThreadCallback callback, void* userData);
        void                            wait(bool terminate = false);
        void                            terminate();

        bool                            isRunning();

        const std::thread::id           getThreadId() const;

        static const std::thread::id    getCurrentThread();
        static const std::thread::id    getMainThreadId();

    private:

        std::thread                     m_thread;
        bool                            m_isRunning;

        void*                           m_userData;
        ThreadCallback                  m_callback;

        static std::thread::id          s_mainThreadId;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

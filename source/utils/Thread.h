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

        bool run(ThreadCallback callback, void* userData);
        void wait(bool terminate = false);
        void terminate();

        bool isRunning();

    private:



        std::thread     m_thread;
        bool            m_isRunning;

        void*           m_userData;
        ThreadCallback  m_callback;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

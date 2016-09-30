#pragma once

#include "renderer/Renderer.h"
#include "utils/Thread.h"
#include "stream/MemoryStream.h"

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Device final
    {
    public:

        Device(renderer::RendererPtr renderer, bool isThreaded);
        ~Device();

        bool                        init();
        const renderer::RendererPtr getRenderer() const;

        bool                        isThreaded() const;

        void                        threadWorker(void* data);

    private:


        void                        runCommand(const stream::MemoryStream& stream);
        
        bool                        m_isThreaded;
        bool                        m_isRunning;

        stream::MemoryStream        m_commandBuffer;

        utils::Thread               m_thread;
        //semophore
        renderer::RendererPtr       m_render;


    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    using DevicePtr = std::unique_ptr<Device>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace platform
} //namespace v3d

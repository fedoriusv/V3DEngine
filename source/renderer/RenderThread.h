#pragma once

#include "common.h"
#include "utils/Thread.h"
#include "renderer/Renderer.h"
#include "stream/MemoryStream.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ERenderCommand
    {
        eRCmdInit = 0,
        eRCmdBeginFrame,
        eRCmdEndFrame,
        eRCmdPresentFrame,
        eRCmdDraw,

        eRCmdDestroy,

        eRCmdTerminate,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderThread final
    {
    public:

        explicit RenderThread(const renderer::RendererPtr renderer);
        ~RenderThread();

        void                        init();
                                    
        void                        beginFrame();
        void                        endFrame();
        void                        presentFrame();
                                    
        void                        draw();

        //

        void                        destroy();

        void                        terminate();

    private:

        void                        threadWorker(void* data);
        void                        runCommand(const stream::MemoryStream& stream);

        std::atomic_bool            m_isRunning;

        stream::MemoryStream        m_commandBuffer;

        utils::Thread               m_thread;
        //semophore

        renderer::RendererWPtr      m_renderer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

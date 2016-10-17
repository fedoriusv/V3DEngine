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

        eCommandUpdateTexure1D,
        eCommandUpdateTexure2D,
        eCommandUpdateTexure3D,
        eCommandUpdateTexureCube,
        eCommandFillTexure1D,
        eCommandFillTexure2D,
        eCommandFillTexure3D,

        eRCmdDestroy,

        eRCmdTerminate,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderThread;

    class RenderStreamCommand
    {
    public:

        RenderStreamCommand(ERenderCommand cmd);
        ~RenderStreamCommand();

        template <class T>
        const T&    readValue(u32 count = 1);
        void*       readValue(u32 size, u32 count = 1);

        template <class T>
        void        writeValue(const T& val, u32 count = 1);
        void        writeValue(const void* data, u32 size, u32 count = 1);

        void*       getStreamData() const;
        u32         getStreamSize() const;

    private:

        friend              RenderThread;

        RenderStreamCommand(const stream::MemoryStream& stream);

        stream::MemoryStream m_commandStream;
        ERenderCommand       m_command;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderThread final
    {
    public:

        explicit RenderThread(const renderer::RendererPtr renderer);
        ~RenderThread();

        void                        pushCommand(const RenderStreamCommand& command);

        void                        init();
                                    
        void                        beginFrame();
        void                        endFrame();
        void                        presentFrame();
                                    
        void                        draw();

        //

        void                        destroy();

        void                        terminate();

    private:

        const RenderStreamCommand   popCommand();

        void                        threadWorker(void* data);
        void                        runCommand(const RenderStreamCommand& command);

        std::atomic_bool            m_isRunning;

        stream::MemoryStream        m_commandBuffer;

        utils::Thread               m_thread;
        //semophore

        renderer::RendererWPtr      m_renderer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    inline const T& RenderStreamCommand::readValue(u32 count)
    {
        T& value;
        m_commandStream.read(value, sizeof(T), count);

        return value;
    }

    template<class T>
    inline void RenderStreamCommand::writeValue(const T& val, u32 count)
    {
        m_commandStream.write((const void*)val, sizeof(T), count);
    }

} //namespace renderer
} //namespace v3d

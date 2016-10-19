#include "RenderThread.h"

namespace v3d
{
namespace renderer
{

RenderStreamCommand::RenderStreamCommand(ERenderCommand cmd)
    : m_commandStream()
    , m_command(cmd)
{
    m_commandStream.seekBeg(0);
}

RenderStreamCommand::~RenderStreamCommand()
{
    m_commandStream.clear();
}

void* RenderStreamCommand::readValue(u32 size, u32 count) const
{
    void* data = nullptr;
    m_commandStream.read(data, size, count);

    return data;
}

void RenderStreamCommand::writeValue(const void* data, u32 size, u32 count)
{
    m_commandStream.write(data, size, count);
}

void* RenderStreamCommand::getStreamData() const
{
    return m_commandStream.getData();
}

u32 RenderStreamCommand::getStreamSize() const
{
    return m_commandStream.size();
}

ERenderCommand RenderStreamCommand::getCommand() const
{
    return m_command;
}

RenderThread::RenderThread(const renderer::RendererPtr renderer)
    : m_renderer(renderer)
    , m_waitSemophore(1)
{
    bool result = m_thread.run(std::bind(&RenderThread::threadWorker, this, std::placeholders::_1), nullptr);
    if (!result)
    {
        ASSERT(false, "RenderThread::RenderThread: thread has not started");
    }
}

RenderThread::~RenderThread()
{
    m_thread.wait(true);
}

void RenderThread::pushCommand(const RenderStreamCommand& command)
{
    m_commandBufferQueue.push(std::move(command));
}

void RenderThread::submit()
{
    m_waitSemophore.signal();
}

void RenderThread::wait()
{
    m_waitSemophore.wait();
}

const RenderStreamCommand RenderThread::popCommand()
{
    RenderStreamCommand& command = m_commandBufferQueue.front();
    m_commandBufferQueue.pop();

    return command;
}

void RenderThread::init()
{
    /*m_commandBuffer.write(s32(ERenderCommand::eRCmdInit));
    m_commandBuffer.seekBeg(0);*/
}

void RenderThread::beginFrame()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdBeginFrame));
}

void RenderThread::endFrame()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdEndFrame));
}

void RenderThread::presentFrame()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdPresentFrame));
}

void RenderThread::draw()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdDraw));
}

void RenderThread::destroy()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdDestroy));
}

void RenderThread::terminate()
{
    //m_commandBuffer.write(s32(ERenderCommand::eRCmdTerminate));
}

void RenderThread::threadWorker(void* data)
{
    m_thread.setName("Render Thread");

    stream::MemoryStream* stream = reinterpret_cast<stream::MemoryStream*>(data);
    m_isRunning = true;

    while (m_isRunning)
    {
        if (!m_thread.isRunning())
        {
            m_isRunning = false;
        }

        RenderThread::wait();
        if (!m_commandBufferQueue.empty())
        {
            const RenderStreamCommand& command = RenderThread::popCommand();
            RenderThread::runCommand(command);
        }
    }
}

void RenderThread::runCommand(const RenderStreamCommand& command)
{
    ERenderCommand cmd = command.getCommand();

    ASSERT(!m_renderer.expired(), "Render is't valid");
    renderer::RendererPtr render = m_renderer.lock();

    switch (cmd)
    {
    case ERenderCommand::eCommandInitialize:
        render->immediateInit();
        break;

 /*   case ERenderCommand::eRCmdBeginFrame:
        render->beginFrame();
        break;

    case ERenderCommand::eRCmdEndFrame:
        render->endFrame();
        break;

    case ERenderCommand::eRCmdPresentFrame:
        render->presentFrame();
        break;

    case ERenderCommand::eRCmdDraw:
        render->draw();
        break;

    case ERenderCommand::eRCmdDestroy:
        break;

    case ERenderCommand::eRCmdTerminate:
        m_isRunning = false;
        break;*/

    case ERenderCommand::eCommandUpdateTexure1D:
    {
        Texture* texute = reinterpret_cast<Texture*>(command.readValue<void*>());
        u32 offset = command.readValue<u32>();
        u32 size = command.readValue<u32>();
        void* data = command.readValue(size, 1);
        u32 mipLevel = command.readValue<u32>();

        texute->immediateUpdate(offset, size, data, mipLevel);
        RenderThread::submit();

        free(data);
        break;
    }

    

    default:
        ASSERT(false, "Buffer command is unknown");
    }
}

} //namespace renderer
} //namespace v3d

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
    RenderStreamCommand::writeValue<ERenderCommand>(cmd);
}

RenderStreamCommand::~RenderStreamCommand()
{
    m_commandStream.clear();
}

void* RenderStreamCommand::readValue(u32 size, u32 count)
{
    void* data = malloc(size * count);
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

RenderStreamCommand::RenderStreamCommand(const stream::MemoryStream& stream)
    : m_commandStream(stream)
{
    m_commandStream.seekBeg(0);
    m_command = RenderStreamCommand::readValue<ERenderCommand>();
}

RenderThread::RenderThread(const renderer::RendererPtr renderer)
    : m_renderer(renderer)
{
    bool result = m_thread.run(std::bind(&RenderThread::threadWorker, this, std::placeholders::_1), &m_commandBuffer);
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
    m_commandBuffer.write(command.getStreamSize());
    m_commandBuffer.write(command.getStreamData(), command.getStreamSize(), 1);
}

const RenderStreamCommand RenderThread::popCommand()
{
    u32 streamSize;
    m_commandBuffer.read(streamSize);
    stream::MemoryStream stream = stream::MemoryStream(nullptr, streamSize);
    m_commandBuffer.read(stream.getData(), streamSize, 1);

    RenderStreamCommand command(stream);
    return command;
}

void RenderThread::init()
{
    /*m_commandBuffer.write(s32(ERenderCommand::eRCmdInit));
    m_commandBuffer.seekBeg(0);*/
}

void RenderThread::beginFrame()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdBeginFrame));
}

void RenderThread::endFrame()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdEndFrame));
}

void RenderThread::presentFrame()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdPresentFrame));
}

void RenderThread::draw()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdDraw));
}

void RenderThread::destroy()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdDestroy));
}

void RenderThread::terminate()
{
    m_commandBuffer.write(s32(ERenderCommand::eRCmdTerminate));
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

        if (m_commandBuffer.size() > 0)
        {
            const RenderStreamCommand& command = RenderThread::popCommand();
            RenderThread::runCommand(command);
        }
    }
}

void RenderThread::runCommand(const RenderStreamCommand& command)
{
    s32 flag;
    m_commandBuffer.read(flag);

    ASSERT(!m_renderer.expired(), "Render is't valid");
    renderer::RendererPtr render = m_renderer.lock();

    switch (flag)
    {
    case ERenderCommand::eRCmdInit:
        render->immidateInit();
        break;

    case ERenderCommand::eRCmdBeginFrame:
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
        break;

    case ERenderCommand::eCommandUpdateTexure:
        Texture tex;
        TexData data;
        m_commandBuffer.read(tex);
        m_commandBuffer.read(data);
        tex->immidateUpdate(data);




    default:
        ASSERT(false, "Buffer command is unknown");
    }
}

} //namespace renderer
} //namespace v3d

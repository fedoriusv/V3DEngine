#include "RenderThread.h"

namespace v3d
{
namespace renderer
{

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

        RenderThread::runCommand(*stream);
    }
}

void RenderThread::runCommand(const stream::MemoryStream& stream)
{
    if (m_commandBuffer.size() <= 0)
    {
        return;
    }

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

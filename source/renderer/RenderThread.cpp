#include "RenderThread.h"
#include "utils/Logger.h"

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

RenderStreamCommand::RenderStreamCommand(const RenderStreamCommand& obj)
    : m_commandStream(obj.m_commandStream)
    , m_command(obj.m_command)
{
}

RenderStreamCommand::RenderStreamCommand(RenderStreamCommand&& obj)
    : m_commandStream(std::move(obj.m_commandStream))
    , m_command(obj.m_command)
{
    obj.m_command = eCommandUnknown;
}

RenderStreamCommand::~RenderStreamCommand()
{
    m_commandStream.clear();
}

void* RenderStreamCommand::readValue(u32 size, u32 count) const
{
    void* data = malloc(size * count);
    m_commandStream.read(data, size, count);

    return data;
}

void RenderStreamCommand::writeValue(const void* data, u32 size, u32 count)
{
    m_commandStream.write(data, size, count);
}

void RenderStreamCommand::endCommand()
{
    m_commandStream.seekBeg(0);
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
    , m_lastCommand(eCommandUnknown)
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

void RenderThread::pushCommand(RenderStreamCommand& command)
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
    RenderStreamCommand command = std::move(m_commandBufferQueue.front());
    m_commandBufferQueue.pop();

    return command;
}

void RenderThread::init()
{
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

    RenderThread::submit();

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
        RenderThread::submit();
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
        RenderThread::submit();
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

    case ERenderCommand::eCommandCreateTexture:
    {
        Texture* texute = command.readValue<Texture*>();
        u32 size = command.readValue<u32>();
        EImageFormat format = command.readValue<EImageFormat>();
        EImageType type = command.readValue<EImageType>();
        bool presentData = command.readValue<bool>();
        void* data = nullptr;
        if (presentData)
        {
            u32 dataSize = size * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
            if (texute->getTarget() == ETextureTarget::eTextureCubeMap)
            {
                data = command.readValue(dataSize, k_textureCubemapSideCount);
            }
            else
            {
                data = command.readValue(dataSize, 1);
            }

            texute->create(data, dataSize);
            free(data);
        }

        texute->create(nullptr, 0);
        break;
    }

    case ERenderCommand::eCommandUpdateTexure:
    {
        Texture* texute = command.readValue<Texture*>();
        u32 mipLevel = command.readValue<u32>();
        if (texute->getTarget() == ETextureTarget::eTexture1D ||
            texute->getTarget() == ETextureTarget::eTextureBuffer)
        {
            u32 offset = command.readValue<u32>();
            u32 size = command.readValue<u32>();
            void* data = command.readValue(size, 1);
            RenderThread::submit();

            texute->update(offset, size, data, mipLevel);
            free(data);
        }
        else if (texute->getTarget() == ETextureTarget::eTexture2D ||
            texute->getTarget() == ETextureTarget::eTexture1DArray)
        {
            core::Dimension2D offset = command.readValue<core::Dimension2D>();
            core::Dimension2D size = command.readValue<core::Dimension2D>();
            void* data = command.readValue(size.getArea(), 1);
            RenderThread::submit();

            texute->update(offset, size, data, mipLevel);
            free(data);
        }
        else if (texute->getTarget() == ETextureTarget::eTexture3D ||
            texute->getTarget() == ETextureTarget::eTexture2DArray)
        {
            core::Dimension3D offset = command.readValue<core::Dimension3D>();
            core::Dimension3D size = command.readValue<core::Dimension3D>();
            void* data = command.readValue(size.getArea(), 1);
            RenderThread::submit();

            texute->update(offset, size, data, mipLevel);
            free(data);
        }
        break;
    }

    case ERenderCommand::eCommandReadTexture:
    {
        Texture* texute = command.readValue<Texture*>();
        void const* dataDst = command.readValue<void const*>();
        u32 mipLevel = command.readValue<u32>();
        if (texute->getTarget() == ETextureTarget::eTextureCubeMap)
        {
            u32 cubemapSide = command.readValue<u32>();
            texute->read(cubemapSide, dataDst, mipLevel);
        }
        else
        {
            texute->read(dataDst, mipLevel);
        }
        RenderThread::submit();
        break;
    }

    case ERenderCommand::eCommandFillTexure:
    {
        Texture* texute = command.readValue<Texture*>();
        u32 mipLevel = command.readValue<u32>();
        if (texute->getTarget() == ETextureTarget::eTexture1D ||
            texute->getTarget() == ETextureTarget::eTextureBuffer)
        {
            u32 offset = command.readValue<u32>();
            u32 size = command.readValue<u32>();
            void* data = command.readValue(size, 1);
            RenderThread::submit();

            texute->fill(data, offset, size, mipLevel);
            free(data);
        }
        else if (texute->getTarget() == ETextureTarget::eTexture2D ||
            texute->getTarget() == ETextureTarget::eTexture1DArray)
        {
            core::Dimension2D offset = command.readValue<core::Dimension2D>();
            core::Dimension2D size = command.readValue<core::Dimension2D>();
            void* data = command.readValue(size.getArea(), 1);
            RenderThread::submit();

            texute->fill(data, offset, size, mipLevel);
            free(data);
        }
        else if (texute->getTarget() == ETextureTarget::eTexture3D ||
            texute->getTarget() == ETextureTarget::eTexture2DArray)
        {
            core::Dimension3D offset = command.readValue<core::Dimension3D>();
            core::Dimension3D size = command.readValue<core::Dimension3D>();
            void* data = command.readValue(size.getArea(), 1);
            RenderThread::submit();

            texute->fill(data, offset, size, mipLevel);
            free(data);
        }
        break;
    }

    case ERenderCommand::eCommandDestoyTexure:
    {
        Texture* texute = command.readValue<Texture*>();
        texute->destroy();
        delete texute;
        break;
    }

    case ERenderCommand::eCommandCreateBuffer:
    {
        Buffer* buffer = command.readValue<Buffer*>();
        u32 size = command.readValue<u32>();
        bool presentData = command.readValue<bool>();
        void* data = nullptr;
        if (presentData)
        {
            data = command.readValue(size, 1);
        }

        if (!buffer->create(size, data))
        {
            LOG_ERROR("ERenderCommand::eCommandCreateBuffer: can't create buffer");
        }

        free(data);
        break;
    }

    case ERenderCommand::eCommandUpdateBuffer:
    {
        Buffer* buffer = command.readValue<Buffer*>();
        u32 offset = command.readValue<u32>();
        u32 size = command.readValue<u32>();
        bool presentData = command.readValue<bool>();
        void* data = nullptr;
        if (presentData)
        {
            data = command.readValue(size, 1);
        }

        buffer->update(offset, size, data);
        free(data);
        break;
    }

    case ERenderCommand::eCommandReadBuffer:
    {
        Buffer* buffer = command.readValue<Buffer*>();
        u32 offset = command.readValue<u32>();
        u32 size = command.readValue<u32>();
        void* data = command.readValue<void*>();

        buffer->read(offset, size, data);
        break;
    }

    case ERenderCommand::eCommandDestroyBuffer:
    {
        Buffer* buffer = command.readValue<Buffer*>();

        buffer->destroy();
        delete buffer;

        break;
    }

    default:
        ASSERT(false, "Buffer command is unknown");
    }

    m_lastCommand = cmd;
}

} //namespace renderer
} //namespace v3d

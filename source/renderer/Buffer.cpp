#include "Buffer.h"
#include "Engine.h"
#include "RenderThread.h"

namespace v3d
{
namespace renderer
{

Buffer::Buffer()
    : m_impl(nullptr)
{
}

Buffer::Buffer(EBufferTarget target, EDataUsageType type, u32 size, const void* data)
    : m_impl(ENGINE_CONTEXT->createBuffer(target, type))
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCreateBuffer);
        command.writeValue<Buffer* const>(m_impl);
        command.writeValue<u32>(size);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size, 1);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Buffer::create(size, data);
    }
}

Buffer::~Buffer()
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandDestroyBuffer);
        command.writeValue<Buffer* const>(m_impl);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Buffer::destroy();
        delete m_impl;
    }
}

bool Buffer::create(u32 size, const void* data)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->create(size, data);
}

void Buffer::destroy()
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->destroy();
}

void Buffer::update(u32 offset, u32 size, const void* data)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateBuffer);
        command.writeValue<Buffer* const>(m_impl);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size, 1);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->update(offset, size, data);
    }
}

void Buffer::read(u32 offset, u32 size, void* const data)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandReadBuffer);
        command.writeValue<Buffer* const>(m_impl);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        command.writeValue<void* const>(data);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, true);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->read(offset, size, data);
    }
}

void Buffer::bind() const
{
    assert(false);
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->bind();
}

void Buffer::bindToTarget(EBufferTarget target, u32 offset, u32 size) const
{
    assert(false);
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->bindToTarget(target, offset, size);
}

void Buffer::unbind() const
{
    assert(false);
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->unbind();
}

void* const Buffer::map(u32 offset, u32 size)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->map(offset, size);
}

bool Buffer::unmap()
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->unmap();
}

EBufferTarget Buffer::getTarget() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getTarget();
}

} //namespace renderer
} //namespace v3d

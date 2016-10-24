#include "Texture.h"
#include "utils/Logger.h"
#include "resources/Image.h"
#include "Engine.h"
#include "RenderThread.h"

namespace v3d
{
namespace renderer
{

using namespace core;

Texture::Texture()
    : m_impl(nullptr)
{
}

Texture::Texture(ETextureTarget target, EImageFormat format, EImageType type, u32 size, const void* data, u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createTexture(target, format, type, Dimension3D(size, 1, 1), data, mipCount))
{
    ASSERT(target == eTexture1D || target == eTextureBuffer, "Invalid target");
    if (target == eTextureBuffer)
    {
        ASSERT(mipCount == 1, "Unsupported mipmap target for buffer texture");
    }

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCreateTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(size);
        command.writeValue<EImageFormat>(format);
        command.writeValue<EImageType>(type);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            u32 dataSize = size * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
            command.writeValue(data, dataSize, 1);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        u32 dataSize = size * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->create(data, dataSize);
    }
}

Texture::Texture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data, u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createTexture(target, format, type, Dimension3D(size.width, size.height, 1), data, mipCount))
{
    ASSERT(target == eTexture1DArray || target == eTexture2D || target == eTextureRectangle || target == eTexture2DMSAA, "Invalid target");
    if ((target == eTextureRectangle || target == eTexture2DMSAA))
    {
        ASSERT(mipCount == 1, "Unsupported mipmap target");
    }

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCreateTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(size.getArea());
        command.writeValue<EImageFormat>(format);
        command.writeValue<EImageType>(type);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
            command.writeValue(data, dataSize, 1);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->create(data, dataSize);
    }
}

Texture::Texture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createTexture(target, format, type, size, data, mipCount))
{
    ASSERT(target == eTexture2DArray || target == eTexture3D || target == eTexture3DMSAA, "Invalid target");
    ASSERT(target == eTexture3DMSAA && mipCount > 1, "Unsupported mipmap target");

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCreateTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(size.getArea());
        command.writeValue<EImageFormat>(format);
        command.writeValue<EImageType>(type);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
            command.writeValue(data, dataSize, 1);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->create(data, dataSize);
    }
}

Texture::Texture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[k_textureCubemapSideCount], u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createCubeTexture(format, type, size, data, mipCount))
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCreateTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(size.getArea());
        command.writeValue<EImageFormat>(format);
        command.writeValue<EImageType>(type);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
            command.writeValue(data, dataSize, k_textureCubemapSideCount);
        }
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        u32 dataSize = size.getArea() * ImageFormat::typeSize(type) * ImageFormat::componentCount(format);
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->create(data, dataSize * k_textureCubemapSideCount);
    }
}

Texture::~Texture()
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandDestoyTexure);
        command.writeValue<Texture* const>(m_impl);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->destroy();
        delete m_impl;
    }
}

bool Texture::create(const void* data, u32 srcSize)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->create(data, srcSize);
}

void Texture::destroy()
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->destroy();
}

void Texture::bind(u32 unit)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->bind(unit);
}

void Texture::unbind()
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->unbind();
}

bool Texture::isValid() const
{
    if (m_impl)
    {
        return m_impl->isValid();
    }

    ASSERT(false, "m_impl is nullptr");
    return false;
}

void Texture::update(u32 offset, u32 size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size, 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->update(offset, size, data, mipLevel);
    }
}

void Texture::update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->update(offset, size, data, mipLevel);
    }
}

void Texture::update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension3D>(offset);
        command.writeValue<Dimension3D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->update(offset, size, data, mipLevel);
    }
}

void Texture::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->update(offset, size, data, mipLevel);
    }
}

void Texture::read(void const* data, u32 mipLevel) const
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandReadTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<void const*>(data);
        command.writeValue<u32>(mipLevel);
        ENGINE_RENDERER->pushCommand(command, true);
        command.endCommand();
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->read(data, mipLevel);
    }
}

void Texture::read(u32 cubemapSide, void const* data, u32 mipLevel) const
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandReadTexture);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<void const*>(data);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(cubemapSide);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, true);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->read(data, mipLevel);
    }
}

void Texture::fill(const void* data, u32 offset, u32 size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size, 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->fill(data, offset, size, mipLevel);
    }
}

void Texture::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->fill(data, offset, size, mipLevel);
    }
}

void Texture::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture* const>(m_impl);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension3D>(offset);
        command.writeValue<Dimension3D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->fill(data, offset, size, mipLevel);
    }
}

ETextureTarget Texture::getTarget() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getTarget();
}

ETextureFilter Texture::getMinFilter() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getMinFilter();
}

ETextureFilter Texture::getMagFilter() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getMagFilter();
}

EWrapType Texture::getWrap() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getWrap();
}

EAnisotropic Texture::getAnisotropic()const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getAnisotropic();
}

u32 Texture::getMipmapLevels() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getMipmapLevels();
}

const Dimension3D& Texture::getSize() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getSize();
}

EImageFormat Texture::getFormat() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getFormat();
}

EImageType Texture::getType() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getType();
}

void Texture::setFilterType(ETextureFilter min, ETextureFilter mag)
{
    if (m_impl)
    {
        return m_impl->setFilterType(min, mag);
    }
}

void Texture::setAnisotropicLevel(EAnisotropic level)
{
    if (m_impl)
    {
        return m_impl->setAnisotropicLevel(level);
    }
}

TexturePtr Texture::getImpl() const
{
    return m_impl;
}

TexturePtr Texture::clone() const
{
    if (!m_impl)
    {
        return nullptr;
    }

    TexturePtr texure;
    if (Texture::getTarget() == ETextureTarget::eTextureCubeMap)
    {
        texure = new Texture(m_impl->getFormat(), m_impl->getType(), Dimension2D(m_impl->getSize().width, m_impl->getSize().height),
            nullptr, m_impl->getMipmapLevels());
    }
    else
    {
        texure = new Texture(m_impl->getTarget(), m_impl->getFormat(), m_impl->getType(), m_impl->getSize(), nullptr, m_impl->getMipmapLevels());
    }
    texure->copyData(m_impl);

    return texure;
}

void Texture::setWrap(EWrapType wrap)
{
    if (m_impl)
    {
        return m_impl->setWrap(wrap);
    }
}

bool Texture::isEnable() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->isEnable();
}

void Texture::copyData(const TexturePtr& texture)
{
    if (m_impl)
    {
        m_impl->copyData(texture);
    }
}

} //namespace renderer
} //namespace v3d

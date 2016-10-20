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
    ASSERT(target == eTextureBuffer && mipCount > 1, "Unsupported mipmap target");

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommadCreateTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(size);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size, 1);
        }

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::create(data, size);
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
        RenderStreamCommand command(ERenderCommand::eCommadCreateTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(size.getArea());
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size.getArea(), 1);
        }

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::create(data, size.getArea());
    }
}

Texture::Texture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createTexture(target, format, type, size, data, mipCount))
{
    ASSERT(target == eTexture2DArray || target == eTexture3D || target == eTexture3DMSAA, "Invalid target");
    ASSERT(target == eTexture3DMSAA && mipCount > 1, "Unsupported mipmap target");

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommadCreateTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(size.getArea());
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size.getArea(), 1);
        }

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::create(data, size.getArea());
    }
}

Texture::Texture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[k_textureCubemapSideCount], u32 mipCount)
    : m_impl(ENGINE_CONTEXT->createCubeTexture(format, type, size, data, mipCount))
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommadCreateTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(size.getArea() * k_textureCubemapSideCount);
        bool presentData = (data != nullptr) ? true : false;
        command.writeValue<bool>(presentData);
        if (presentData)
        {
            command.writeValue(data, size.getArea(), k_textureCubemapSideCount);
        }

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::create(data, size.getArea() * k_textureCubemapSideCount);
    }
}

Texture::~Texture()
{
}

bool Texture::create(const void* data, u32 srcSize)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->create(data, srcSize);
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
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size, 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension3D>(offset);
        command.writeValue<Dimension3D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandUpdateTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::immediateUpdate(u32 offset, u32 size, const void* data, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->update(offset, size, data, mipLevel);
}

void Texture::immediateUpdate(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->update(offset, size, data, mipLevel);
}

void Texture::immediateUpdate(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->update(offset, size, data, mipLevel);
}

void Texture::immediateUpdate(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D & size, const void* data, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->update(cubemapSide, offset, size, data, mipLevel);
}

void Texture::read(void const* data, u32 mipLevel) const
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandReadTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<void const*>(data);
        command.writeValue<u32>(mipLevel);
        ENGINE_RENDERER->pushCommand(command, true);
    }
    else
    {
        Texture::immediateRead(data, mipLevel);
    }
}

void Texture::read(u32 cubemapSide, void const* data, u32 mipLevel) const
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandReadTexture);
        command.writeValue<Texture const*>(this);
        command.writeValue<void const*>(data);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(cubemapSide);

        ENGINE_RENDERER->pushCommand(command, true);
    }
    else
    {
        Texture::immediateRead(data, mipLevel);
    }
}

void Texture::immediateRead(void const* data, u32 mipLevel) const
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->read(data, mipLevel);
}

void Texture::immediateRead(u32 cubemapSide, void const* data, u32 mipLevel) const
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->read(cubemapSide, data, mipLevel);
}

void Texture::fill(const void* data, u32 offset, u32 size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<u32>(offset);
        command.writeValue<u32>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size, 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateFill(data, offset, size, mipLevel);
    }
}

void Texture::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension2D>(offset);
        command.writeValue<Dimension2D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 mipLevel)
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandFillTexure);
        command.writeValue<Texture const*>(this);
        command.writeValue<u32>(mipLevel);
        command.writeValue<Dimension3D>(offset);
        command.writeValue<Dimension3D>(size);
        ASSERT(data, "data is nullptr");
        command.writeValue(data, size.getArea(), 1);

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        Texture::immediateUpdate(offset, size, data, mipLevel);
    }
}

void Texture::immediateFill(const void* data, u32 offset, u32 size, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->fill(data, offset, size, mipLevel);
}

void Texture::immediateFill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->fill(data, offset, size, mipLevel);
}

void Texture::immediateFill(const void * data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 mipLevel)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->fill(data, offset, size, mipLevel);
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

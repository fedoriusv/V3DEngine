#include "TextureGL.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "BufferGL.h"
#include "SamplerGL.h"
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

using namespace core;

#define TEXTURE_CUBE_MAP_COUNT 6

GLenum ECubeMapGL[TEXTURE_CUBE_MAP_COUNT] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

GLenum ETextureTargetGL[ETextureTarget::eTargetCount] =
{
    GL_TEXTURE_1D,
    GL_TEXTURE_1D_ARRAY,
    GL_TEXTURE_RECTANGLE,
    GL_TEXTURE_2D,
    GL_TEXTURE_2D_MULTISAMPLE,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_3D,
    GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_BUFFER
};

GLenum ETextureBindingGL[ETextureTarget::eTargetCount] =
{
    GL_TEXTURE_BINDING_1D,
    GL_TEXTURE_BINDING_1D_ARRAY,
    GL_TEXTURE_BINDING_RECTANGLE,
    GL_TEXTURE_BINDING_2D,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE,
    GL_TEXTURE_BINDING_2D_ARRAY,
    GL_TEXTURE_BINDING_3D,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,
    GL_TEXTURE_BINDING_CUBE_MAP,
    GL_TEXTURE_BINDING_BUFFER
};

GLenum ETextureFilterGL[ETextureFilter::eFilterCount] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};

GLenum EWrapTypeGL[EWrapType::eWrapCount] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER
};

GLenum EImageFormatGL[toEnumType(EImageFormat::eFormatCount)] =
{
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_DEPTH_COMPONENT,
    GL_STENCIL_INDEX
};

GLenum EImageTypeGL[toEnumType(EImageType::eImageTypeCount)] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT_24_8,
    GL_HALF_FLOAT,
    GL_FLOAT
};

u32 CTextureGL::s_currentTextureID[] = { 0 };
u32 CTextureGL::s_currentUnitID      = 0;

u32 CTextureGL::internalFormat(u32 format, u32 type)
{
    switch (type)
    {

    //BYTE
    case GL_BYTE:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R8_SNORM;
        case GL_RG:
            return GL_RG8_SNORM;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB8_SNORM;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA8_SNORM;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //UNSIGNED_BYTE
    case GL_UNSIGNED_BYTE:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R8;
        case GL_RG:
            return GL_RG8;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB8;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA8;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //SHORT
    case GL_SHORT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R16I;
        case GL_RG:
            return GL_RG16I;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB16I;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA16I;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    };

    //UNSIGNED_SHORT
    case GL_UNSIGNED_SHORT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R16UI;
        case GL_RG:
            return GL_RG16UI;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB16UI;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA16UI;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT16;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //INT
    case GL_INT:
    {
        switch (format)
        {
        case GL_RED_INTEGER:
            return GL_R32I;
        case GL_RG_INTEGER:
            return GL_RG32I;
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return GL_RGB32I;
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            return GL_RGBA32I;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //UNSIGNED_INT
    case GL_UNSIGNED_INT:
    {
        switch (format)
        {
        case GL_RED_INTEGER:
            return GL_R32UI;
        case GL_RG_INTEGER:
            return GL_RG32UI;
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return GL_RGB32UI;
        case GL_RGBA:
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            return GL_RGBA32UI;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //UNSIGNED_INT_24_8
    case GL_UNSIGNED_INT_24_8:
    {
        switch (format)
        {
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT24;
        case GL_STENCIL_INDEX:
            return GL_STENCIL_INDEX8;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //HALF_FLOAT
    case GL_HALF_FLOAT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R16F;
        case GL_RG:
            return GL_RG16F;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB16F;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA16F;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }

    //FLOAT
    case GL_FLOAT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R32F;
        case GL_RG:
            return GL_RG32F;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB32F;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA32F;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT32F;
        default:
            ASSERT(false, "Unkown format");
            return format;
        };
    }
    };

    return format;
}

CTextureGL::CTextureGL(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
    : m_target(target)
    , m_format(format)
    , m_type(type)
    , m_size(size)

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_textureID(0)

    , m_textureBuffer(nullptr)
    , m_sampler(nullptr)

    , m_initialized(false)
{
    LOG_DEBUG("CTextureGL::CTextureGL constructor %x", this);
    CTextureGL::create();
    if (data)
    {
        switch (m_target)
        {
        case ETextureTarget::eTexture1D:
        case ETextureTarget::eTextureBuffer:
            CTextureGL::update(0, size.width, data, level);
            break;

        case ETextureTarget::eTexture1DArray:
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture2DMSAA:
        case ETextureTarget::eTextureRectangle:
            CTextureGL::update(Dimension2D(0U, 0U), Dimension2D(size.width, size.height), data, level);
            break;

        case ETextureTarget::eTexture2DArray:
        case ETextureTarget::eTexture3D:
        case ETextureTarget::eTexture3DMSAA:
            CTextureGL::update(Dimension3D(0U, 0U, 0U), size, data, level);
            break;

        default:
            ASSERT(false, "Invalid target");
            break;
        }
    }
}

CTextureGL::CTextureGL(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)
    : m_target(eTextureCubeMap)
    , m_format(format)
    , m_type(type)
    , m_size(Dimension3D(size.width, size.height, 1))

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_textureID(0)

    , m_textureBuffer(nullptr)
    , m_sampler(nullptr)

    , m_initialized(false)
{
    LOG_DEBUG("CTextureGL::CTextureGL constructor %x", this);
    CTextureGL::create();
    if (data)
    {
        for (u32 cubemapSide = 0; cubemapSide < TEXTURE_CUBE_MAP_COUNT; ++cubemapSide)
        {
            CTextureGL::update(cubemapSide, Dimension2D(0U, 0U), size, data[cubemapSide], level);
        }
    }
}

CTextureGL::~CTextureGL()
{
    CTextureGL::destroy();

    ASSERT(m_textureID == 0, "Texture doesn't deleted");
    LOG_DEBUG("CTextureGL::CTextureGL destructor %x", this);
}

void CTextureGL::bind(u32 unit)
{
    if (!m_initialized)
    {
        return;
    }

    CTextureGL::bindTextureUnit(unit);
    if (m_sampler)
    {
        m_sampler->bind(unit);
    }

    if (m_textureBuffer)
    {
        u32 format = EImageFormatGL[toEnumType(m_format)];
        u32 type = EImageTypeGL[toEnumType(m_type)];

        CTextureGL::bindTexBuffer(internalFormat(format, type), m_textureID, m_textureBuffer->getID());
        RENDERER->checkForErrors("CTextureGL: Bind Texture Buffer Error");
    }
    else
    {
        CTextureGL::bindTexture(m_target, m_textureID);
        RENDERER->checkForErrors("CTextureGL: Bind Texture Error");
    }
}

void CTextureGL::unbind()
{
    if (!m_initialized)
    {
        return;
    }

    if (m_textureBuffer)
    {
        CTextureGL::bindTexture(ETextureTarget::eTextureBuffer, 0);
    }
    else
    {
        CTextureGL::bindTexture(m_target, 0);
    }

    RENDERER->checkForErrors("CTextureGL: Unbind Texture Error");
}

void CTextureGL::reset()
{
    CTextureGL::bindTextureUnit(0);
    for (u32 i = 0; i < eTargetCount; ++i)
    {
        CTextureGL::bindTexture((ETextureTarget)i, 0);
    }

    RENDERER->checkForErrors("CTextureGL: Unbind All Texture Error");
}

bool CTextureGL::create()
{
    if (m_initialized)
    {
        return true;
    }

    bool success = false;

    glGenTextures(1, &m_textureID);

    GLint originalTexture;
    glGetIntegerv(ETextureBindingGL[m_target], &originalTexture);

    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        {
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_size.width <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage1D(ETextureTargetGL[eTexture1D], m_mipmapLevel + 1, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]), m_size.width);
            RENDERER->checkForErrors("CTextureGL::setData eTexture1D Error");

            success = true;
        }
        break;

        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture1DArray:
        case ETextureTarget::eTextureRectangle:
        {
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_size.width <= maxSize && (s32)m_size.height <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage2D(ETextureTargetGL[m_target], m_mipmapLevel + 1, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]), m_size.width, m_size.height);
            RENDERER->checkForErrors("CTextureGL::setData eTexture2D Error");

            success = true;
        }
        break;

        case ETextureTarget::eTexture2DMSAA:
        {
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_size.width <= maxSize && (s32)m_size.height <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL

            u32 samplesSize = DRIVER_CONTEXT->getSamplesCount();
#ifdef _DEBUG_GL
            GLint maxSamples;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            ASSERT((s32)samplesSize <= maxSamples, "Invalid Samples count");
#endif //_DEBUG_GL
            glTexStorage2DMultisample(ETextureTargetGL[eTexture2DMSAA], samplesSize, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]),
                m_size.width, m_size.height, GL_TRUE);
            RENDERER->checkForErrors("CTextureGL::setData eTexture2DMSAA Error");

            success = true;
        }
        break;

        case ETextureTarget::eTexture3D:
        case ETextureTarget::eTexture2DArray:
        {
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_size.width <= maxSize && (s32)m_size.height <= maxSize &&
                (s32)m_size.depth <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage3D(ETextureTargetGL[m_target], m_mipmapLevel + 1, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]),
                m_size.width, m_size.height, m_size.depth);
            RENDERER->checkForErrors("CTextureGL::setData eTexture3D Error");

            success = true;
        }
        break;

        case ETextureTarget::eTexture3DMSAA:
        {
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_size.width <= maxSize && (s32)m_size.height <= maxSize && 
                (s32)m_size.depth <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL

            u32 samplesSize = DRIVER_CONTEXT->getSamplesCount();
#ifdef _DEBUG_GL
            GLint maxSamples;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            ASSERT((s32)samplesSize <= maxSamples, "Invalid Samples count");
#endif //_DEBUG_GL
            glTexStorage3DMultisample(ETextureTargetGL[eTexture3DMSAA], samplesSize, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]),
                m_size.width, m_size.height, m_size.depth, GL_TRUE);
            RENDERER->checkForErrors("CTextureGL::initTexture3DMSAA error");

            success = true;
        }
        break;

        case ETextureTarget::eTextureCubeMap:
        {
            for (u32 cubemapSide = 0; cubemapSide < TEXTURE_CUBE_MAP_COUNT; ++cubemapSide)
            {
                //glTexStorage2D(GL_PROXY_TEXTURE_CUBE_MAP, m_mipmapLevel + 1, internalFormat(EImageFormatGL[m_format], EImageTypeGL[m_type]), m_size.width, m_size.height);
                glTexImage2D(ECubeMapGL[cubemapSide], 0, internalFormat(EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)]), m_size.width, m_size.height,
                    0, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], NULL);
                RENDERER->checkForErrors("CTextureGL::setData eTextureCubeMap Error");
            }

            success = true;
        }
        break;

        case ETextureTarget::eTextureBuffer:
        {
            LOG_DEBUG("CTextureGL::create: Store to buffer");
            ASSERT(!m_textureBuffer, "Texture target already exist");
            m_textureBuffer = new BufferGL(EBufferTarget::eTextureStoreBuffer);

            s32 dataSize = m_size.getArea() * ImageFormat::typeSize(m_type) * ImageFormat::componentCount(m_format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->set(EDataUsageType::eDrawStatic, dataSize, nullptr);
            RENDERER->checkForErrors("CTextureGL::setData eTextureBuffer Error");

            success = true;
        }
        break;

        default:
            ASSERT(false, "Wrong target");
            return success;
    }

    ASSERT(!m_sampler, "Sampler already exist");
    CTextureGL::setSampler(new SamplerGL());

    m_sampler->setWrap(EWrapTypeGL[m_wrap]);
    m_sampler->setAnisotropicLevel(m_anisotropicLevel);
    m_sampler->setFilterType(ETextureFilterGL[m_minFilter], ETextureFilterGL[m_magFilter]);

    glTexParameteri(ETextureTargetGL[m_target], GL_TEXTURE_MAX_LEVEL, m_mipmapLevel);
    RENDERER->checkForErrors("CTextureGL: set level Error");
    if (m_mipmapLevel > 0)
    {
        glGenerateMipmap(ETextureTargetGL[m_target]);
        RENDERER->checkForErrors("CTextureGL: GenerateMipmap Error");
    }

    if (originalTexture >= 0)
    {
        CTextureGL::bindTexture(m_target, originalTexture);
    }
    RENDERER->checkForErrors("CTextureGL: Create Texture Error");

    if (success)
    {
        m_initialized = true;
        m_enable = true;
    }

    return success;
}

void CTextureGL::destroy()
{
    if (m_sampler)
    {
        m_sampler->unbind(m_textureID);

        delete m_sampler;
        m_sampler = nullptr;
    }

    if (m_textureBuffer)
    {
        delete m_textureBuffer;
        m_textureBuffer = nullptr;
    }

    CTextureGL::bindTexture(m_target, 0);
    if (m_textureID > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

bool CTextureGL::isValid() const
{
    return m_textureID > 0 && !m_size.isNull();
}

bool CTextureGL::isEnable() const
{
    return m_enable;
}

void CTextureGL::update(u32 offset, u32 size, const void* data, u32 level)
{
    ASSERT(size <= m_size.width, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, ImageFormat::typeSize(m_type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage1D(ETextureTargetGL[eTexture1D], level, offset, size, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture1D Error");
            return;
        }

        case ETextureTarget::eTextureBuffer:
        {
            ASSERT(level == 0, "Texture buffer unsupport lods");

            s32 dataSize = size * ImageFormat::typeSize(m_type) * ImageFormat::componentCount(m_format);
            s32 dataOffset = offset * ImageFormat::typeSize(m_type) * ImageFormat::componentCount(m_format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->update(dataOffset, dataSize, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureBuffer Error");
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }
}

void CTextureGL::update(const Dimension2D& offset, const Dimension2D& size, const void* data, u32 level)
{
    ASSERT(size.width <= m_size.width && size.height <= m_size.height, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture1DArray:
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, ImageFormat::typeSize(m_type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage2D(ETextureTargetGL[eTexture2D], level, offset.width, offset.height, size.width, size.height, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture2D Error");
            return;
        }

        case ETextureTarget::eTextureRectangle:
            ASSERT(false, "Rectangle data updated through renderbuffer");
            return;

        case ETextureTarget::eTexture2DMSAA:
            ASSERT(false, "Multisample data updated through renderbuffer");
            return;

        default:
            ASSERT(false, "Wrong target");
    }

    RENDERER->checkForErrors("CTextureGL::readData Error");
}

void CTextureGL::update(const Dimension3D& offset, const Dimension3D& size, const void* data, u32 level)
{
    ASSERT(size.width <= m_size.width && size.height <= m_size.height && size.depth <= m_size.depth, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture3D:
        case ETextureTarget::eTexture2DArray:
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, ImageFormat::typeSize(m_type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage3D(ETextureTargetGL[m_target], level, offset.width, offset.height, offset.depth, size.width, size.height, size.depth,
                EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture3D Error");
            return;
        }

        case ETextureTarget::eTexture3DMSAA:
            ASSERT(false, "Multisample data updated through renderbuffer");
            return;

        default:
            ASSERT(false, "Wrong target");
    }
}

void CTextureGL::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level)
{
    ASSERT(size.width <= m_size.width && size.height <= m_size.height, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTextureCubeMap:
        {
            ASSERT(cubemapSide < TEXTURE_CUBE_MAP_COUNT, "Wrong cubemap side");
            s32 index = (cubemapSide < TEXTURE_CUBE_MAP_COUNT) ? cubemapSide : 0;

            glPixelStorei(GL_UNPACK_ALIGNMENT, ImageFormat::typeSize(m_type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage2D(ECubeMapGL[index], level, offset.width, offset.height, size.width, size.height, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureCubeMap Error");
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }
}



void CTextureGL::read(void* data, u32 level) const
{
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTexture1DArray:
    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTextureRectangle:
    case ETextureTarget::eTexture2DArray:
    case ETextureTarget::eTexture3D:
    {
        auto convertToGLImageFormat = [](EImageFormat format, EImageType type) -> u32
        {
            switch (type)
            {
            case EImageType::eInt:
            case EImageType::eUnsignedInt:
            case EImageType::eShort:
            case EImageType::eUnsignedShort:
                switch (format)
                {
                case EImageFormat::eRed:
                    return GL_RED_INTEGER;

                case EImageFormat::eRG:
                    return GL_RG_INTEGER;

                case EImageFormat::eRGB:
                    return GL_RGB_INTEGER;

                case EImageFormat::eRGBA:
                    return GL_RGBA_INTEGER;

                default:
                    break;
                }
                break;

            default:
                break;
            };

            return EImageFormatGL[toEnumType(format)];
        };

        u32 format = convertToGLImageFormat(m_format, m_type);
        u32 type = EImageTypeGL[toEnumType(m_type)];

        glPixelStorei(GL_PACK_ALIGNMENT, ImageFormat::typeSize(m_type));
        RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

        glGetTexImage(ETextureTargetGL[toEnumType(m_target)], level, format, type, data);
        RENDERER->checkForErrors("CTextureGL::readData Error");

        return;
    }

    case ETextureTarget::eTexture2DMSAA:
    case ETextureTarget::eTexture3DMSAA:
        ASSERT(false, "Multisample doesn't supported");

        return;

    case ETextureTarget::eTextureBuffer:
    {
        u32 format = EImageFormatGL[toEnumType(m_format)];
        u32 type = EImageTypeGL[toEnumType(m_type)];

        s32 dataSize = m_size.getArea() * ImageFormat::typeSize(m_type) * ImageFormat::componentCount(m_format);

#ifdef _DEBUG_GL
        GLint maxSize;
        glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
        ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
        ASSERT(m_textureBuffer, "TBO doesn't exist");
        m_textureBuffer->read(0, dataSize, data); //TODO: maybe need to use mapper
        RENDERER->checkForErrors("CTextureGL::readData buffer Error");

        return;
    }

    default:
        ASSERT(false, "Wrong target");
    }
}

void CTextureGL::read(u32 cubemapSide, void* data, u32 level) const
{
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTextureCubeMap:
        {
            ASSERT(cubemapSide > TEXTURE_CUBE_MAP_COUNT, "Wrong cubemap side");
            s32 index = (cubemapSide > TEXTURE_CUBE_MAP_COUNT) ? 0 : cubemapSide;

            u32 format = EImageFormatGL[toEnumType(m_format)];
            u32 type = EImageTypeGL[toEnumType(m_type)];

            glGetTexImage(ECubeMapGL[index], level, format, type, data);
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }
}

u32 CTextureGL::getTextureID() const
{
    return m_textureID;
}

void CTextureGL::setFilterType(ETextureFilter min, ETextureFilter mag)
{
    ASSERT(mag <= ETextureFilter::eLinear, "Incorrect mag filter");
    if (min != m_minFilter || mag != m_magFilter)
    {
        if (m_sampler)
        {
            m_sampler->setFilterType(ETextureFilterGL[m_minFilter], ETextureFilterGL[mag]);
            m_minFilter = min;
            m_magFilter = mag;
        }
        else
        {
            LOG_WARNING("TextureGL::setWrap: Sampler is nullptr");
        }
    }
}

void CTextureGL::setWrap(EWrapType wrap)
{
    if (wrap != m_wrap)
    {
        if (m_sampler)
        {
            m_sampler->setWrap(EWrapTypeGL[wrap]);
            m_wrap = wrap;
        }
        else
        {
            LOG_WARNING("TextureGL::setWrap: Sampler is nullptr");
        }
    }
}

void CTextureGL::setAnisotropicLevel(EAnisotropic level)
{
    if (level != m_anisotropicLevel)
    {
        if (m_sampler)
        {
            m_sampler->setAnisotropicLevel(level);
            m_anisotropicLevel = level;
        }
        else
        {
            LOG_WARNING("TextureGL::setAnisotropicLevel: Sampler is nullptr");
        }
    }
}

ETextureTarget CTextureGL::getTarget() const
{
    return m_target;
}

ETextureFilter CTextureGL::getMinFiler() const
{
    return m_minFilter;
}

ETextureFilter CTextureGL::getMagFiler() const
{
    return m_magFilter;
}

EWrapType CTextureGL::getWrap() const
{
    return m_wrap;
}

EAnisotropic CTextureGL::getAnisotropic() const
{
    return m_anisotropicLevel;
}

u32 CTextureGL::getMipmapLevel() const
{
    return m_mipmapLevel;
}

const core::Dimension3D& CTextureGL::getSize() const
{
    return m_size;
}

EImageFormat CTextureGL::getFormat() const
{
    return m_format;
}

EImageType CTextureGL::getType() const
{
    return m_type;
}

void CTextureGL::fill(const void* data, u32 offset, u32 size, u32 level)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL
    ASSERT(offset < size || size <= m_size.width, "Invalid Size");

    if (m_target == ETextureTarget::eTextureBuffer)
    {
        u32 format = EImageFormatGL[toEnumType(m_format)];
        u32 type = EImageTypeGL[toEnumType(m_type)];

        if (offset > 0 || size > 0)
        {
            glClearBufferSubData(GL_TEXTURE_BUFFER, internalFormat(format, type), offset, size, format, type, data);
        }
        else
        {
            glClearBufferData(GL_TEXTURE_BUFFER, internalFormat(format, type), format, type, data);
        }
        RENDERER->checkForErrors("CTextureGL::fillTexture Buffer texture Error");
        return;
    }
    else if (m_target == ETextureTarget::eTexture1D)
    {
        if (offset > 0 || size > 0)
        {
            glClearTexSubImage(m_textureID, level, offset, 0, 0, size, 1, 1, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
        }
        else
        {
            glClearTexImage(m_textureID, level, EImageFormatGL[toEnumType(m_format)], EImageTypeGL[toEnumType(m_type)], data);
        }
    }
    RENDERER->checkForErrors("CTextureGL::fillTexture 1D Error");
}

void CTextureGL::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 level)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL
    ASSERT(offset.width < size.width || offset.height < size.height ||
        size.width <= m_size.width || size.height <= m_size.height, "Invalid Size");

    u32 format = EImageFormatGL[toEnumType(m_format)];
    u32 type = EImageTypeGL[toEnumType(m_type)];

    if (m_target == ETextureTarget::eTextureCubeMap)
    {
        glClearTexSubImage(m_textureID, level, offset.width, offset.height, 0,
            size.width, size.height, TEXTURE_CUBE_MAP_COUNT, format, type, data);

        RENDERER->checkForErrors("CTextureGL::fillTexture CubeMap Error");
        return;
    }
    else if (m_target == ETextureTarget::eTexture2D || m_target == ETextureTarget::eTexture2DArray)
    {
        if (offset.width > 0 || offset.height > 0 || size.width > 0 || size.height > 0)
        {
            glClearTexSubImage(m_textureID, level, offset.width, offset.height, 0,
                size.width, size.height, 1, format, type, data);
        }
        else
        {
            glClearTexImage(m_textureID, level, format, type, data);
        }
    }
    RENDERER->checkForErrors("CTextureGL::fillTexture 2D Error");
}

void CTextureGL::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 level)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL

    //TODO: will need to add operator<, operator> for Dimension class
    ASSERT(offset.width < size.width || offset.height < size.height || offset.depth < size.depth ||
        size.width <= m_size.width || size.height <= m_size.height || size.depth <= m_size.depth, "Invalid Size");

    u32 format = EImageFormatGL[toEnumType(m_format)];
    u32 type = EImageTypeGL[toEnumType(m_type)];

    if (m_target == ETextureTarget::eTexture3D || m_target == ETextureTarget::eTexture2DArray)
    {
        if (offset.width > 0 || offset.height > 0 || offset.depth > 0
            || size.width > 0 || size.height > 0 || size.depth > 0)
        {
            glClearTexSubImage(m_textureID, level, offset.width, offset.height, offset.depth,
                size.width, size.height, size.depth, format, type, data);
        }
        else
        {
            glClearTexImage(m_textureID, level, format, type, data);
        }
    }
    RENDERER->checkForErrors("CTextureGL::fillTexture 3D Error");
}

void CTextureGL::setSampler(SamplerGL* sampler)
{
    m_sampler = sampler;
}

bool CTextureGL::bindTexture(ETextureTarget target, u32 texture)
{
    if (s_currentTextureID[target] != texture)
    {
        glBindTexture(ETextureTargetGL[target], texture);
#ifdef _DEBUG_GL
        ASSERT((glIsTexture(texture) || texture == 0), "Invalid Texture index");
        RENDERER->checkForErrors("CTextureGL: bindTexture Error");
#endif //_DEBUG_GL
        s_currentTextureID[target] = texture;

        return true;
    }

    return false;
}

void CTextureGL::bindTexBuffer(u32 format, u32 texture, u32 buffer, u32 offset, u32 size)
{
    CTextureGL::bindTexture(ETextureTarget::eTextureBuffer, texture);
    if (offset != 0 || size != 0)
    {
        glTexBufferRange(ETextureTargetGL[ETextureTarget::eTextureBuffer], format, buffer, offset, size);
    }
    else
    {
        glTexBuffer(ETextureTargetGL[ETextureTarget::eTextureBuffer], format, buffer);
    }

    RENDERER->checkForErrors("CTextureGL::bindTexBuffer Error");
}

bool CTextureGL::bindTextureUnit(u32 unit)
{
    if (s_currentUnitID != unit)
    {
#ifdef _DEBUG_GL
        ASSERT(unit < DRIVER_CONTEXT->getTextureUnitsCount(), "Not supported count texture units");
#endif //_DEBUG_GL
        glActiveTexture(GL_TEXTURE0 + unit);
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CTextureGL: bindTextureUnit Error");
#endif //_DEBUG_GL
        s_currentUnitID = unit;

        return true;
    }

    return false;
}

s32 CTextureGL::getActiveTexture(u32 target)
{
    s32 texture;
    glGetIntegerv(target, &texture);

    return texture;
}

s32 CTextureGL::getActiveTextureUnit()
{
    s32 unit;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &unit);

    return unit;
}

void CTextureGL::freeMemory(u32 texture)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(texture), "Invalid Texture index");
#endif //_DEBUG_GL
    switch (m_target)
    {
    case ETextureTarget::eTexture2DMSAA:
    case ETextureTarget::eTexture3DMSAA:
    case ETextureTarget::eTextureBuffer:
    case ETextureTarget::eTextureRectangle:

        glInvalidateTexImage(texture, 0);
        return;

    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTexture1DArray:
    case ETextureTarget::eTexture3D:
    case ETextureTarget::eTexture2DArray:
    case ETextureTarget::eTextureCubeMap:

        for (u32 i = 0; i < m_mipmapLevel; ++i)
        {
            glInvalidateTexImage(texture, i);
            return;
        }

    default:
        ASSERT(false, "Invalid target");
    }
}

void CTextureGL::copyData(const TexturePtr& texture)
{
    u32 id = static_cast<CTextureGL*>(texture->getImpl().get())->getTextureID();
    for (u32 i = 0; i < m_mipmapLevel; ++i)
    {
        glCopyImageSubData(id, ETextureTargetGL[texture->getTarget()], i, 0, 0, 0, 
            m_textureID, ETextureTargetGL[m_target], i, 0, 0, 0, m_size.width, m_size.height, m_size.depth);
    }

    RENDERER->checkForErrors("CTextureGL::copyData Error");
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

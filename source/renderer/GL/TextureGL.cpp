#include "TextureGL.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "BufferGL.h"
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
    GL_TEXTURE_2D,
    GL_TEXTURE_2D_MULTISAMPLE,
    GL_TEXTURE_3D,
    GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_BUFFER
};

GLenum ETextureBindingGL[ETextureTarget::eTargetCount] =
{
    GL_TEXTURE_BINDING_1D,
    GL_TEXTURE_BINDING_2D,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE,
    GL_TEXTURE_BINDING_3D,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,
    GL_TEXTURE_BINDING_CUBE_MAP,
    GL_TEXTURE_BINDING_BUFFER,
};

GLenum ETextureFilterGL[ETextureFilter::eFilterCount] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR,
};

GLenum EWrapTypeGL[EWrapType::eWrapCount] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
};

GLenum EImageFormatGL[EImageFormat::eFormatCount] =
{
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_BGR,
    GL_BGRA,
    GL_DEPTH_COMPONENT,
};

GLenum EImageTypeGL[EImageType::eImageTypeCount] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT_5_6_5,
    GL_UNSIGNED_SHORT_4_4_4_4,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT
};

auto internalFormat = [](u32 format, u32 type) -> u32
{
    switch (type)
    {

    //BYTE
    case GL_BYTE:
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
        case GL_STENCIL:
            return GL_STENCIL_INDEX8;
        default:
            return format;
        };
    }

    //SHORT
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R16;
        case GL_RG:
            return GL_RG16;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB16;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA16;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT16;
        case GL_STENCIL:
            return GL_STENCIL_INDEX16;
        default:
            return format;
        };
    }

    //UNSIGNED_SHORT_5_6_5
    case GL_UNSIGNED_SHORT_5_6_5:
    {
        switch (format)
        {
        case GL_RGB:
        case GL_BGR:
            return GL_RGB565;
        default:
            return format;
        };
    }

    //UNSIGNED_SHORT_4_4_4_4
    case GL_UNSIGNED_SHORT_4_4_4_4:
    {
        switch (format)
        {
        case GL_RGB:
        case GL_BGR:
            return GL_RGB4;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA4;
        default:
            return format;
        };
    }

    //INT
    case GL_INT:
    case GL_UNSIGNED_INT:
    {
        switch (format)
        {
        case GL_R:
        case GL_RED:
            return GL_R32I;
        case GL_RG:
            return GL_RG32I;
        case GL_RGB:
        case GL_BGR:
            return GL_RGB32I;
        case GL_RGBA:
        case GL_BGRA:
            return GL_RGBA32I;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT24;
        default:
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
            return format;
        };
    }
    };
    return format;
};

auto typeSize = [](u32 type) -> s32
{
    switch (type)
    {
    case GL_BYTE:
        return sizeof(s8);

    case GL_UNSIGNED_BYTE:
        return sizeof(u8);

    case GL_SHORT:
        return sizeof(s16);

    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
        return sizeof(u16);

    case GL_INT:
        return sizeof(s32);

    case GL_UNSIGNED_INT:
        return sizeof(u32);

    case GL_FLOAT:
        return sizeof(f32);

    default:
        return 0;
    }
};

auto componentCount = [](u32 format) -> u32
{
    switch (format)
    {
    case GL_R:
    case GL_RED:
        return 1U;

    case GL_RG:
        return 2U;

    case GL_RGB:
    case GL_BGR:
        return 3U;

    case GL_RGBA:
    case GL_BGRA:
        return 4U;

    default:
        return 0U;
    }
};

u32 CTextureGL::s_currentTextureID[] = { 0 };
u32 CTextureGL::s_currentLayerID   = 0;
u32 CTextureGL::s_currentSamplerID = 0;

CTextureGL::CTextureGL(bool useTextureBuffer)
    : m_textureID(0)
    , m_samplerID(0)

    , m_textureBuffer(nullptr)
    , m_initialized(false)
{
    LOG_DEBUG("CTextureGL::CTextureGL constructor %x", this);
}

CTextureGL::~CTextureGL()
{
    CTextureGL::destroy();

    if (m_textureBuffer)
    {
        delete m_textureBuffer;
        m_textureBuffer = nullptr;
    }

    ASSERT(m_textureID == 0, "Texture doesn't deleted");
    ASSERT(m_samplerID == 0, "Sampler doesn't deleted");
    LOG_DEBUG("CTextureGL::CTextureGL destructor %x", this);
}

void CTextureGL::bind(u32 layer, u32 sampler)
{
    if (!m_initialized)
    {
        return;
    }

    CTextureGL::activeTextureLayer(layer);
    //CTextureGL::bindSampler(sampler, m_samplerID);

    if (m_textureBuffer)
    {
        u32 format = EImageFormatGL[m_data.front()._format];
        u32 type = EImageTypeGL[m_data.front()._type];

        CTextureGL::bindTexBuffer(internalFormat(format, type), m_textureID, m_textureBuffer->getID());
        RENDERER->checkForErrors("CTextureGL: Bind Texture Buffer Error");
    }
    else
    {
        CTextureGL::bindTexture(m_target, m_textureID);
        RENDERER->checkForErrors("CTextureGL: Bind Texture Error");
    }
}

void CTextureGL::unbind(u32 layer, u32 sampler)
{
    CTextureGL::bindSampler(sampler, 0);
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
    CTextureGL::activeTextureLayer(0);
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
            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_data.front()._size.width <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage1D(ETextureTargetGL[eTexture1D], m_mipmapLevel + 1, internalFormat(format, type), m_data.front()._size.width);
            RENDERER->checkForErrors("CTextureGL::setData eTexture1D Error");

            if (m_data.front()._data)
            {
                CTextureGL::updateData(0, m_data.front()._size.width, m_data.front()._data);
                m_data.front().free();
            }
            success = true;
        }
        break;

        case ETextureTarget::eTexture2D:
        {
            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

           /* glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexImage2D(ETextureTargetGL[eTexture2D], 0, GL_RGBA, m_data.front()._size.width, m_data.front()._size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);*/

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_data.front()._size.width <= maxSize && (s32)m_data.front()._size.height <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage2D(ETextureTargetGL[eTexture2D], m_mipmapLevel + 1, internalFormat(format, type), m_data.front()._size.width, m_data.front()._size.height);
            RENDERER->checkForErrors("CTextureGL::setData eTexture2D Error");

            if (m_data.front()._data)
            {
                CTextureGL::updateData(Dimension2D(0, 0), Dimension2D(m_data.front()._size.width, m_data.front()._size.height), m_data.front()._data);
                m_data.front().free();
            }
            success = true;
        }
        break;

        case ETextureTarget::eTexture2DMSAA:
        {
            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            ASSERT(!m_data.front()._data, "Multisample used only for render target");
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_data.front()._size.width <= maxSize && (s32)m_data.front()._size.height <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL

            u32 samplesSize = DRIVER_CONTEXT->getSamplesCount(); //TODO: maybe need set fixed value
#ifdef _DEBUG_GL
            GLint maxSamples;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            ASSERT((s32)samplesSize <= maxSamples, "Invalid Samples count");
#endif //_DEBUG_GL
            glTexStorage2DMultisample(ETextureTargetGL[eTexture2DMSAA], samplesSize, internalFormat(format, type),
                m_data.front()._size.width, m_data.front()._size.height, GL_TRUE);
            RENDERER->checkForErrors("CTextureGL::setData eTexture2DMSAA Error");

            success = true;
        }
        break;

        case ETextureTarget::eTexture3D:
        {
            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_data.front()._size.width <= maxSize && (s32)m_data.front()._size.height <= maxSize &&
                (s32)m_data.front()._size.depth <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            glTexStorage3D(ETextureTargetGL[eTexture3D], m_mipmapLevel + 1, internalFormat(format, type),
                m_data.front()._size.width, m_data.front()._size.height, m_data.front()._size.depth);
            RENDERER->checkForErrors("CTextureGL::setData eTexture3D Error");

            if (m_data.front()._data)
            {
                CTextureGL::updateData(Dimension3D(0, 0, 0), m_data.front()._size, m_data.front()._data);
                m_data.front().free();
            }
            success = true;
        }
        break;

        case ETextureTarget::eTexture3DMSAA:
        {
            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            ASSERT(!m_data.front()._data, "Multisample used only for render target");
#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
            ASSERT((s32)m_data.front()._size.width <= maxSize && (s32)m_data.front()._size.height <= maxSize && 
                (s32)m_data.front()._size.depth <= maxSize, "Size greater than max value");
#endif //_DEBUG_GL

            u32 samplesSize = DRIVER_CONTEXT->getSamplesCount(); //TODO: maybe need set fixed value
#ifdef _DEBUG_GL
            GLint maxSamples;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            ASSERT((s32)samplesSize <= maxSamples, "Invalid Samples count");
#endif //_DEBUG_GL
            glTexStorage3DMultisample(ETextureTargetGL[eTexture3DMSAA], samplesSize, internalFormat(format, type),
                m_data.front()._size.width, m_data.front()._size.height, m_data.front()._size.depth, GL_TRUE);
            RENDERER->checkForErrors("CTextureGL::initTexture3DMSAA error");

            success = true;
        }
        break;

        case ETextureTarget::eTextureCubeMap:
        {
            ASSERT(m_data.size() == TEXTURE_CUBE_MAP_COUNT, "Data have incorret count layer");
            for (u32 cubemapSide = 0; cubemapSide < TEXTURE_CUBE_MAP_COUNT; ++cubemapSide)
            {
                u32 format = EImageFormatGL[m_data[cubemapSide]._format];
                u32 type = EImageTypeGL[m_data[cubemapSide]._type];

                glPixelStorei(GL_UNPACK_ALIGNMENT, typeSize(type));
                RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

                glTexImage2D(ECubeMapGL[cubemapSide], 0, internalFormat(format, type), m_data[cubemapSide]._size.width, m_data[cubemapSide]._size.height, 
                    0, format, type, m_data[cubemapSide]._data);
                RENDERER->checkForErrors("CTextureGL::setData eTextureCubeMap Error");
                m_data[cubemapSide].free();
            }

            success = true;
        }
        break;

        case ETextureTarget::eTextureBuffer:
        {
            LOG_DEBUG("CTextureGL::create: Store to buffer");
            m_textureBuffer = new BufferGL(EBufferTarget::eTextureStoreBuffer);

            ASSERT(m_data.size() == 1, "Data have incorret count layer");
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];
            s32 dataSize = m_data.front()._size.getArea() * typeSize(type) * componentCount(format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->setData(EDataUsageType::eDrawStatic, dataSize, m_data.front()._data);
            RENDERER->checkForErrors("CTextureGL::setData eTextureBuffer Error");

            if (m_data.front()._data)
            {
                CTextureGL::updateData(Dimension3D(0, 0, 0), m_data.front()._size, m_data.front()._data);
                m_data.front().free();
            }
            success = true;
        }
        break;

        default:
            ASSERT(false, "Wrong target");
            return success;
    }

    glGenSamplers(1, &m_samplerID);
    CTextureGL::bindSampler(m_textureID, m_samplerID);
    CTextureGL::wrapSampler(m_samplerID, m_wrap);
    CTextureGL::anisotropicSampler(m_samplerID, m_anisotropicLevel);

    if (m_target == ETextureTarget::eTexture1D 
        || m_target == ETextureTarget::eTexture2D
        || m_target == ETextureTarget::eTexture3D
        || m_target == ETextureTarget::eTextureCubeMap)
    {
        if (m_mipmapLevel > 0 && m_minFilter > ETextureFilter::eLinear)
        {
            CTextureGL::filterSampler(m_samplerID, m_minFilter, m_magFilter);

            glTexParameteri(ETextureTargetGL[m_target], GL_TEXTURE_MAX_LEVEL, m_mipmapLevel);
            glGenerateMipmap(ETextureTargetGL[m_target]);
            RENDERER->checkForErrors("CTextureGL: GenerateMipmap Error");
        }
        else
        {
            CTextureGL::filterSampler(m_samplerID, m_minFilter > ETextureFilter::eLinear ? ETextureFilter::eLinear : m_minFilter, m_magFilter);
        }
    }

    if (originalTexture > 0)
    {
        CTextureGL::bindTexture(m_target, originalTexture);
    }
    else
    {
        CTextureGL::bindTexture(m_target, 0);
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
    CTextureGL::bindSampler(m_target, 0);
    if (m_samplerID > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
        glDeleteSamplers(1, &m_samplerID);
        m_samplerID = 0;
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

bool CTextureGL::isValid()
{
    return m_textureID > 0;
}

void CTextureGL::updateData(u32 offset, u32 size, void* data)
{
    ASSERT(size <= m_data.front()._size.width, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            glPixelStorei(GL_UNPACK_ALIGNMENT, typeSize(type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage1D(ETextureTargetGL[eTexture1D], 0, offset, size, format, type, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture1D Error");
            return;
        }

        case ETextureTarget::eTextureBuffer:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            s32 dataSize = size * typeSize(type) * componentCount(format);
            s32 dataOffset = offset * typeSize(type) * componentCount(format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->updateData(dataOffset, dataSize, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureBuffer Error");
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }
}

void CTextureGL::updateData(const Dimension2D& offset, const Dimension2D& size, void* data, u32 cubemapSide)
{
    ASSERT(size.width <= m_data.front()._size.width && size.height <= m_data.front()._size.height, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture2D:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            glPixelStorei(GL_UNPACK_ALIGNMENT, typeSize(type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage2D(ETextureTargetGL[eTexture2D], 0, offset.width, offset.height, size.width, size.height, format, type, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture2D Error");
            return;
        }

        case ETextureTarget::eTexture2DMSAA:
            ASSERT(false, "For multisample use set data");
            return;

        case ETextureTarget::eTextureCubeMap:
        {
            ASSERT(cubemapSide < TEXTURE_CUBE_MAP_COUNT, "Wrong cubemap side");
            s32 index = (cubemapSide < TEXTURE_CUBE_MAP_COUNT) ? cubemapSide : 0;

            u32 format = EImageFormatGL[m_data[index]._format];
            u32 type = EImageTypeGL[m_data[index]._type];

            glPixelStorei(GL_UNPACK_ALIGNMENT, typeSize(type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage2D(ECubeMapGL[index], 0, offset.width, offset.height, size.width, size.height, format, type, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureCubeMap Error");
            return;
        }

        case ETextureTarget::eTextureBuffer:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            s32 dataSize = size.width * size.height * typeSize(type) * componentCount(format);
            s32 dataOffset = offset.width * offset.height * typeSize(type) * componentCount(format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->updateData(dataOffset, dataSize, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureBuffer Error");
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }

    RENDERER->checkForErrors("CTextureGL::readData Error");
}

void CTextureGL::updateData(const Dimension3D& offset, const Dimension3D& size, void* data)
{
    ASSERT(size.width <= m_data.front()._size.width && size.height <= m_data.front()._size.height && size.depth <= m_data.front()._size.depth, "Different size");
    ASSERT(data, "Data nullptr");
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture3D:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            glPixelStorei(GL_UNPACK_ALIGNMENT, typeSize(type));
            RENDERER->checkForErrors("CTextureGL::Pixel Store Error");

            glTexSubImage3D(ETextureTargetGL[m_target], 0, offset.width, offset.height, offset.depth, size.width, size.height, size.depth, format, type, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTexture3D Error");
            return;
        }

        case ETextureTarget::eTexture3DMSAA:
            ASSERT(false, "For multisample use set data");
            return;

        case ETextureTarget::eTextureBuffer:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            s32 dataSize = size.getArea() * typeSize(type) * componentCount(format);
            s32 dataOffset = offset.getArea() * typeSize(type) * componentCount(format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->updateData(dataOffset, dataSize, data);
            RENDERER->checkForErrors("CTextureGL::updateData eTextureBuffer Error");
            return;
        }

        default:
            ASSERT(false, "Wrong target");
    }
}

void CTextureGL::readData(void* data, u32 cubemapSide)
{
    CTextureGL::bindTexture(m_target, m_textureID);
    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture3D:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            glGetTexImage(ETextureTargetGL[m_target], 0, format, type, data);
            RENDERER->checkForErrors("CTextureGL::readData Error");

            return;
        }

        case ETextureTarget::eTexture2DMSAA:
        case ETextureTarget::eTexture3DMSAA:
            ASSERT(false, "Multisample doesn't supported");

            return;

        case ETextureTarget::eTextureCubeMap:
        {
            ASSERT(cubemapSide > TEXTURE_CUBE_MAP_COUNT, "Wrong cubemap side");
            s32 index = (cubemapSide > TEXTURE_CUBE_MAP_COUNT) ? 0 : cubemapSide;

            u32 format = EImageFormatGL[m_data[index]._format];
            u32 type = EImageTypeGL[m_data[index]._type];

            glGetTexImage(ECubeMapGL[index], 0, format, type, data);
            return;
        }

        case ETextureTarget::eTextureBuffer:
        {
            u32 format = EImageFormatGL[m_data.front()._format];
            u32 type = EImageTypeGL[m_data.front()._type];

            s32 dataSize = m_data.front()._size.getArea() * typeSize(type) * componentCount(format);

#ifdef _DEBUG_GL
            GLint maxSize;
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxSize);
            ASSERT(dataSize < maxSize, "Size greater than max value");
#endif //_DEBUG_GL
            ASSERT(m_textureBuffer, "TBO doesn't exist");
            m_textureBuffer->readData(0, dataSize, data); //TODO: maybe need to use mapper
            RENDERER->checkForErrors("CTextureGL::readData buffer Error");

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

u32 CTextureGL::getSamplerID() const
{
    return m_samplerID;
}

void CTextureGL::setFilterType(ETextureFilter min, ETextureFilter mag)
{
    if (min != m_minFilter || mag != m_magFilter)
    {
        CTextureGL::bindSampler(m_textureID, m_samplerID);
        CTextureGL::filterSampler(m_samplerID, min, mag);
        m_minFilter = min;
        m_magFilter = mag;
    }
}

void CTextureGL::setWrap(EWrapType wrap)
{
    if (wrap != m_wrap)
    {
        CTextureGL::bindSampler(m_textureID, m_samplerID);
        CTextureGL::wrapSampler(m_samplerID, wrap);
        m_wrap = wrap;
    }
}

void CTextureGL::setAnisotropicLevel(EAnisotropic level)
{
    if (level != m_anisotropicLevel)
    {
        CTextureGL::bindSampler(m_textureID, m_samplerID);
        CTextureGL::anisotropicSampler(m_samplerID, level);
        m_anisotropicLevel = level;
    }
}

void CTextureGL::fill(u32 offset, u32 size, void* data)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL
    ASSERT(offset < size || size <= m_data.front()._size.width, "Invalid Size");

    u32 format = EImageFormatGL[m_data.front()._format];
    u32 type = EImageTypeGL[m_data.front()._type];

    if (offset > 0 || size > 0)
    {
        glClearTexSubImage(m_textureID, 0, offset, 0, 0, size, 1, 1, format, type, data);
    }
    else
    {
        glClearTexImage(m_textureID, 0, format, type, data);
    }

    RENDERER->checkForErrors("CTextureGL::fillTexture 1D Error");
}

void CTextureGL::fill(const Dimension2D& offset, const Dimension2D& size, void* data)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL
    ASSERT(offset.width < size.width || offset.height < size.height ||
        size.width <= m_data.front()._size.width || size.height <= m_data.front()._size.height, "Invalid Size");

    u32 format = EImageFormatGL[m_data.front()._format];
    u32 type = EImageTypeGL[m_data.front()._type];

    if (m_target == ETextureTarget::eTextureCubeMap)
    {
        glClearTexSubImage(m_textureID, 0, offset.width, offset.height, 0,
            size.width, size.height, TEXTURE_CUBE_MAP_COUNT, format, type, data);

        RENDERER->checkForErrors("CTextureGL::fillTexture CubeMap Error");
        return;
    }

    if (offset.width > 0 || offset.height > 0 || size.width > 0 || size.height > 0)
    {
        glClearTexSubImage(m_textureID, 0, offset.width, offset.height, 0,
            size.width, size.height, 1, format, type, data);
    }
    else
    {
        glClearTexImage(m_textureID, 0, format, type, data);
    }

    RENDERER->checkForErrors("CTextureGL::fillTexture 2D Error");
}

void CTextureGL::fill(const Dimension3D& offset, const Dimension3D& size, void* data)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(m_textureID), "Invalid Texture index");
#endif //_DEBUG_GL

    //TODO: will need to add operator<, operator> for Dimension class
    ASSERT(offset.width < size.width || offset.height < size.height || offset.depth < size.depth ||
        size.width <= m_data.front()._size.width || size.height <= m_data.front()._size.height || size.depth <= m_data.front()._size.depth, "Invalid Size");

    u32 format = EImageFormatGL[m_data.front()._format];
    u32 type = EImageTypeGL[m_data.front()._type];

    if (offset.width > 0 || offset.height > 0 || offset.depth > 0 
        || size.width > 0 || size.height > 0 || size.depth > 0)
    {
        glClearTexSubImage(m_textureID, 0, offset.width, offset.height, offset.depth,
            size.width, size.height, size.depth, format, type, data);
    }
    else
    {
        glClearTexImage(m_textureID, 0, format, type, data);
    }

    RENDERER->checkForErrors("CTextureGL::fillTexture 3D Error");
}

bool CTextureGL::bindTexture(ETextureTarget target, u32 texture)
{
    if (s_currentTextureID[target] != texture)
    {
        glBindTexture(ETextureTargetGL[target], texture);
#ifdef _DEBUG_GL
        ASSERT((glIsTexture(texture) || texture == 0), "Invalid Texture index");
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

bool CTextureGL::activeTextureLayer(u32 layer)
{
    if (s_currentLayerID != layer)
    {
#ifdef _DEBUG_GL
        ASSERT(ETextureLayer::eLayerMax >= layer, "Not supported count texture units");
#endif //_DEBUG_GL
        glActiveTexture(GL_TEXTURE0 + layer);
        s_currentLayerID = layer;

        return true;
    }

    return false;
}

void CTextureGL::freeTexture(u32 texture)
{
#ifdef _DEBUG_GL
    ASSERT(glIsTexture(texture), "Invalid Texture index");
#endif //_DEBUG_GL
    switch (m_target)
    {
    case ETextureTarget::eTexture2DMSAA:
    case ETextureTarget::eTexture3DMSAA:
    case ETextureTarget::eTextureBuffer:
    case ETextureTarget::eTextureCubeMap:

        glInvalidateTexImage(texture, 0);
        return;

    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTexture3D:

        for (u32 i = 0; i < m_mipmapLevel; ++i)
        {
            glInvalidateTexImage(texture, i);
            return;
        }

    default:
        ASSERT(false, "Invalid target");
    }
}

void CTextureGL::wrapSampler(u32 sampler, EWrapType wrap)
{
#ifdef _DEBUG_GL
    ASSERT(glIsSampler(sampler), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, EWrapTypeGL[wrap]);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, EWrapTypeGL[wrap]);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, EWrapTypeGL[wrap]);

    RENDERER->checkForErrors("CTextureGL::wrapSampler Error");
}

void CTextureGL::filterSampler(u32 sampler, ETextureFilter min, ETextureFilter mag)
{
#ifdef _DEBUG_GL
    ASSERT(glIsSampler(sampler), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, ETextureFilterGL[mag]);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, ETextureFilterGL[min]);

    RENDERER->checkForErrors("CTextureGL::filterSampler Error");
}

void CTextureGL::anisotropicSampler(u32 sampler, u32 level)
{
#ifdef _DEBUG_GL
    GLfloat largest = DRIVER_CONTEXT->getMaxAnisotropySize();
    ASSERT(largest >= level, "Anisotropy level out the range");
    ASSERT(glIsSampler(sampler), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)level);

    RENDERER->checkForErrors("CTextureGL::anisotropicSampler Error");
}

bool CTextureGL::bindSampler(u32 texture, u32 sampler)
{
    if (s_currentSamplerID != sampler)
    {
        glBindSampler(texture, sampler);
#ifdef _DEBUG_GL
        ASSERT((glIsSampler(sampler) || sampler == 0), "Invalid Sampler index");
        ASSERT((glIsTexture(texture) || texture == 0), "Invalid Texture index");
#endif //_DEBUG_GL
        s_currentSamplerID = sampler;

        return true;
    }

    return false;
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

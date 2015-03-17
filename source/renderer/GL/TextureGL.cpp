#include "TextureGL.h"
#include "Engine.h"

#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

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
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP,
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
    GL_COLOR_INDEX,
    GL_ALPHA,
    GL_RGB,
    GL_RGBA,
    GL_BGR,
    GL_BGRA,
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA,
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
    GL_FLOAT,
    GL_DOUBLE,
    GL_HALF_FLOAT,
};

u32 CTextureGL::s_currentTextureID[] = { 0 };
u32 CTextureGL::s_currentLayerID   = 0;
u32 CTextureGL::s_currentSamplerID = 0;

CTextureGL::CTextureGL()
    : m_samplerID(0)
{
}

CTextureGL::~CTextureGL()
{
    CTextureGL::destroy();
}

void CTextureGL::bind(u32 layer)
{
    CTextureGL::activeTextureLayer(layer);
    CTextureGL::bindSampler(m_textureID, m_samplerID);
    CTextureGL::bindTexture(m_target, m_textureID);

    RENDERER->checkForErrors("CTextureGL: Bind Texture Error");
}

void CTextureGL::unbind(u32 layer)
{
    CTextureGL::bindSampler(0, m_samplerID);
    CTextureGL::bindTexture(m_target, 0);

    RENDERER->checkForErrors("CTextureGL: Unbind Texture Error");
}

void CTextureGL::reset()
{
    CTextureGL::activeTextureLayer(0);
    CTextureGL::bindSampler(0, 0);
    for (u32 i = 0; i < eTargetCount; ++i)
    {
        CTextureGL::bindTexture((ETextureTarget)i, 0);
    }

    RENDERER->checkForErrors("CTextureGL: Unbind All Texture Error");
}

bool CTextureGL::create()
{
    bool success = false;

    CTextureGL::genTexture(m_textureID);

    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        {
            CTextureGL::initTexture1D(m_textureID);
            success = true;
        }
        break;

        case ETextureTarget::eTexture2D:
        {
            CTextureGL::initTexture2D(m_textureID);
            success = true;
        }
        break;

        case ETextureTarget::eTexture3D:
        {
            CTextureGL::initTexture3D(m_textureID);
            success = true;
        }
        break;

        case ETextureTarget::eTextureCubeMap:
        {
            CTextureGL::initTextureCubeMap(m_textureID);
            success = true;
        }
        break;

        default:
            break;
    }

    CTextureGL::bindTexture(m_target, m_textureID);

    CTextureGL::genSampler(m_samplerID);
    CTextureGL::bindSampler(m_textureID, m_samplerID);
    CTextureGL::wrapSampler(m_samplerID, m_wrap);
    CTextureGL::filterSampler(m_samplerID, m_minFilter, m_magFilter);
    CTextureGL::anisotropicSampler(m_samplerID, m_anisotropicLevel);
    CTextureGL::bindSampler(0, m_samplerID);
    
    if (m_minFilter > ETextureFilter::eLinear)
    {
        CTextureGL::generateMipmap(m_target);
    }

    CTextureGL::bindTexture(m_target, 0);

    RENDERER->checkForErrors("CTextureGL: Create Texture Error");

    if (success)
    {
        m_enable = true;
    }

    return success;
}

void CTextureGL::destroy()
{
    CTextureGL::deleteSampler(m_samplerID);
    CTextureGL::deleteTexture(m_textureID);
}

void CTextureGL::copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data)
{
    CTextureGL::bindTexture(eTexture2D, m_textureID);
    glTexSubImage2D(ETextureTargetGL[eTexture2D], 0, offset.width, offset.height, size.width, size.height, EImageFormatGL[format], EImageTypeGL[eUnsignedByte], data);
    CTextureGL::bindTexture(eTexture2D, 0);

    RENDERER->checkForErrors("CTextureGL: Copy Texture Error");
}

bool CTextureGL::bindTexture(ETextureTarget target, u32 texture)
{
    if (texture != 0)
    {
        ASSERT(glIsTexture(texture) || "Invalid Texture index");
    }

    if (s_currentTextureID[target] != texture)
    {
        glBindTexture(ETextureTargetGL[target], texture);
        s_currentTextureID[target] = texture;

        return true;
    }

    return false;
}

bool CTextureGL::activeTextureLayer(u32 layer)
{
    ASSERT(ETextureLayer::eLayerMax >= layer || "Not supported count texture units");
    if (s_currentLayerID != layer)
    {
        glActiveTexture(GL_TEXTURE0 + layer);
        s_currentLayerID = layer;

        return true;
    }

    return false;
}

void CTextureGL::genTexture(u32& texture)
{
    glGenTextures(1, &texture);
    ASSERT(glIsTexture(texture) || "Invalid Texture index");
}

void CTextureGL::deleteTexture(u32 texture)
{
    if (texture > 0)
    {
        ASSERT(glIsTexture(texture) || "Invalid Texture index");
        glDeleteTextures(1, &texture);
    }
}

void CTextureGL::wrapSampler(u32 sampler, EWrapType wrap)
{
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, EWrapTypeGL[wrap]);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, EWrapTypeGL[wrap]);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, EWrapTypeGL[wrap]);
}

void CTextureGL::filterSampler(u32 sampler, ETextureFilter min, ETextureFilter mag)
{
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, ETextureFilterGL[mag]);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, ETextureFilterGL[min]);
}

void CTextureGL::anisotropicSampler(u32 sampler, u32 level)
{
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
    {
        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        ASSERT(fLargest >= level || "Anisotropy level out the range");
        glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)level);
    }
}

void CTextureGL::genSampler(u32& sampler)
{
    glGenSamplers(1, &sampler);
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
}

bool CTextureGL::bindSampler(u32 texture, u32 sampler)
{
    if (sampler != 0)
    {
        ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    }

    if (s_currentSamplerID != sampler)
    {
        glBindSampler(texture, sampler);
        s_currentSamplerID = sampler;

        return true;
    }

    return false;
}

void CTextureGL::deleteSampler(u32 sampler)
{
    if (sampler > 0)
    {
        ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
        glDeleteSamplers(1, &sampler);
    }
}

void CTextureGL::generateMipmap(ETextureTarget target)
{
    glGenerateMipmap(ETextureTargetGL[target]);
}

void CTextureGL::initTexture1D(u32 texture)
{
    CTextureGL::bindTexture(eTexture1D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = EImageFormatGL[m_data[0]._format];

    glTexImage1D(ETextureTargetGL[eTexture1D], 0, format, m_data[0]._width, 0,
        format, EImageTypeGL[m_data[0]._type], m_data[0]._data);
}

void CTextureGL::initTexture2D(u32 texture)
{
    CTextureGL::bindTexture(eTexture2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = EImageFormatGL[m_data[0]._format];
    std::function<s32(s32)> internalFormat = [](s32 format)
    {
        switch (format)
        {
        case GL_RGB:
        case GL_BGR:
            return GL_RGB;

        case GL_RED:
            return GL_R8;

        default:
            return format;
        };

        return format;
    };

    glTexImage2D(ETextureTargetGL[eTexture2D], 0, internalFormat(format), m_data[0]._width, m_data[0]._height, 0,
        format, EImageTypeGL[m_data[0]._type], m_data[0]._data);
}

void CTextureGL::initTexture3D(u32 texture)
{
    CTextureGL::bindTexture(eTexture3D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = EImageFormatGL[m_data[0]._format];

    glTexImage3D(ETextureTargetGL[eTexture3D], 0, format, m_data[0]._width, m_data[0]._height, m_data[0]._depth, 0,
        format, EImageTypeGL[m_data[0]._type], m_data[0]._data);
}

void CTextureGL::initTextureCubeMap(u32 texture)
{
    CTextureGL::bindTexture(eTextureCubeMap, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = EImageFormatGL[m_data[0]._format];

    for (u32 i = 0; i < TEXTURE_CUBE_MAP_COUNT; ++i)
    {
        glTexImage2D(ECubeMapGL[i], 0, format, m_data[i]._width, m_data[i]._height, 0, format,
            EImageTypeGL[m_data[i]._type], m_data[i]._data);
    }
}

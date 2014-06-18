#include "TextureGL.h"
#include "Engine.h"

#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

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

GLenum EImageTypeGL[EImageType::eTypeCount] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_DOUBLE,
    GL_HALF_FLOAT,
};

CTextureGL::CTextureGL()
    : m_samplerID(0)
{
}

CTextureGL::~CTextureGL()
{
}

void CTextureGL::bind(u32 layer)
{
    CTextureGL::bindSampler(ETextureTargetGL[m_target], m_samplerID);
    CTextureGL::activeTextureLayer(layer);
    CTextureGL::bindTexture(m_textureID, ETextureTargetGL[m_target]);

    RENDERER->checkForErrors("Bind Texture Error");
}

void CTextureGL::unbind(u32 layer)
{
    CTextureGL::bindSampler(ETextureTargetGL[m_target], 0);
    CTextureGL::bindTexture(0, ETextureTargetGL[m_target]);

    RENDERER->checkForErrors("Unbind Texture Error");
}

bool CTextureGL::create()
{
    bool success = false;

    CTextureGL::genTexture(m_textureID);
    CTextureGL::bindTexture(ETextureTargetGL[m_target], m_textureID);

    CTextureGL::genSampler(m_samplerID);
    CTextureGL::wrapSampler(m_samplerID, EWrapTypeGL[m_wrap]);
    CTextureGL::filterSampler(m_samplerID, ETextureFilterGL[m_minFilter], ETextureFilterGL[m_magFilter]);
    CTextureGL::anisotropicSampler(m_samplerID, m_anisotropicLevel);

    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        {
            //TODO: eTexture1D
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
          //TODO: eTexture3D
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

    if (m_minFilter > ETextureFilter::eLinear)
    {
        CTextureGL::generateMipmap(ETextureTargetGL[m_target]);
    }

    CTextureGL::bindTexture(ETextureTargetGL[m_target], 0);

    RENDERER->checkForErrors("Create Texture Error");

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

void CTextureGL::bindTexture(u32 target, u32 texture)
{
    if (texture != 0)
    {
        ASSERT(glIsTexture(texture) || "Invalid Texture index");
    }

    glBindTexture(target, texture);
}

void CTextureGL::activeTextureLayer(u32 layer)
{
    ASSERT(ETextureLayer::eLayerMax >= layer || "Not supported count texture units");
    glActiveTexture(GL_TEXTURE0 + layer);
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

void CTextureGL::wrapSampler(u32 sampler, u32 wrap)
{
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, wrap);
}

void CTextureGL::filterSampler(u32 sampler, u32 min, u32 mag)
{
    ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, mag);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, min);
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

void CTextureGL::bindSampler(u32 target, u32 sampler)
{
    if (sampler != 0)
    {
        ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
    }
    glBindSampler(target, sampler);
}

void CTextureGL::deleteSampler(u32 sampler)
{
    if (sampler > 0)
    {
        ASSERT(glIsSampler(sampler) || "Invalid Sampler index");
        glDeleteSamplers(1, &sampler);
    }
}

void CTextureGL::generateMipmap(u32 target)
{
    glGenerateMipmap(target);
}

void CTextureGL::initTexture2D(u32 texture)
{
    CTextureGL::bindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = EImageFormatGL[m_data[0].format];

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

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat(format), m_data[0].width, m_data[0].height, 0, 
        format, EImageTypeGL[m_data[0].type], m_data[0].data);
}

void CTextureGL::initTextureCubeMap(u32 texture)
{

    CTextureGL::bindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (u32 i = 0; i < TEXTURE_CUBE_MAP_COUNT; ++i)
    {
        glTexImage2D(ECubeMapGL[i], 0, GL_RGB, m_data[i].width, m_data[i].height, 0, EImageFormatGL[m_data[i].format],
            EImageTypeGL[m_data[i].type], m_data[i].data);
    }
}
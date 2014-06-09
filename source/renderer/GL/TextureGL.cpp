#include "TextureGL.h"
#include "Engine.h"

#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

#define TEXTURE_CUBE_MAP_COUNT 6

GLenum ECubeMap[TEXTURE_CUBE_MAP_COUNT] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
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
    CTextureGL::bindSampler(getGLTextureTarget(m_target), m_samplerID);
    CTextureGL::activeTextureLayer(layer);
    CTextureGL::bindTexture(m_textureID, getGLTextureTarget(m_target));

    RENDERER->checkForErrors("Bind Texture Error");
}

void CTextureGL::unbind(u32 layer)
{
    CTextureGL::bindSampler(getGLTextureTarget(m_target), 0);
    CTextureGL::bindTexture(0, getGLTextureTarget(m_target));

    RENDERER->checkForErrors("Unbind Texture Error");
}

bool CTextureGL::create()
{
    bool success = false;

    CTextureGL::genTexture(m_textureID);
    CTextureGL::bindTexture(getGLTextureTarget(m_target), m_textureID);

    CTextureGL::genSampler(m_samplerID);
    CTextureGL::wrapSampler(m_samplerID, getGLWrapSampler(m_wrap));
    CTextureGL::filterSampler(m_samplerID, getGLFilterSampler(m_minFilter), getGLFilterSampler(m_magFilter));
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
        CTextureGL::generateMipmap(getGLTextureTarget(m_target));
    }

    CTextureGL::bindTexture(getGLTextureTarget(m_target), 0);

    RENDERER->checkForErrors("Create Texture Error");

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

u32 CTextureGL::getGLFilterSampler(ETextureFilter filter)
{
    switch (filter)
    {
    case ETextureFilter::eNearest:
        return GL_NEAREST;

    case ETextureFilter::eLinear:
        return GL_LINEAR;

    case ETextureFilter::eNearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;

    case ETextureFilter::eNearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;

    case ETextureFilter::eLinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;

    case ETextureFilter::eLinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;

    default:
        return GL_LINEAR;
    }

    return GL_LINEAR;
}

u32 CTextureGL::getGLWrapSampler(EWrapType wrap)
{
    switch (wrap)
    {
    case EWrapType::eRepeat:
        return GL_REPEAT;

    case EWrapType::eMirroredRepeat:
        return GL_MIRRORED_REPEAT;

    case EWrapType::eClampToBorder:
        return GL_CLAMP_TO_BORDER;

    case EWrapType::eClampToEdge:
        return GL_CLAMP_TO_EDGE;

     default:
        return GL_CLAMP_TO_EDGE;
    }

    return GL_CLAMP_TO_EDGE;
}

u32 CTextureGL::getGLTextureTarget(ETextureTarget target)
{
    switch (target)
    {
    case ETextureTarget::eTexture1D:
        return GL_TEXTURE_1D;

    case ETextureTarget::eTexture2D:
        return GL_TEXTURE_2D;

    case ETextureTarget::eTexture3D:
        return GL_TEXTURE_3D;

    case ETextureTarget::eTextureCubeMap:
        return GL_TEXTURE_CUBE_MAP;

    default:
        return GL_TEXTURE_2D;
    }

    return GL_TEXTURE_2D;
}

u32 CTextureGL::getGLImageFormat(EImageFormat format)
{
    switch (format)
    {
    case EImageFormat::eRGB:
        return GL_RGB;

    case EImageFormat::eBGR:
        return GL_BGR;

    case EImageFormat::eRGBA:
        return GL_RGBA;

    case EImageFormat::eBGRA:
        return GL_BGRA;

    case EImageFormat::eAlpha:
        return GL_ALPHA;

    case EImageFormat::eColorIndex:
        return GL_COLOR_INDEX;

    case EImageFormat::eDepthComponent:
        return GL_DEPTH_COMPONENT;

    case EImageFormat::eLumiance:
        return GL_LUMINANCE;

    case EImageFormat::eLuminanceAlpha:
        return GL_LUMINANCE_ALPHA;

    default:
        return GL_RGB;
    };

    return GL_RGB;
}

inline u32 CTextureGL::getGLImageType(EImageType type)
{
    switch (type)
    {
    case EImageType::eByte:
        return GL_BYTE;

    case EImageType::eUnsignedByte:
        return GL_UNSIGNED_BYTE;

    case EImageType::eShort:
        return GL_SHORT;

    case EImageType::eUnsignedShort:
        return GL_UNSIGNED_SHORT;

    case EImageType::eInt:
        return GL_INT;

    case EImageType::eUnsignedInt:
        return GL_UNSIGNED_INT;

    case EImageType::eFloat:
        return GL_FLOAT;

    case EImageType::eDouble:
        return GL_DOUBLE;

    case EImageType::eHalf:
        return GL_HALF_FLOAT;

    default:
        return GL_UNSIGNED_BYTE;
    };

    return GL_UNSIGNED_BYTE;
}

void CTextureGL::initTexture2D(u32 texture)
{
    CTextureGL::bindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    u32 format = getGLImageFormat(m_data[0].format);

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
        format, getGLImageType(m_data[0].type), m_data[0].data);
}

void CTextureGL::initTextureCubeMap(u32 texture)
{

    CTextureGL::bindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (u32 i = 0; i < TEXTURE_CUBE_MAP_COUNT; ++i)
    {
        glTexImage2D(ECubeMap[i], 0, GL_RGB, m_data[i].width, m_data[i].height, 0, getGLImageFormat(m_data[i].format),
            getGLImageType(m_data[i].type), m_data[i].data);
    }
}
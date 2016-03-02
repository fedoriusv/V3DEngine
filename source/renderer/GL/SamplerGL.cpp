#include "SamplerGL.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

u32 SamplerGL::s_currentSamplerID = 0U;

SamplerGL::SamplerGL()
    : m_samplerID(0U)
{
    glGenSamplers(1, &m_samplerID);
    ASSERT(m_samplerID != 0, "Sampler doesn't created");

    LOG_DEBUG("SamplerGL::SamplerGL constructor %x", this);
}

SamplerGL::~SamplerGL()
{
    if (m_samplerID > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
        glDeleteSamplers(1, &m_samplerID);
        m_samplerID = 0;
    }

    LOG_DEBUG("SamplerGL::SamplerGL destructor %x", this);
}

bool SamplerGL::bind(u32 unit)
{
    if (s_currentSamplerID != m_samplerID)
    {
        glBindSampler(unit, m_samplerID);
#ifdef _DEBUG_GL
        ASSERT((glIsSampler(m_samplerID)), "Invalid Sampler index");
#endif //_DEBUG_GL
        s_currentSamplerID = m_samplerID;

        RENDERER->checkForErrors("CTextureGL::anisotropicSampler Error");

        return true;
    }

    return false;
}

bool SamplerGL::unbind(u32 unit)
{
    if (s_currentSamplerID != 0)
    {
        glBindSampler(unit, 0);
        s_currentSamplerID = 0;

        RENDERER->checkForErrors("CTextureGL::anisotropicSampler Error");

        return true;
    }

    return false;
}

void SamplerGL::setFilterType(u32 min, u32 mag)
{
#ifdef _DEBUG_GL
    //ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, min);
    glSamplerParameteri(m_samplerID, GL_TEXTURE_MAG_FILTER, mag);

    RENDERER->checkForErrors("CTextureGL::filterSampler Error");
}

void SamplerGL::setWrap(u32 wrap)
{
#ifdef _DEBUG_GL
    //ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_S, wrap);
    glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_T, wrap);
    glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_R, wrap);

    RENDERER->checkForErrors("CTextureGL::wrapSampler Error");
}

void SamplerGL::setAnisotropicLevel(EAnisotropic level)
{
#ifdef _DEBUG_GL
    GLfloat largest = DRIVER_CONTEXT->getMaxAnisotropySize();
    ASSERT(largest >= level, "Anisotropy level out the range");
    ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameterf(m_samplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)level);

    RENDERER->checkForErrors("CTextureGL::anisotropicSampler Error");
}

void SamplerGL::setTextureCompare(u32 mode, u32 func)
{
#ifdef _DEBUG_GL
    ASSERT(glIsSampler(m_samplerID), "Invalid Sampler index");
#endif //_DEBUG_GL
    glSamplerParameteri(m_samplerID, GL_TEXTURE_COMPARE_MODE, mode);
    glSamplerParameteri(m_samplerID, GL_TEXTURE_COMPARE_FUNC, func);

    RENDERER->checkForErrors("CTextureGL::anisotropicSampler Error");
}

u32 SamplerGL::getSamplerID() const
{
    return m_samplerID;
}

s32 SamplerGL::getActiveSampler()
{
    s32 sampler;
    glGetIntegerv(GL_SAMPLER_BINDING, &sampler);

    return sampler;
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

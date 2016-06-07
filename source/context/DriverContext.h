#ifndef _V3D_DRIVER_CONTEXT_H_
#define _V3D_DRIVER_CONTEXT_H_

#include "common.h"
#include "platform/Window.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace platform
{
    class CPlatform;
} //namespace platform

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Interface Context stored all information about device context.
    */
    class CDriverContext
    {
    public:

        CDriverContext(const platform::WindowPtr& window);
        virtual                     ~CDriverContext();

        virtual void                checkForErrors(const std::string& location = "") = 0;
        virtual bool                createContext()                                  = 0;
        virtual void                destroyContext()                                 = 0;

        virtual bool                setVSync(bool use)                               = 0;
        virtual void                flushBuffers()                                   = 0;

        const core::Dimension2D&    getWindowSize() const;
        const platform::WindowPtr&  getWindow()     const;

        u32                         getSamplesCount()       const;
        u32                         getTextureUnitsCount()  const;
        f32                         getMaxAnisotropySize()  const;
        u32                         getMaxPatchVertices()   const;

        const std::string&          getShaderVersion() const;
        s32                         getRenderMajorVersion() const;
        s32                         getRenderMinorrVersion() const;

    protected:

        friend                      CTexture;

        virtual TexturePtr          createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) = 0;
        virtual TexturePtr          createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)                 = 0;

        friend                      platform::CPlatform;

        virtual void                driverInfo() = 0;

        void                        setSamplesCount(u32 count);
        void                        setTextureUnitsCount(u32 count);
        void                        setMaxAnisotropySize(f32 size);
        void                        setMaxPatchVertices(u32 size);

        std::string                 m_shaderVersion;
        s32                         m_renderMajorVersion;
        s32                         m_renderMinorVersion;

    private:

        const platform::WindowPtr   m_window;

        u32                         m_samplesCount;
        u32                         m_maxTextureUnits;
        f32                         m_maxAnisotropy;
        u32                         m_maxPatchVertices;



    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDriverContext> DriverContextPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_DRIVER_CONTEXT_H_

#ifndef _V3D_DRIVER_CONTEXT_H_
#define _V3D_DRIVER_CONTEXT_H_

#include "common.h"
#include "platform/Window.h"

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
    * Interface DriverContext stored all information about device context.
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
        s32                         getTextureUnitsCount()  const;
        f32                         getMaxAnisotropySize()  const;

        const std::string&          getShaderVersion() const;
        s32                         getRenderMajorVersion() const;
        s32                         getRenderMinorrVersion() const;

    protected:

        friend                      platform::CPlatform;

        virtual void                driverInfo() = 0;

        void                        setSamplesCount(u32 count);
        void                        setTextureUnitsCount(u32 count);
        void                        setMaxAnisotropySize(f32 size);

        std::string                 m_shaderVersion;
        s32                         m_renderMajorVersion;
        s32                         m_renderMinorVersion;

    private:

        const platform::WindowPtr   m_window;

        u32                         m_samplesCount;
        s32                         m_maxTextureUnits;
        f32                         m_maxAnisotropy;



    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDriverContext> DriverContextPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_DRIVER_CONTEXT_H_

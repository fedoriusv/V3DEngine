#ifndef _V3D_DRIVER_CONTEXT_GL_H_
#define _V3D_DRIVER_CONTEXT_GL_H_

#include "DriverContext.h"
#if defined(_PLATFORM_WIN_)
#   include <windows.h>
#elif defined(_PLATFORM_MACOSX_)
#   include <pthread.h>
#endif //_PLATFORM_MACOSX_

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDriverContextGL : public CDriverContext
    {
    public:

        CDriverContextGL(const platform::WindowPtr& window);
        ~CDriverContextGL();

        void                checkForErrors(const std::string& location = "") override;
        bool                createContext()                                  override;
        void                destroyContext()                                 override;
        void                flushBuffers()                                   override;
        
        bool                setVSync(bool use)                               override;

    protected:

        TexturePtr          createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)  override;
        TexturePtr          createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)                  override;

        void                driverInfo()                                     override;

        void                printExtensionList() const;

#if defined(_PLATFORM_WIN_)
        bool                createWin32Context();
        void                destroyWin32Context();

        HGLRC               m_hRc;
#elif defined(_PLATFORM_LINUX_)
        bool                createLinuxContext();
#elif defined(_PLATFORM_MACOSX_)
        bool                createMacOSXContext();
        void                destroyMacOSXContext();
        
        struct SContextNS
        {
            id              _context;
            id              _pixelFormat;
            pthread_key_t   _thread;
        };
        
        SContextNS          m_context;
#endif
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_DRIVER_CONTEXT_GL_H_

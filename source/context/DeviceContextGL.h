#pragma once

#include "DeviceContext.h"

#ifdef _OPENGL_RENDER_
#if defined(_PLATFORM_WIN_)
#include <windows.h>
#elif defined(_PLATFORM_MACOSX_)
#include <pthread.h>
#endif //_PLATFORM_MACOSX_

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class DeviceContextGL final : public DeviceContext
    {
    public:

        explicit            DeviceContextGL(const platform::WindowPtr window);
                            ~DeviceContextGL();

        void                checkForErrors(const std::string& location = "") override;
        bool                create() override;
        void                destroy() override;
        bool                present() override;
        void                flush();

        bool                setVSync(bool use) override;

    protected:

        TexturePtr          createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) override;
        TexturePtr          createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) override;

        void                fillGrapthicCaps() override;

        void                printExtensionList() const;

#if defined(_PLATFORM_WIN_)
        bool                createWinApiContext();
        void                destroyWinApiContext();

        HGLRC               m_hRc;
        HDC                 m_hDc;
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
#endif //_PLATFORM_MACOSX_
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_

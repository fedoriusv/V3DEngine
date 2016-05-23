#ifndef _V3D_DRIVER_CONTEXT_D3D_H_
#define _V3D_DRIVER_CONTEXT_D3D_H_

#include "DriverContext.h"
#if defined(_PLATFORM_WIN_)
#   include <windows.h>
#endif //_PLATFORM_WIN_

#ifdef _DIRECT3D_DRIVER_
#   include <d3d11.h>
#endif //_DIRECT3D_DRIVER_

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDriverContextD3D : public CDriverContext
    {
    public:

        CDriverContextD3D(const platform::WindowPtr& window);
        ~CDriverContextD3D();

        void                    driverInfo()                                     override;
        void                    checkForErrors(const std::string& location = "") override;
        bool                    createContext()                                  override;
        void                    destroyContext()                                 override;
        void                    flushBuffers()                                   override;

        bool                    setVSync(bool use)                               override;

#if defined(_DIRECT3D_DRIVER_)
        ID3D11DeviceContext*    getD3DContext()     const;
        ID3D11RenderTargetView* getD3DTargetView()  const;
#endif //_DIRECT3D_DRIVER_

    protected:

        TexturePtr              createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)  override;
        TexturePtr              createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)                  override;

        void                    printExtensionList() const;

#if defined(_DIRECT3D_DRIVER_)
        bool                    createWin32Context();
        void                    destroyWin32Context();

        s32                     m_vsyncValue;
        IDXGISwapChain*         m_swapChain;
        ID3D11Device*           m_device;
        ID3D11DeviceContext*    m_deviceContext;
        ID3D11RenderTargetView* m_renderTargetView;
#endif //_DIRECT3D_DRIVER_
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DRIVER_CONTEXT_D3D_H_

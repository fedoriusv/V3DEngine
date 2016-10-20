#pragma once

#include "DeviceContext.h"

#ifdef _DIRECT3D_RENDER_
#if defined(_PLATFORM_WIN_)
#   include <windows.h>
#endif //_PLATFORM_WIN_
#include <d3d11.h>

namespace v3d
{
namespace renderer
{
namespace d3d
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class DeviceContextD3D final : public DeviceContext
    {
    public:

        explicit                DeviceContextD3D(const platform::WindowPtr window);
                                ~DeviceContextD3D();

        void                    checkForErrors(const std::string& location = "") override;
        bool                    create() override;
        void                    destroy() override;

        bool                    setVSync(bool use) override;
        bool                    present() override;

#if defined(_DIRECT3D_RENDER_)
        ID3D11DeviceContext*    getD3DContext()     const;
        ID3D11RenderTargetView* getD3DTargetView()  const;
#endif //_DIRECT3D_RENDER_

    protected:

        TexturePtr              createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) override;
        TexturePtr              createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) override;

        void                    printExtensionList() const;

        void                    fillGrapthicCaps() override;

#if defined(_DIRECT3D_RENDER_)
        bool                    createWinApiContext();
        void                    destroyWinApiContext();

        s32                     m_vsyncValue;
        IDXGISwapChain*         m_swapChain;
        ID3D11Device*           m_device;
        ID3D11DeviceContext*    m_deviceContext;
        ID3D11RenderTargetView* m_renderTargetView;
#endif //_DIRECT3D_RENDER_
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_DIRECT3D_RENDER_

#include "DeviceContextD3D.h"
#include "utils/Logger.h"

#ifdef _DIRECT3D_RENDER_
#include <dxgi.h>
#include <d3dcommon.h>

#if defined(_PLATFORM_WIN_)
#   include <winuser.h>
#   include "platform/WindowWinApi.h"

#   pragma comment(lib, "dxgi.lib")
#   pragma comment(lib, "d3d11.lib")
#endif //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{
namespace d3d
{

DeviceContextD3D::DeviceContextD3D(const platform::WindowPtr window)
    : DeviceContext(window)
{
#if defined(_PLATFORM_WIN_)
    m_swapChain = nullptr;
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_renderTargetView = nullptr;
    m_vsyncValue = 1;
#endif //_PLATFORM_WIN_
}

DeviceContextD3D::~DeviceContextD3D()
{
}

bool DeviceContextD3D::create()
{
#if defined(_PLATFORM_WIN_)
    return createWinApiContext();
#endif //_PLATFORM_WIN_
    return false;
}

void DeviceContextD3D::destroy()
{
#if defined(_PLATFORM_WIN_)
    destroyWinApiContext();
#endif //_PLATFORM_WIN_
}

#if defined(_PLATFORM_WIN_)
bool DeviceContextD3D::createWinApiContext()
{
    LOG_INFO("Create Win32 D3D Context");

    // Create a DirectX graphics interface factory.
    IDXGIFactory* factory;
    HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
    {
        return false;
    }

    // Use the factory to create an adapter for the primary graphics interface (video card).
    IDXGIAdapter* adapter;
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        factory->Release();
        factory = nullptr;
        return false;
    }

    // Enumerate the primary adapter output (monitor).
    IDXGIOutput* adapterOutput;
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
    {
        adapter->Release();
        adapter = nullptr;
        factory->Release();
        factory = nullptr;
        return false;
    }

    u32 numModes;
    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result))
    {
        adapterOutput->Release();
        adapterOutput = nullptr;
        adapter->Release();
        adapter = nullptr;
        factory->Release();
        factory = nullptr;
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination.
    // Now fill the display mode list structures.
    DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        delete[] displayModeList;
        displayModeList = nullptr;
        adapterOutput->Release();
        adapterOutput = nullptr;
        adapter->Release();
        adapter = nullptr;
        factory->Release();
        factory = nullptr;
    }

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    u32 numerator = 0, denominator = 1;
    for (u32 i = 0; i < numModes; ++i)
    {
        if (displayModeList[i].Width == getWindow()->getSize().width)
        {
            if (displayModeList[i].Height == getWindow()->getSize().height)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // Get the adapter (video card) description.
    DXGI_ADAPTER_DESC adapterDesc;
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        delete[] displayModeList;
        displayModeList = nullptr;
        adapterOutput->Release();
        adapterOutput = nullptr;
        adapter->Release();
        adapter = nullptr;
        factory->Release();
        factory = nullptr;
        return false;
    }

    // Store the dedicated video card memory in megabytes.
    s32 videoCardMemory = (s32)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array and store it.
    size_t stringLength;
    char videoCardDescription[128];
    s32 error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        return false;
    }
    delete[] displayModeList;
    displayModeList = nullptr;
    adapterOutput->Release();
    adapterOutput = nullptr;
    adapter->Release();
    adapter = nullptr;
    factory->Release();
    factory = nullptr;

    // Get HWND
    HWND window = std::static_pointer_cast<const platform::WindowWinApi>(getWindow())->getHandleWindow();

    // Initialize the swap chain description.
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;                                  // Set to a single back buffer.
    swapChainDesc.BufferDesc.Width = getWindow()->getSize().width;     // Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Height = getWindow()->getSize().height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // Set the usage of the back buffer.
    swapChainDesc.OutputWindow = window;                            // Set the handle for the window to render to.
    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;     // Set the refresh rate of the back buffer.
    swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    swapChainDesc.SampleDesc.Count = 1;                             // Turn multisampling off.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !getWindow()->isFullscreen();           // Set to full screen or windowed mode.
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // Set the scan line ordering and scaling to unspecified.
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;             // Discard the back buffer contents after presenting.
    swapChainDesc.Flags = 0;                                         // Don't set the advanced flags.

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (DIRECTD3D_VERSION_MAJOR == 9 && DIRECTD3D_VERSION_MINOR == 1)
    {
        featureLevel = D3D_FEATURE_LEVEL_9_1;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 9 && DIRECTD3D_VERSION_MINOR == 2)
    {
        featureLevel = D3D_FEATURE_LEVEL_9_2;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 9 && DIRECTD3D_VERSION_MINOR == 3)
    {
        featureLevel = D3D_FEATURE_LEVEL_9_3;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 10 && DIRECTD3D_VERSION_MINOR == 0)
    {
        featureLevel = D3D_FEATURE_LEVEL_10_0;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 10 && DIRECTD3D_VERSION_MINOR == 1)
    {
        featureLevel = D3D_FEATURE_LEVEL_10_1;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 11 && DIRECTD3D_VERSION_MINOR == 0)
    {
        featureLevel = D3D_FEATURE_LEVEL_11_0;
    }
    else if (DIRECTD3D_VERSION_MAJOR == 11 && DIRECTD3D_VERSION_MINOR == 1)
    {
        featureLevel = D3D_FEATURE_LEVEL_11_1;
    }
    else
    {
        LOG_WARNING("CDriverContextD3D::createWin32Context: D3D level not found set D3D_FEATURE_LEVEL_11_0");
    }

    // Create the swap chain, Direct3D device, and Direct3D device context.
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
    if (FAILED(result))
    {
        return false;
    }

    // Get the pointer to the back buffer.
    ID3D11Texture2D* backBufferPtr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    // Create the render target view with the back buffer pointer.
    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(result))
    {
        return false;
    }
    backBufferPtr->Release();
    backBufferPtr = nullptr;

    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);


    DeviceContextD3D::checkForErrors("Create Context");

    LOG_INFO("Win32 Context D3D successful created");

    return true;
}

void DeviceContextD3D::destroyWinApiContext()
{
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, NULL);
    }

    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_deviceContext)
    {
        m_deviceContext->Release();
        m_deviceContext = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }
}

ID3D11DeviceContext* DeviceContextD3D::getD3DContext() const
{
    return m_deviceContext;
}

ID3D11RenderTargetView* DeviceContextD3D::getD3DTargetView() const
{
    return m_renderTargetView;
}
#endif //_PLATFORM_WIN_

//void DeviceContextD3D::driverInfo()
//{
//    // print renderer information
//    
//    LOG_INFO("Direct3D config info:");
//    /*LOG("Render: %s", renderer);
//    LOG("Vendor: %s", vendor);
//    LOG("GLSL: %s", GLSL);
//    LOG("GL Version: %s", version);
//    LOG("Max Texure Units: %d", maxTextureUnits);
//    LOG("Max Texure Layers: %d", maxTextureLayers);
//    LOG("Max Anisotropy: %f", m_maxAnisotropy);
//    LOG("Max Draw Buffers: %d", maxDrawBuffers);
//    LOG("MSAA x%d", m_samplersCount);*/
//
//    /*glewIsSupported("GL_ARB_multitexture");
//    glewIsSupported("GL_ARB_vertex_buffer_object");
//    glewIsSupported("GL_ARB_vertex_array_object");
//    glewIsSupported("GL_ARB_sampler_objects");*/
//
//#ifdef _DEBUG
//    DeviceContextD3D::printExtensionList();
//#endif
//}

void DeviceContextD3D::checkForErrors(const std::string& location)
{
#ifdef _DEBUG
    //TODO:
#endif
}

bool DeviceContextD3D::setVSync(bool use)
{
    m_vsyncValue = use ? 1 : 0;
    return true;
}

bool DeviceContextD3D::present()
{
    return false;
}

TexturePtr DeviceContextD3D::createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D & size, const void * data, u32 level)
{
    return TexturePtr();
}

TexturePtr DeviceContextD3D::createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D & size, const void * data[6], u32 level)
{
    return TexturePtr();
}

void DeviceContextD3D::printExtensionList() const
{
    LOG_INFO("DirectD3D Extentions List:");
   //TODO:
}

void DeviceContextD3D::fillGrapthicCaps()
{
}

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_DIRECT3D_DRIVER_

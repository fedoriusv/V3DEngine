#include "DriverContextGL.h"
#include "renderer/GL/TextureGL.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"
#pragma comment(lib, "OpenGL32.lib")
#ifdef _DEBUG
#   pragma comment(lib, "glew_static_d.lib")
#else //_DEBUG
#   pragma comment(lib, "glew_static.lib")
#endif //_DEBUG

#if defined(_PLATFORM_WIN_)
#   include <winuser.h>
#   include "GL/wglew.h"
#   include "platform/WindowWin32.h"
#elif defined(_PLATFORM_LINUX_)
#   include "GL/glxew.h"
#   include <GL/glx.h>
#   include "platform/WindowLinux.h"
#elif defined(_PLATFORM_MACOSX_)
#   import <Cocoa/Cocoa.h>
#   include "platform/WindowMacOSX.h"
#endif //_PLATFORM_MACOSX_

namespace v3d
{
namespace renderer
{

CDriverContextGL::CDriverContextGL(const platform::WindowPtr& window)
    : CDriverContext(window)
{
#if defined(_PLATFORM_WIN_)
    m_hRc = NULL;
#elif defined(_PLATFORM_MACOSX_)
    m_context._thread = 0;
#endif
}

CDriverContextGL::~CDriverContextGL()
{
}

bool CDriverContextGL::createContext()
{
#if defined(_PLATFORM_WIN_)
    return createWin32Context();

#elif defined(_PLATFORM_LINUX_)
    return createLinuxContext();

#elif defined(_PLATFORM_MACOSX_)
    return createMacOSXContext();
#endif //_PLATFORM_MACOSX_
    return false;
}

void CDriverContextGL::destroyContext()
{
#if defined(_PLATFORM_WIN_)
    destroyWin32Context();
#elif defined(_PLATFORM_LINUX_)
    return createLinuxContext();
#elif defined(_PLATFORM_MACOSX_)
    destroyMacOSXContext();
#endif //_PLATFORM_MACOSX_
}

#if defined(_PLATFORM_WIN_)
bool CDriverContextGL::createWin32Context()
{
    LOG_INFO("Create Win32 GL Context");

    // Create a window to test antialiasing support
    HINSTANCE hInstance = GetModuleHandle(NULL);
    LPCWSTR className = __TEXT("V3DWin32GL");

    // Register Class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)DefWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = className;
    wcex.hIconSm = 0;
    wcex.hIcon = 0;
    RegisterClassEx(&wcex);

    RECT clientSize;
    clientSize.top = 0;
    clientSize.left = 0;
    clientSize.right = getWindow()->getSize().width;
    clientSize.bottom = getWindow()->getSize().height;

    DWORD style = WS_POPUP;
    if (!getWindow()->isFullscreen())
    {
        style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }

    AdjustWindowRect(&clientSize, style, FALSE);

    const s32 realWidth = clientSize.right - clientSize.left;
    const s32 realHeight = clientSize.bottom - clientSize.top;

    const s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
    const s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

    HWND tempWindow = CreateWindow(className, __TEXT(""), style, windowLeft,
        windowTop, realWidth, realHeight, NULL, NULL, hInstance, NULL);

    if (!tempWindow)
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Cannot create a temp window");
        UnregisterClass(className, hInstance);

        return false;
    }

    HDC tempDC = GetDC(tempWindow);

    // Set up pixel format descriptor with desired parameters
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),             // Size Of This Pixel Format Descriptor
        1,                                         // Version Number
        PFD_DRAW_TO_WINDOW |                       // Format Must Support Window
        PFD_SUPPORT_OPENGL |                       // Format Must Support OpenGL
        PFD_DOUBLEBUFFER |                         // Must Support Double Buffering
        PFD_TYPE_RGBA,                             // Request An RGBA Format
        32,                                        // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                          // Color Bits Ignored
        0,                                         // No Alpha Buffer
        0,                                         // Shift Bit Ignored
        0,                                         // No Accumulation Buffer
        0, 0, 0, 0,	                               // Accumulation Bits Ignored
        24,                                        // Z-Buffer (Depth Buffer)
        0,                                         // Stencil Buffer Depth
        0,                                         // No Auxiliary Buffer
        PFD_MAIN_PLANE,                            // Main Drawing Layer
        0,                                         // Reserved
        0, 0, 0                                    // Layer Masks Ignored
    };

    GLuint pixelFormat;
    pixelFormat = ChoosePixelFormat(tempDC, &pfd);
    SetPixelFormat(tempDC, pixelFormat, &pfd);

    HGLRC hRc = wglCreateContext(tempDC);
    if (!hRc)
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Cannot create a temporary GL rendering context");
        ReleaseDC(tempWindow, tempDC);
        DestroyWindow(tempWindow);
        UnregisterClass(className, hInstance);

        return false;
    }

    if (!wglMakeCurrent(tempDC, hRc))
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Cannot activate a temporary GL rendering context");
        wglDeleteContext(hRc);
        ReleaseDC(tempWindow, tempDC);
        DestroyWindow(tempWindow);
        UnregisterClass(className, hInstance);

        return false;
    }

    const GLubyte* version = glewGetString(GLEW_VERSION);
    LOG_INFO("GLEW Init. Version %s", version);
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(error);
        LOG_ERROR("DriverContextGL::createWin32Context: Couldn't initialize GLEW: %s", errorStr);
        wglDeleteContext(hRc);
        ReleaseDC(tempWindow, tempDC);
        DestroyWindow(tempWindow);
        UnregisterClass(className, hInstance);

        return false;
    }

    if (!wglewIsSupported("WGL_ARB_create_context") ||
        !wglewIsSupported("WGL_ARB_pixel_format") ||
        !wglewIsSupported("WGL_ARB_multisample"))
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Error Supported GLEW Lib");
        wglDeleteContext(hRc);
        ReleaseDC(tempWindow, tempDC);
        DestroyWindow(tempWindow);
        UnregisterClass(className, hInstance);

        return false;
    }

    s32 antiAlias = 32;

    f32 fAttributes[] = { 0.0f, 0.0f };
    s32 iAttributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_STEREO_ARB, 0,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_SAMPLES_ARB, antiAlias,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        //WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
        0, 0,
        0, 0
    };

    GLuint numFormats = 0;
    GLint  newPixelFormat = 0;

    while (numFormats == 0)
    {
        if (!wglChoosePixelFormatARB(tempDC, iAttributes, fAttributes, 1, &newPixelFormat, &numFormats))
        {
            LOG_ERROR("DriverContextGL::createWin32Context: Can't Find A Suitable ExPixelFormat");
            wglDeleteContext(hRc);
            ReleaseDC(tempWindow, tempDC);
            DestroyWindow(tempWindow);
            UnregisterClass(className, hInstance);

            return false;
        }

        --iAttributes[21]; //WGL_SAMPLES_ARB, antiAlias
        if (iAttributes[21] < 0)
        {
            LOG_ERROR("DriverContextGL::createWin32Context: Can't Choose ExPixelFormat");
            iAttributes[21] = 0;
            break;
        }
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRc);
    ReleaseDC(tempWindow, tempDC);
    DestroyWindow(tempWindow);
    UnregisterClass(className, hInstance);

    // Get HWND
    HWND window = std::static_pointer_cast<const platform::CWindowWin32>(getWindow())->getHandleWindow();

    HDC hDC = GetDC(window);
    if (!hDC)
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Cannot create a GL device context");
        return false;
    }

    if (newPixelFormat == 0 || !SetPixelFormat(hDC, newPixelFormat, &pfd))
    {
        LOG_WARNING("DriverContextGL::createWin32Context: Cannot create ExPixelFormat. Set default pixel format");
        newPixelFormat = ChoosePixelFormat(hDC, &pfd);
        m_hRc = wglCreateContext(hDC);
    }
    else
    {
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, OPENGL_VERSION_MAJOR,
            WGL_CONTEXT_MINOR_VERSION_ARB, OPENGL_VERSION_MINOR,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        m_hRc = wglCreateContextAttribsARB(hDC, 0, attribs);
    }


    if (!m_hRc || !wglMakeCurrent(hDC, m_hRc))
    {
        LOG_ERROR("DriverContextGL::createWin32Context: Can't Create GL Rendering Context");
        return false;
    }

    //TODO Need Reinit glew http://stackoverflow.com/questions/9663262/resetting-opengl-extension-pointers-in-windows-api
    /*error = wglewContextInit();
    if (error != GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(error);
        LOG_ERROR("Couldn't initialize GLEW: %s", errorStr);

        return false;
    }*/

    CDriverContextGL::checkForErrors("Create Context");

    int pf = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    LOG_INFO("Win32 Context GL successful created");

    return true;
}

void CDriverContextGL::destroyWin32Context()
{
    wglMakeCurrent(NULL, NULL);
    if (m_hRc)
    {
        wglDeleteContext(m_hRc);
        m_hRc = NULL;
    }
}

#elif defined(_PLATFORM_LINUX_)

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

bool CDriverContextGL::createLinuxContext()
{
    LOG_INFO("CDriverContextGL::createLinuxContext: Create Linux GL Context");

    // Get the default screen's GLX extension list
    Display* display = std::static_pointer_cast<const platform::CWindowLinux>(m_window)->getDisplay();
    const char *glxExtensions = glXQueryExtensionsString(display, DefaultScreen(display));

    if (!gluCheckExtension((const GLubyte*)"GLX_ARB_create_context" , (const GLubyte*)glxExtensions))
    {
        LOG_ERROR("glXCreateContextAttribsARB() not found");
        return false;
    }

    int attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, OPENGL_VERSION_MAJOR,
        GLX_CONTEXT_MINOR_VERSION_ARB, OPENGL_VERSION_MINOR,
        //GLX_CONTEXT_FLAGS_ARB,       GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None
    };

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    GLXContext context = nullptr;
    GLXFBConfig fbConfigs = std::static_pointer_cast<const platform::CWindowLinux>(m_window)->getFBConfig();
    //XVisualInfo* visualInfo = static_cast< const platform::CPlatformLinux*>(m_platform)->getVisualInfo();
    //context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
    context = glXCreateContextAttribsARB(display, fbConfigs, 0, True, attribs);
    if (!context)
    {
        LOG_ERROR("CDriverContextGL::createLinuxContext: Error to create exContext OpenGL");
        return false;
    }
    
    XSync(display, False);

    Window window = std::static_pointer_cast<const platform::CWindowLinux>(m_window)->getWidow();
    if (!glXMakeCurrent(display, window, context))
    {
        LOG_ERROR("CDriverContextGL::createLinuxContext: Can not set current context");
        return false;
    }

    glewExperimental = GL_TRUE;
    
    GLenum error = glewInit();
    if (error !=  GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(error);
        LOG_ERROR("CDriverContextGL::createLinuxContext: Couldn't initialize GLEW: %s", errorStr);

        glXMakeCurrent(display, 0, 0);
        glXDestroyContext(display, context);

        return false;
    }

    CDriverContextGL::checkForErrors("Create Context");

    LOG_INFO("CDriverContextGL::createLinuxContext: Linux Context GL successful created");

    return true;
}

#elif defined(_PLATFORM_MACOSX_)
bool CDriverContextGL::createMacOSXContext()
{
    LOG_INFO("DriverContextGL::createMacOSXContext: Create MacOSX GL Context");
    
    u32 attributeCount = 0;
    
    void* currentContext = pthread_getspecific(m_context._thread);
    
    if (pthread_key_create(&m_context._thread, NULL) != 0)
    {
        LOG_ERROR("DriverContextGL::createMacOSXContext: Failed to create context");
        return false;
    }
    
#define ADD_ATTR(x) { attributes[attributeCount++] = x; }
#define ADD_ATTR2(x, y) { ADD_ATTR(x); ADD_ATTR(y); }
    
    // Arbitrary array size here
    NSOpenGLPixelFormatAttribute attributes[40];
    
    ADD_ATTR(NSOpenGLPFAAccelerated);
    ADD_ATTR(NSOpenGLPFAClosestPolicy);
    
    if (OPENGL_VERSION_MAJOR >= 4)
    {
        ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core);
    }
    else if (OPENGL_VERSION_MAJOR >= 3)
    {
        ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
    }
    else if (OPENGL_VERSION_MAJOR <= 2)
    {
        ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy);
        ADD_ATTR2(NSOpenGLPFAAuxBuffers, 0);
        ADD_ATTR2(NSOpenGLPFAAccumSize, 0);
    }
    else
    {
        LOG_ERROR("DriverContextGL::createMacOSXContext: OpenGL version %d.%d not supported", OPENGL_VERSION_MAJOR, OPENGL_VERSION_MINOR);
        return false;
    }

    u32 samplers = 4;
    
    ADD_ATTR2(NSOpenGLPFAColorSize, 24);
    ADD_ATTR2(NSOpenGLPFAAlphaSize, 8);
    ADD_ATTR2(NSOpenGLPFADepthSize, 24);
    ADD_ATTR2(NSOpenGLPFAStencilSize, 8);
    //ADD_ATTR(NSOpenGLPFAStereo);
    ADD_ATTR(NSOpenGLPFADoubleBuffer);
    ADD_ATTR2(NSOpenGLPFASampleBuffers, 1);
    ADD_ATTR2(NSOpenGLPFASamples, samplers);
    
    ADD_ATTR(0);
    
#undef ADD_ATTR
#undef ADD_ATTR2
    
    m_context._pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    if (m_context._pixelFormat == nil)
    {
        LOG_ERROR("DriverContextGL::createMacOSXContext: Failed to create OpenGL pixel format");
        
        return false;
    }
    
    NSOpenGLContext* share = NULL;
    m_context._context = [[NSOpenGLContext alloc] initWithFormat:m_context._pixelFormat
                               shareContext:share];
    if (m_context._context == nil)
    {
        LOG_ERROR("DriverContextGL::createMacOSXContext: Failed to create OpenGL context");
        
        return false;
    }
    
    const platform::WindowNS& window = std::static_pointer_cast<const platform::CWindowMacOSX>(m_window)->getHandleWindow();
    [m_context._context setView:window._view];
    
    if (window._window)
    {
        [m_context._context makeCurrentContext];
    }
    else
    {
        [NSOpenGLContext clearCurrentContext];
    }
    pthread_setspecific(m_context._thread, window._window);
    [m_context._context flushBuffer];
    
    const GLubyte* version = glewGetString(GLEW_VERSION);
    LOG_INFO("DriverContextGL::createMacOSXContext: GLEW Init. Version %s", version);
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(error);
        LOG_ERROR("DriverContextGL::createMacOSXContext: Couldn't initialize GLEW: %s", errorStr);
        
        return false;
    }
    CDriverContextGL::checkForErrors("Create Context");
    
    pthread_setspecific(m_context._thread, currentContext);
    LOG_INFO("DriverContextGL::createMacOSXContext: MacOSX Context GL successful created");
    
    return true;
}

void CDriverContextGL::destroyMacOSXContext()
{
    if (m_context._pixelFormat)
    {
        [m_context._pixelFormat release];
        m_context._pixelFormat = nil;
    }
    
    if (m_context._context)
    {
        [m_context._context release];
        m_context._context = nil;
    }

    if (m_context._thread)
    {
        pthread_key_delete(m_context._thread);
        m_context._thread = 0;
    }
}
#endif //_PLATFORM_MACOSX_

TexturePtr CDriverContextGL::createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
{
    return new gl::CTextureGL(target, format, type, size, data, level);
}

TexturePtr CDriverContextGL::createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)
{
    return new gl::CTextureGL(format, type, size, data, level);
}

void CDriverContextGL::driverInfo()
{
    // print renderer information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* GLSL = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte* version = glGetString(GL_VERSION);

    GLint glslVersionsNum;
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &glslVersionsNum);
    std::vector<const GLubyte*> glslVersionList;
    for (s32 i = 0; i < glslVersionsNum; ++i)
    {
        const GLubyte* versionStr = glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
        glslVersionList.push_back(versionStr);
    }
    GLubyte* ver = const_cast<GLubyte*>(glslVersionList.front());
    m_shaderVersion = reinterpret_cast<c8*>(ver);

    glGetIntegerv(GL_MAJOR_VERSION, &m_renderMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &m_renderMinorVersion);

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    CDriverContext::setTextureUnitsCount((u32)maxTextureUnits);

    GLint maxTextureSamplers;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureSamplers);

    GLint maxDrawBuffers;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    CDriverContext::setMaxAnisotropySize(maxAnisotropy);

    GLint samplesCount;
    glGetIntegerv(GL_SAMPLES, &samplesCount);
    CDriverContext::setSamplesCount(samplesCount);

    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    CDriverContext::setMaxPatchVertices(maxPatchVertices);

    LOG_INFO("OpenGL config info:");
    LOG("Render: %s", renderer);
    LOG("Vendor: %s", vendor);
    LOG("GLSL: %s", GLSL);
    LOG("GL Version: %s", version);
    LOG("Max Texure Units: %d", maxTextureUnits);
    LOG("Max Texure Samplers: %d", maxTextureSamplers);
    LOG("Max Anisotropy: %f", maxAnisotropy);
    LOG("Max Draw Buffers: %d", maxDrawBuffers);
    LOG("MSAA x%d", samplesCount);
    LOG("Max supported patch vertices: %d", maxPatchVertices);

#ifdef _DEBUG_GL
    CDriverContextGL::printExtensionList();
#endif //_DEBUG_GL
}

void CDriverContextGL::checkForErrors(const std::string& location)
{
#ifdef _DEBUG_GL
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        std::string error;
        switch (glError)
        {
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION";              break;
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM";                   break;
            case GL_INVALID_VALUE:                  error = "INVALID_VALUE";                  break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY";                  break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
            default:                                error = "UNKNOWN";                        break;
        }

        if (location.empty())
        {
            LOG_ERROR("GL Error: %s. Discription %s", error.c_str(), glewGetErrorString(glError));
#if _DEBUG_GL_ERROR_ASSERT
            ASSERT(false, error.c_str());
#endif //_DEBUG_GL_ERROR_ASSERT
        }
        else
        {
            LOG_ERROR("GL %s: %s. Discription %s", location.c_str(), error.c_str(), glewGetErrorString(glError));
#if _DEBUG_GL_ERROR_ASSERT
            ASSERT(false, error.c_str());
#endif //_DEBUG_GL_ERROR_ASSERT
        }
    }
#endif //_DEBUG_GL
}

bool CDriverContextGL::setVSync(bool use)
{
    bool succeed = false;
#if defined(_PLATFORM_WIN_)
    if (WGLEW_EXT_swap_control)
    {
        succeed = (wglSwapIntervalEXT(use ? 1 : 0) == TRUE) ? true : false;
    }
#elif defined(_PLATFORM_MACOSX_)
    GLint sync = use ? 1 : 0;
    [m_context._context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
#endif //_PLATFORM_MACOSX_
    return succeed;
}

void CDriverContextGL::printExtensionList() const
{
    LOG_INFO("OpenGL Extentions List:");

    GLint num;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num);

    for (s32 i = 0; i < num; ++i) 
    {
        const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);
        LOG("%s", ext);
    }
}

void CDriverContextGL::flushBuffers()
{
    glFlush();
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

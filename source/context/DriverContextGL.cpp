#include "DriverContextGL.h"
#include "utils/Logger.h"

#include "GL/glew.h"

#if defined(_PLATFORM_WIN_)
#   include <windows.h>
#   include <winuser.h>
#   include "GL/wglew.h"
#   include "platform/WindowWin32.h"
#elif defined(_PLATFORM_LINUX_)
#   include "GL/glxew.h"
#   include <GL/glx.h>
#   include "platform/WindowLinux.h"
#endif

using namespace v3d;
using namespace v3d::renderer;

CDriverContextGL::CDriverContextGL(const platform::WindowPtr& window)
    : CDriverContext(window)
{
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

#endif
    return false;
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
    clientSize.right = m_window->getSize().width;
    clientSize.bottom = m_window->getSize().height;

    DWORD style = WS_POPUP;
    if (!m_window->isFullscreen())
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
        LOG_ERROR("Cannot create a temp window");
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
        PFD_DOUBLEBUFFER |                       // Must Support Double Buffering
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
        LOG_ERROR("Cannot create a temporary GL rendering context");
        ReleaseDC(tempWindow, tempDC);
        DestroyWindow(tempWindow);
        UnregisterClass(className, hInstance);

        return false;
    }

    if (!wglMakeCurrent(tempDC, hRc))
    {
        LOG_ERROR("Cannot activate a temporary GL rendering context");
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
        LOG_ERROR("Couldn't initialize GLEW: %s", errorStr);

        return false;
    }

    if (!wglewIsSupported("WGL_ARB_create_context") ||
        !wglewIsSupported("WGL_ARB_pixel_format") ||
        !wglewIsSupported("WGL_ARB_multisample"))
    {
        LOG_ERROR("Error Supported GLEW Lib");
        return false;
    }

    int antiAlias = 32;

    f32 fAttributes[] = { 0.0f, 0.0f };
    s32 iAttributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 0,
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
            LOG_ERROR("Can't Find A Suitable ExPixelFormat");
            return false;
        }

        --iAttributes[21]; //WGL_SAMPLES_ARB, antiAlias
        if (iAttributes[21] < 0)
        {
            LOG_ERROR("Can't Choose ExPixelFormat");
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
    HWND window = std::static_pointer_cast<const platform::CWindowWin32>(m_window)->getHandleWindow();

    HDC hDC = GetDC(window);
    if (!hDC)
    {
        LOG_ERROR("Cannot create a GL device context");
        return false;
    }

    if (newPixelFormat == 0 || !SetPixelFormat(hDC, newPixelFormat, &pfd))
    {
        LOG_WARNING("Cannot create ExPixelFormat. Set default pixel format");
        newPixelFormat = ChoosePixelFormat(hDC, &pfd);
        hRc = wglCreateContext(hDC);
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

        hRc = wglCreateContextAttribsARB(hDC, 0, attribs);
    }


    if (!hRc || !wglMakeCurrent(hDC, hRc))
    {
        LOG_ERROR("Can't Create GL Rendering Context");
        return false;
    }

    CDriverContextGL::checkForErrors("Create Context");

    int pf = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    LOG_INFO("Win32 Context GL successful created");

    return true;
}

#elif defined(_PLATFORM_LINUX_)

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

bool CDriverContextGL::createLinuxContext()
{
    LOG_INFO("Create Linux GL Context");

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
        LOG_ERROR("Error to create exContext OpenGL");
        return false;
    }
    
    XSync(display, False);

    Window window = std::static_pointer_cast<const platform::CWindowLinux>(m_window)->getWidow();
    if (!glXMakeCurrent(display, window, context))
    {
        LOG_ERROR("Can not set current context");
        return false;
    }

    glewExperimental = GL_TRUE;
    
    GLenum error = glewInit();
    if (error !=  GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(error);
        LOG_ERROR("Couldn't initialize GLEW: %s", errorStr);

        glXMakeCurrent(display, 0, 0);
        glXDestroyContext(display, context);

        return false;
    }

    CDriverContextGL::checkForErrors("Create Context");

    LOG_INFO("Linux Context GL successful created");

    return true;
}

#elif defined(_PLATFORM_MACOSX_)

bool CDriverContextGL::createMacOSXContext()
{
    return false;
}

#endif

void CDriverContextGL::driverInfo()
{
    // print renderer information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* GLSL = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte* version = glGetString(GL_VERSION);

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);

    LOG_INFO("Render: %s", renderer);
    LOG_INFO("Vendor: %s", vendor);
    LOG_INFO("GLSL: %s", GLSL);
    LOG_INFO("GL Version: %s", version);
    LOG_INFO("Max Texture Units: %d", maxTextureUnits);

    /*glewIsSupported("GL_ARB_multitexture");
    glewIsSupported("GL_ARB_vertex_buffer_object");
    glewIsSupported("GL_ARB_vertex_array_object");
    glewIsSupported("GL_ARB_sampler_objects");*/

#ifdef _DEBUG
    CDriverContextGL::printExtensionList();
#endif
}

void CDriverContextGL::checkForErrors(const std::string& location)
{
#ifdef _DEBUG
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        if (location.empty())
        {
            LOG_ERROR("GL Error: %s", glewGetErrorString(glError))
        }
        else
        {
            LOG_ERROR("GL %s: %s", location.c_str(), glewGetErrorString(glError))
        }
    }
#endif
}

bool CDriverContextGL::setVSync(bool use)
{
    bool succeed = false;
#if defined(_PLATFORM_WIN_)
    if (WGLEW_EXT_swap_control)
    {
        succeed = (wglSwapIntervalEXT(use ? 1 : 0) == TRUE) ? true : false;
    }
#endif
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

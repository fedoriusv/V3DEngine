#include "WindowLinux.h"

#include "Engine.h"
#include "utils/Logger.h"

#include <X11/Xatom.h>
#include <iostream>

using namespace v3d;
using namespace v3d::platform;

CWindowLinux::CWindowLinux(const WindowParam& param)
    : CWindow(param)
{
    m_platformType = EPlatformType::ePlatformLinux;
}

CWindowLinux::~CWindowLinux()
{
}

Window CWindowLinux::getWidow() const
{
	return m_window;
}
GLXFBConfig CWindowLinux::getFBConfig() const
{
	return m_glxFBConfigs;
}

Display* CWindowLinux::getDisplay() const
{
	return m_display;
}

XVisualInfo* CWindowLinux::getVisualInfo() const
{
	return m_visualInfo;
}

void CWindowLinux::minimize()
{
	XIconifyWindow(m_display, m_window, m_screen);
}

void CWindowLinux::maximize()
{
	XMapWindow(m_display, m_window);
}

void CWindowLinux::restore()
{
	XMapWindow(m_display, m_window);
}

void CWindowLinux::setFullScreen( bool value )
{
}

void CWindowLinux::setResizeble ( bool value )
{
}

void CWindowLinux::setCaption( const std::string& text )
{
	XTextProperty txt;

	char* temp = const_cast<char*>(text.c_str());
	if (Success == XmbTextListToTextProperty(m_display, &temp, 1, XStdICCTextStyle, &txt))
	{
		XSetWMName(m_display, m_window, &txt);
		XSetWMIconName(m_display, m_window, &txt);
		XFree(txt.value);
	}
}

void CWindowLinux::setPosition(const Dimension2D &pos)
{
    if (m_param.isFullscreen)
    {
        return;
    }

    //TODO:

    m_param.position = pos;
}

bool CWindowLinux::isMaximized() const
{
	return false;
}

bool CWindowLinux::isMinimized() const
{
	return false;
}

bool CWindowLinux::isActive() const
{
	return false;
}

bool CWindowLinux::isFocused() const
{
	return false;
}

void CWindowLinux::create()
{
    LOG_INFO("Create Windows")

	m_display = XOpenDisplay(NULL);
	if (!m_display)
	{
        LOG_ERROR("Failed to open X display");
		return;
	}

	int major, minor;
	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion(m_display, &major, &minor) ||
			(( major == 1) && (minor < 3)) || (major < 1) )
	{
        LOG_ERROR("Invalid GLX version");
		return;
	}

	// Get a matching FB config
	 static int fbAttribs[] =
	{
		GLX_X_RENDERABLE,       True,
		GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,        GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,      GLX_TRUE_COLOR,
		GLX_RED_SIZE,           8,
		GLX_GREEN_SIZE,         8,
		GLX_BLUE_SIZE,          8,
		GLX_ALPHA_SIZE,         8,
		GLX_DEPTH_SIZE,         24,
		GLX_STENCIL_SIZE,       8,
		GLX_DOUBLEBUFFER,       True,
		//GLX_SAMPLE_BUFFERS,   True,
		//GLX_SAMPLES,          4,
		//GLX_SAMPLE_BUFFERS_ARB, True,
		//GLX_SAMPLES_ARB,        antiAlias,
		None
	};

	m_screen = DefaultScreen(m_display);
	int numConfigs = 0;

	GLXFBConfig *fbConfigs;
	fbConfigs = glXChooseFBConfig(m_display, m_screen, fbAttribs, &numConfigs);
    if (!fbConfigs)
	{
        LOG_ERROR("Failed to retrieve a framebuffer config");
		return;
	}

	// Pick the FB config/visual with the most samples per pixel
	int bestFbc = -1, worstFbc = -1, bestNumSamp = -1, worstNumSamp = 999;
	for ( int i = 0; i < numConfigs; ++i )
	{
		XVisualInfo *visualInfo;
        visualInfo = glXGetVisualFromFBConfig(m_display, fbConfigs[i]);
		if ( visualInfo )
		{
			int sampBuf, samples;
            glXGetFBConfigAttrib(m_display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampBuf);
            glXGetFBConfigAttrib(m_display, fbConfigs[i], GLX_SAMPLES       , &samples);

			if ( bestFbc < 0 || sampBuf && samples > bestNumSamp )
			{
				bestFbc = i;
				bestNumSamp = samples;
			}

			if ( worstFbc < 0 || !sampBuf || samples < worstNumSamp )
			{
				worstFbc = i;
				worstNumSamp = samples;
			}
		}

        XFree(visualInfo);
	}

    m_glxFBConfigs = fbConfigs[bestFbc];

	// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree( fbConfigs );

	// Get a visual
	XVisualInfo *visualInfo;
	visualInfo = glXGetVisualFromFBConfig( m_display, m_glxFBConfigs );

	m_visualInfo = visualInfo;

	XSetWindowAttributes winAttribs;

	Colormap colorMap;
	colorMap = XCreateColormap( m_display, RootWindow(m_display, visualInfo->screen), visualInfo->visual, AllocNone );

	winAttribs.colormap          = colorMap;
	winAttribs.background_pixmap = None ;
	winAttribs.border_pixel      = 0;
    winAttribs.event_mask        = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
	//winAttribs.bit_gravity       = StaticGravity;
	//winAttribs.background_pixel  = WhitePixel(m_display, m_screen);
	//winAttribs.override_redirect = m_param.isFullscreen;

    m_window = XCreateWindow(m_display, RootWindow( m_display, visualInfo->screen ),
                            0, 0, m_param.size.width, m_param.size.height, 0, visualInfo->depth, InputOutput,
                            visualInfo->visual, CWBorderPixel|CWColormap|CWEventMask, &winAttribs);
	if ( !m_window )
	{
        LOG_ERROR("Failed to create window");
		return;
	}

    XStoreName(m_display, m_window, "V3DLinux");

    LOG_INFO("Window Size (%d, %d)", m_param.size.width, m_param.size.height);

    /*GLXContext glc = glXCreateContext(m_display, visualInfo, NULL, GL_TRUE);
    glXMakeCurrent(m_display, m_window, glc);

    // Create OpenGL rendering context
    m_glxWindow = glXCreateWindow( m_display, m_glxFBConfigs, m_window, NULL );
    if (m_glxWindow)
    {
        // create glx context
        //m_context = glXCreateContext(m_display, visualInfo, None, True);
        m_glxContext = glXCreateNewContext( m_display, m_glxFBConfigs, GLX_RGBA_TYPE, NULL, True );
        if (m_glxContext)
        {
            //if (!glXMakeCurrent(m_display, m_window, m_context))
            if (!glXMakeContextCurrent( m_display, m_glxWindow, m_glxWindow, m_glxContext ))
            {
                //"Could not make context current."
                glXDestroyContext(m_display, m_glxContext);
            }
        }
        else
        {
            //"Could not create GLX rendering context."
        }
    }
    else
    {
        //"Could not create GLX window."
    }*/

	XMapWindow(m_display, m_window);

    // Get the default screen's GLX extension list
    /*const char *glxExts = glXQueryExtensionsString( m_display, DefaultScreen( m_display ) );

    // NOTE: It is not necessary to create or make current to a context before calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

    XFree( visualInfo );*/
	XFlush(m_display);
}

void CWindowLinux::close()
{
    glXMakeCurrent(m_display, 0, 0);
    glXDestroyContext(m_display, m_glxContext);

    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
}

bool CWindowLinux::begin()
{
    while (XPending(m_display) > 0)
    {
        XEvent xevent;
        XNextEvent(m_display, &xevent);

        switch (xevent.type)
        {

        case ConfigureNotify:
        {
            int a = 0;
            break;
        }

        case MapNotify:
            //WindowMinimized=false;
            break;

        case UnmapNotify:
            //WindowMinimized=true;
            break;

        case FocusIn:
            //WindowHasFocus=true;
            break;

        case FocusOut:
            //WindowHasFocus=false;
            break;

        case KeyPress:
        {
            KeySym mp;
            c8 buf[8]={0};
            XLookupString(&xevent.xkey, buf, sizeof(buf), &mp, NULL);

            v3d::event::SKeyboardInputEventPtr event = std::make_shared<v3d::event::SKeyboardInputEvent>();
            event->_event = v3d::event::eKeyboardPressDown;
            event->_key = (EKeyCode)mp;
            event->_character = (c8)mp;

            INPUT_EVENTS->pushEvent(event);

            return true;
        }

        case KeyRelease:
        {
            int a4 = 0;
            break;
        }

            default:
                break;
        }
    }

    return true;
}

bool CWindowLinux::end()
{
    glXSwapBuffers (m_display, m_window);

    return true;
}

void CWindowLinux::addKeyCodes()
{
    m_keys.add(eKeyUknown, 0x00);
}

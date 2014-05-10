#include "PlatformLinux.h"
#include "context/DriverContextGL.h"

#include <X11/Xatom.h>
#include <iostream>

using namespace v3d;
using namespace v3d::platform;

CPlatformLinux::CPlatformLinux( const PlatformParam& param )
	: CPlatform(param)
{
	m_platformType = EPlatformType::ePlatformLinux;

	CPlatformLinux::createWindows();
}

CPlatformLinux::~CPlatformLinux()
{
}

Window CPlatformLinux::getWidow() const
{
	return m_window;
}
GLXFBConfig CPlatformLinux::getFBConfig() const
{
	return m_glxFBConfigs;
}

Display* CPlatformLinux::getDisplay() const
{
	return m_display;
}

XVisualInfo* CPlatformLinux::getVisualInfo() const
{
	return m_visualInfo;
}

void CPlatformLinux::minimizeWindow()
{
	XIconifyWindow(m_display, m_window, m_screen);
}

void CPlatformLinux::maximizeWindow()
{
	XMapWindow(m_display, m_window);
}

void CPlatformLinux::restoreWindow()
{
	XMapWindow(m_display, m_window);
}

void CPlatformLinux::setFullScreen( bool value )
{
}

void CPlatformLinux::setResizeble ( bool value )
{
}

void CPlatformLinux::setWindowCaption( const std::string& text )
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

bool CPlatformLinux::isWindowMaximized() const
{
	return false;
}

bool CPlatformLinux::isWindowMinimized() const
{
	return false;
}

bool CPlatformLinux::isWindowActive() const
{
	return false;
}

bool CPlatformLinux::isWindowFocused() const
{
	return false;
}

void CPlatformLinux::createWindows()
{
	m_display = XOpenDisplay(NULL);
	if (!m_display)
	{
		//"Error: Failed to open X display"

		return;
	}

	int major, minor;
	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion(m_display, &major, &minor) ||
			(( major == 1) && (minor < 3)) || (major < 1) )
	{
		// "Invalid GLX version" );

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
	if ( !fbConfigs )
	{
		// "Failed to retrieve a framebuffer config\n" );

		return;
	}

	// Pick the FB config/visual with the most samples per pixel
	int bestFbc = -1, worstFbc = -1, bestNumSamp = -1, worstNumSamp = 999;
	for ( int i = 0; i < numConfigs; ++i )
	{
		XVisualInfo *visualInfo;
		visualInfo = glXGetVisualFromFBConfig( m_display, fbConfigs[i] );
		if ( visualInfo )
		{
			int sampBuf, samples;
			glXGetFBConfigAttrib( m_display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampBuf );
			glXGetFBConfigAttrib( m_display, fbConfigs[i], GLX_SAMPLES       , &samples );

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

		XFree( visualInfo );
	}

	m_glxFBConfigs = fbConfigs[ bestFbc ];

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
	winAttribs.event_mask        = StructureNotifyMask;
	//winAttribs.bit_gravity       = StaticGravity;
	//winAttribs.background_pixel  = WhitePixel(m_display, m_screen);
	//winAttribs.override_redirect = m_param.isFullscreen;

	m_window = XCreateWindow( m_display, RootWindow( m_display, visualInfo->screen ),
							0, 0, m_param.windowSize.width, m_param.windowSize.height, 0, visualInfo->depth, InputOutput,
							visualInfo->visual, CWBorderPixel|CWColormap|CWEventMask, &winAttribs );
	if ( !m_window )
	{
		// "Failed to create window."

		return;
	}

	XStoreName( m_display, m_window, "ValeraLinux" );

//	GLXContext glc = glXCreateContext(m_display, visualInfo, NULL, GL_TRUE);
//	glXMakeCurrent(m_display, m_window, glc);

//	// Create OpenGL rendering context
//	m_glxWindow = glXCreateWindow( m_display, m_glxFBConfigs, m_window, NULL );
//	if (m_glxWindow)
//	{
//		// create glx context
//		//m_context = glXCreateContext(m_display, visualInfo, None, True);
//		m_glxContext = glXCreateNewContext( m_display, m_glxFBConfigs, GLX_RGBA_TYPE, NULL, True );
//		if (m_glxContext)
//		{
//			//if (!glXMakeCurrent(m_display, m_window, m_context))
//			if (!glXMakeContextCurrent( m_display, m_glxWindow, m_glxWindow, m_glxContext ))
//			{
//				//"Could not make context current."
//				glXDestroyContext(m_display, m_glxContext);
//			}
//		}
//		else
//		{
//			//"Could not create GLX rendering context."
//		}
//	}
//	else
//	{
//		//"Could not create GLX window."
//	}

	XMapWindow(m_display, m_window);

	// Get the default screen's GLX extension list
	//const char *glxExts = glXQueryExtensionsString( m_display, DefaultScreen( m_display ) );

	// NOTE: It is not necessary to create or make current to a context before
	// calling glXGetProcAddressARB
	//glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	//glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );




	//XFree( visualInfo );
	XFlush(m_display);

	renderer::CDriverContext* driver = nullptr;
	driver = new renderer::CDriverContextGL(this);

	if (!driver->createContext())
	{
		//Error crete context
		CPlatformLinux::closeWindow();

		return;
	}
}

void CPlatformLinux::closeWindow()
{
	glXMakeCurrent( m_display, 0, 0 );
	glXDestroyContext( m_display, m_glxContext );

	XDestroyWindow( m_display, m_window );
	XCloseDisplay( m_display );
}

bool CPlatformLinux::begin()
{
	return true;
}

bool CPlatformLinux::end()
{
	glXSwapBuffers ( m_display, m_window );

	return true;
}

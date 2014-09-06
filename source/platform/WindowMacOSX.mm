#include "WindowMacOSX.h"


using namespace v3d;
using namespace v3d::platform;

CWindowMacOSX::CWindowMacOSX(const WindowParam& param)
    : CWindow(param)
{
    m_platformType = EPlatformType::ePlatformMacOSX;
}

CWindowMacOSX::~CWindowMacOSX()
{
}

void CWindowMacOSX::minimize()
{
}

void CWindowMacOSX::maximize()
{
}

void CWindowMacOSX::restore()
{
}

void CWindowMacOSX::setFullScreen( bool value )
{
}

void CWindowMacOSX::setResizeble( bool value )
{
}

void CWindowMacOSX::setPosition(const core::Dimension2D& pos)
{
    //m_param.position = pos;
}

void CWindowMacOSX::setCaption( const std::string& text )
{
}

bool CWindowMacOSX::isMaximized() const
{
    return false;
}

bool CWindowMacOSX::isMinimized() const
{
    return false;
}

bool CWindowMacOSX::isActive() const
{
    return false;
}

bool CWindowMacOSX::isFocused() const
{
    return false;
}

bool CWindowMacOSX::begin()
{
    return false;
}

bool CWindowMacOSX::end()
{
    return false;
}

void CWindowMacOSX::create()
{
    m_autoreleasePool = [[NSAutoreleasePool alloc] init];
    
    pthread_key_t   current;
    if (pthread_key_create(&current, NULL) != 0)
    {
        //"NSGL: Failed to create context TLS");
        return;
    }
    
    void* framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    if (framework == NULL)
    {
        //"NSGL: Failed to locate OpenGL framework");
        return;
    }
    
    void* currentContext = pthread_getspecific(current);
    
    
    if (NSApp)
    {
        return;
    }
    
    [NSApplication sharedApplication];
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    
    [NSApp finishLaunching];
    
    
    
    unsigned int styleMask = NSBorderlessWindowMask;
    
    styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
    //
    //        if (resizable)
    //            styleMask |= NSResizableWindowMask;
    //    }
    
    m_window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, m_param.size.width, m_param.size.height)
                                           styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
    if (m_window == nil)
    {
        // "Cocoa: Failed to create window");
        return;
    }
    
    [m_window setTitle:[NSString stringWithUTF8String:"ValeraMacOSX"]];
    [m_window setBackgroundColor:[NSColor whiteColor]];
    [m_window makeKeyAndOrderFront:nil];
	[NSMenu setMenuBarVisible:NO];
    [m_window setAcceptsMouseMovedEvents:YES];
    [m_window center];
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (floor(NSAppKitVersionNumber) >= NSAppKitVersionNumber10_7)
    {
        [m_window setRestorable:NO];
    }
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
    
//    if (m_window)
//    {
//        [m_context makeCurrentContext];
//    }
//    else
//    {
//        [NSOpenGLContext clearCurrentContext];
//    }
    //pthread_setspecific(m_current, m_window);}
}

void CWindowMacOSX::close()
{
//    [m_context release];
//    m_context = nil;

    [m_window release];
    m_window = nil;
    
    [m_autoreleasePool release];
}
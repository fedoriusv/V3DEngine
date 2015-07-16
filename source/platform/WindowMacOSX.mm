#include "WindowMacOSX.h"
#include "Utils/Logger.h"
#import "AppDelegate.h"
#import "WindowsDelegate.h"

#include <crt_externs.h>

NSString* findAppName(void)
{
    size_t i;
    NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
    
    NSString* nameKeys[] =
    {
        @"CFBundleDisplayName",
        @"CFBundleName",
        @"CFBundleExecutable",
    };
    
    for (i = 0;  i < sizeof(nameKeys) / sizeof(nameKeys[0]);  i++)
    {
        id name = [infoDictionary objectForKey:nameKeys[i]];
        if (name && [name isKindOfClass:[NSString class]] && ![name isEqualToString:@""])
        {
            return name;
        }
    }
    
    char** progname = _NSGetProgname();
    if (progname && *progname)
    {
        return [NSString stringWithUTF8String:*progname];
    }
    
    return @"MACOSX Application";
}

using namespace v3d;
using namespace utils;
using namespace platform;

CWindowMacOSX::CWindowMacOSX(const WindowParam& param)
    : CWindow(param)
    , m_close(false)
{
    m_platformType = EPlatformType::ePlatformMacOSX;
}

CWindowMacOSX::~CWindowMacOSX()
{
}

const WindowNS& CWindowMacOSX::getHandleWindow() const
{
    return m_window;
}

void CWindowMacOSX::minimize()
{
    [m_window._window miniaturize:nil];
}

void CWindowMacOSX::maximize()
{
    [m_window._window deminiaturize:nil];
}

void CWindowMacOSX::restore()
{
    [m_window._window deminiaturize:nil];
}

void CWindowMacOSX::setFullScreen(bool value)
{
    //TODO:
    
    m_param._isFullscreen = value;
}

void CWindowMacOSX::setResizeble(bool value)
{
    //TODO:
    
    m_param._isResizeble = value;
}

void CWindowMacOSX::setPosition(const core::Point2D& pos)
{
    const f32 height = CGDisplayBounds(CGMainDisplayID()).size.height;
    const NSRect contentRect = [m_window._view frame];
    const NSRect dummyRect = NSMakeRect(pos.x, height - (pos.y + contentRect.size.height), 0, 0);
    const NSRect frameRect = [m_window._window frameRectForContentRect:dummyRect];
    [m_window._window setFrameOrigin:frameRect.origin];
    
    m_param._position = pos;
}

void CWindowMacOSX::setCursorPosition(const core::Point2D& pos)
{
    const NSRect contentRect = [m_window._view frame];
    
    const NSPoint localPoint = NSMakePoint(pos.x, contentRect.size.height - pos.y - 1);
    const NSPoint globalPoint = [m_window._window convertBaseToScreen:localPoint];
    
    const f32 height = CGDisplayBounds(CGMainDisplayID()).size.height;
    CGWarpMouseCursorPosition(CGPointMake(globalPoint.x, height - globalPoint.y));
}

void CWindowMacOSX::setCaption( const std::string& text )
{
    [m_window._window setTitle:[NSString stringWithUTF8String:text.c_str()]];
}

bool CWindowMacOSX::isMaximized() const
{
    return [m_window._window isMiniaturized] == NO;
}

bool CWindowMacOSX::isMinimized() const
{
    return [m_window._window isMiniaturized] == YES;
}

bool CWindowMacOSX::isActive() const
{
    return [m_window._window isActive] == YES;
}

bool CWindowMacOSX::isFocused() const
{
    return [m_window._window isKeyWindow] == YES;
}

const core::Point2D& CWindowMacOSX::getPosition() const
{
    const NSRect contentRect = [m_window._window contentRectForFrameRect:[m_window._window frame]];
    
    m_param._position.x = contentRect.origin.x;
    
    const f32 height = CGDisplayBounds(CGMainDisplayID()).size.height;
    m_param._position.y = height - (contentRect.origin.y + contentRect.size.height);

    return m_param._position;
}

void CWindowMacOSX::getCursorPosition(core::Point2D& pos) const
{
    const NSRect contentRect = [m_window._view frame];
    const NSPoint cursor_pos = [m_window._window mouseLocationOutsideOfEventStream];
    
    pos.x = cursor_pos.x;
    pos.y = contentRect.size.height - cursor_pos.y - 1;
}

bool CWindowMacOSX::begin()
{
    while (true)
    {
        NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
        if (event == nil)
            break;
        
        [NSApp sendEvent:event];
    }
    
    [m_app._autoreleasePool drain];
    m_app._autoreleasePool = [[NSAutoreleasePool alloc] init];
    
    return !m_close;
}

bool CWindowMacOSX::end()
{
    NSOpenGLContext* context = [NSOpenGLContext currentContext];
    [context flushBuffer];
    
    return false;
}

void CWindowMacOSX::create()
{
    LOG_INFO("CWindowMacOSX::create: Create Windows");
    
    if (!CWindowMacOSX::init())
    {
        LOG_ERROR("CWindowMacOSX::create: Can't init macos window");
        return;
    }
    
    if (!CWindowMacOSX::initAppKit())
    {
        LOG_ERROR("CWindowMacOSX::create: Can't init app kit");
        return;
    }
    
    m_window._delegate = [[WindowsDelegate alloc] initWithWindow:&m_window];
    if (m_window._delegate == nil)
    {
        LOG_ERROR("CWindowMacOSX::create: Cocoa: Failed to create window delegate");
        return;
    }

    u32 styleMask = NSBorderlessWindowMask;
    
    styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
    if (m_param._isResizeble)
    {
        styleMask |= NSResizableWindowMask;
    }
    if (m_param._isFullscreen)
    {
        styleMask |= NSFullScreenWindowMask;
    }

    m_window._window = [[NSWindow alloc]
                        initWithContentRect:NSMakeRect(0, 0, m_param._size.width, m_param._size.height)
                                  styleMask:styleMask
                                    backing:NSBackingStoreBuffered
                                      defer:NO];
    
    if (m_window._window == nil)
    {
        LOG_ERROR("CWindowMacOSX::create: Cocoa: Failed to create window");
        return;
    }
    
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    {
        if (m_param._isResizeble)
        {
            [m_window._window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        }
    }

    m_window._view = [[ContentView alloc] initWithWindow:&m_window];
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    {
        [m_window._view setWantsBestResolutionOpenGLSurface:YES];
    }

    [m_window._window setTitle:[NSString stringWithUTF8String:"ValeraMacOSX"]];
    [m_window._window setBackgroundColor:[NSColor whiteColor]];
    [m_window._window makeKeyAndOrderFront:nil];
    [m_window._window setAcceptsMouseMovedEvents:YES];
    [m_window._window center];
    [m_window._window setDelegate:m_window._delegate];
    [m_window._window setContentView:m_window._view];
    
    if (floor(NSAppKitVersionNumber) >= NSAppKitVersionNumber10_7)
    {
        [m_window._window setRestorable:NO];
    }
    
    [NSApp activateIgnoringOtherApps:YES];
    //[NSApp finishLaunching];
    
    LOG_INFO("CWindowMacOSX::create: Window Size (%d, %d)", m_param._size.width, m_param._size.height);
    
    CWindowMacOSX::addKeyCodes();
}

void CWindowMacOSX::close()
{
    m_close = true;
    
    [m_window._view release];
    m_window._view = nil;
    
    [m_window._window setDelegate:nil];
    [m_window._delegate release];
    m_window._delegate = nil;

    [m_window._window close];
    m_window._window = nil;
    
    CWindowMacOSX::terminate();
}

bool CWindowMacOSX::init()
{
    m_app._autoreleasePool = [[NSAutoreleasePool alloc] init];
    
    m_app._eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    if (!m_app._eventSource)
    {
        LOG_ERROR("CWindowMacOSX::init: Failed to create event source");
        return false;
    }
    
    CGEventSourceSetLocalEventsSuppressionInterval((CGEventSourceRef)m_app._eventSource, 0.0);
    
    CFBundleRef framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    if (framework == nullptr)
    {
        LOG_ERROR("CWindowMacOSX::init: Failed to locate OpenGL framework");
        return false;
    }
    
    return true;
}

bool CWindowMacOSX::initAppKit()
{
    if (NSApp)
    {
        LOG_WARNING("CWindowMacOSX::initAppKit: already exist");
        return true;
    }
    
    [Application sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    //TODO: create menubar if need
    /*NSString* appName = findAppName();
    
    NSMenu* bar = [[NSMenu alloc] init];
    [NSApp setMainMenu:bar];
    
    NSMenuItem* appMenuItem =
    [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    NSMenu* appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];
    
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName]
                       action:@selector(orderFrontStandardAboutPanel:)
                keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];*/
    
    m_app._delegate = [[AppDelegate alloc] init];
    if (m_app._delegate == nil)
    {
        LOG_ERROR("CWindowMacOSX::initAppKit:Cocoa: Failed to create application delegate");
        return false;
    }
    
    [NSApp setDelegate:m_app._delegate];
    [NSApp run];

    return true;
}

void CWindowMacOSX::terminate()
{
    if (m_app._eventSource)
    {
        CFRelease(m_app._eventSource);
        m_app._eventSource = nullptr;
    }
    
    if (m_app._delegate)
    {
        [NSApp setDelegate:nil];
        [m_app._delegate release];
        m_app._delegate = nil;
    }

    [m_app._autoreleasePool release];
    m_app._autoreleasePool = nil;
}

void CWindowMacOSX::addKeyCodes()
{
}
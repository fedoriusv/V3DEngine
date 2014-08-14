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
//    m_autoreleasePool = [[NSAutoreleasePool alloc] init];
    
//    if (NSApp)
//    {
//        return;
//    }
//    
//    [NSApplication sharedApplication];
    
//    NSLog(@"%@", [[[NSApplication sharedApplication]delegate] description]);
    int a = 0;
}

void CWindowMacOSX::close()
{
//    [m_autoreleasePool release];
}
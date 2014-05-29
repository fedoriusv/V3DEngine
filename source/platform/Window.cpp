#include "Window.h"

using namespace v3d;
using namespace v3d::platform;

CWindow::CWindow(const WindowParam& param)
	: m_param(param)
	, m_platformType(EPlatformType::ePlatformNull)
{
}

CWindow::~CWindow()
{
}

bool CWindow::isFullscreen() const
{
	return m_param.isFullscreen;
}

bool CWindow::isResizeble() const
{
	return m_param.isResizeble;
}

const core::Dimension2D& CWindow::getSize() const
{
	return m_param.size;
}

const core::Dimension2D& CWindow::getPosition() const
{
	return m_param.position;
}

const EPlatformType CWindow::getPlatformType() const
{
	return m_platformType;
}

const EDriverType CWindow::getDriverType() const
{
	return m_param.driverType;
}

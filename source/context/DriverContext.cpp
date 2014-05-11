#include "DriverContext.h"

using namespace f3d;
using namespace f3d::renderer;

CDriverContext::CDriverContext(const platform::CWindowPtr& window)
	: m_window(window)
{
}

CDriverContext::~CDriverContext()
{
}

const platform::CWindowPtr CDriverContext::getWindow() const
{
	return m_window;
}

const core::Dimension2D& CDriverContext::getViewport() const
{
	return m_window->getSize();
}
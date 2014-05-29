#include "DriverContext.h"

using namespace v3d;
using namespace v3d::renderer;

CDriverContext::CDriverContext(const platform::WindowPtr& window)
	: m_window(window)
{
}

CDriverContext::~CDriverContext()
{
}

const platform::WindowPtr CDriverContext::getWindow() const
{
	return m_window;
}

const core::Dimension2D& CDriverContext::getViewport() const
{
	return m_window->getSize();
}
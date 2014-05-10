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
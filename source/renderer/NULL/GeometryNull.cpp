#include "GeometryNull.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

using namespace v3d;
using namespace renderer;


CGeometryNull::CGeometryNull(const CRenderTechnique* technique)
    : Geometry(technique)
{
}

CGeometryNull::~CGeometryNull()
{
}

void CGeometryNull::init()
{
}

void CGeometryNull::draw()
{
}

void CGeometryNull::free()
{
}

void CGeometryNull::refresh()
{
}

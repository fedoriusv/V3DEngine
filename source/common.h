#pragma once

#include "Config.h"

namespace v3d
{
    typedef unsigned char       u8;
    typedef signed char         s8;
    typedef char                c8;
    typedef unsigned short      u16;
    typedef signed short        s16;
    typedef unsigned int        u32;
    typedef signed int          s32;
    typedef unsigned long long  u64;
    typedef signed long long    s64;
    typedef float               f32;
    typedef double              f64;
    typedef long double         f80;
}

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <memory>
#include <fstream>
#include <functional>
#include <utility>

#include <assert.h>
#include <time.h>
#include <cstdarg>
#include <cstring>
#include <stdio.h>

#include <iostream>

#ifdef _DEBUG
#   define ASSERT(x) assert(x)
#else
#   define ASSERT(x)
#endif

#include "core/Math.h"
#include "core/Vector2D.h"
#include "core/Vector3D.h"
#include "core/Vector4D.h"
#include "core/Matrix2D.h"
#include "core/Matrix3D.h"
#include "core/Matrix4D.h"
#include "core/Dimension2D.h"
#include "core/Rect.h"

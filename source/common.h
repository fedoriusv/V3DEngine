#pragma once

#include "Config.h"

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <array>
#include <algorithm>
#include <memory>
#include <mutex>
#include <atomic>
#include <fstream>
#include <numeric>
#include <functional>
#include <utility>

#include <type_traits>

#include <assert.h>
#include <time.h>
#include <cstdarg>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename E>
    constexpr auto              toEnumType(E enumerator) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace v3d

#ifdef _DEBUG
#   define ASSERT(x, message) assert(x && message)
#else //_DEBUG
#   define ASSERT(x, message)
#endif //_DEBUG

#ifdef __GNUC__
#   define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#   if defined(DEPRECATED)
#       undef DEPRECATED
#   endif //defined(DEPRECATED)
#   define DEPRECATED __declspec(deprecated)
#else //defined(_MSC_VER)
#   pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#   define DEPRECATED
#endif //__GNUC__

#include "core/Math.h"
#include "core/Vector2D.h"
#include "core/Vector3D.h"
#include "core/Vector4D.h"
#include "core/Matrix2D.h"
#include "core/Matrix3D.h"
#include "core/Matrix4D.h"
#include "core/Dimension2D.h"
#include "core/Dimension3D.h"
#include "core/Rect.h"

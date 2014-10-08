#pragma once

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

#define USE_LOGGER 1
#define USE_CONSOLE 1

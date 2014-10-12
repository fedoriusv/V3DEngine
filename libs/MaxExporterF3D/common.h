#pragma once

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

#ifdef _DEBUG
#   define USE_LOGGER 1
#   define USE_CONSOLE 1
#endif
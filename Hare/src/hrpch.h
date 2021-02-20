#pragma once

// hrpch stands for "Hare precompiled headers". 
// Infact this file contains the most used / common header files used in the engine solution. 
// The purpose of the precompiled headers is to reduce the precompile time.
// Each .cpp file must include this file and it must be the first to include else it raise an error.

#include "Hare/Core/PlatformDetection.h"

#ifdef HR_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hare/Core/Base.h"

#include "Hare/Core/Log.h"

#include "Hare/Debug/Instrumentation.h"

#ifdef HR_PLATFORM_WINDOWS
#include <Windows.h>
#endif



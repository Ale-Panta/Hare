#pragma once

/*
hrpch stands for "Hare precompiled headers". 
Infact this file contains the most used / common header files used in the engine solution. 
The purpose of the precompiled headers is to reduce the precompile time.
Each .cpp file must include this file and it must be the first to include else it raise an error.
*/

// ---Common Stuff---
#include <memory>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>

// ---Data Structure---
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hare/Core/Log.h"

#ifdef HR_PLATFORM_WINDOWS
	#include <Hare/Core/Window.h>
#endif


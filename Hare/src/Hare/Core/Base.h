#pragma once

#include <memory>

#include "Hare/Core/PlatformDetection.h"

#ifdef HR_DEBUG
#if defined(HR_PLATFORM_WINDOWS)
#define HR_DEBUGBREAK() __debugbreak()
#elif defined(HR_PLATFORM_LINUX)
#include <signal.h>
#define HR_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define HR_ENABLE_ASSERTS
#else
#define HR_DEBUGBREAK()
#endif

#define HR_EXPAND_MACRO(x) x
#define HR_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define HR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Hare
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Hare/Core/Log.h"
#include "Hare/Core/Assert.h"

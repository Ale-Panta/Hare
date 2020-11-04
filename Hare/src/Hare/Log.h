#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hare
{
	/// <summary>
	/// Wrapper class of spdlog
	/// </summary>
	/// <see cref="$(SolutionDir)Hare/vendor/spdlog/"/>
	class HARE_API Log 
	{
	private:
		// Two consoles:
		// Engine console,
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		// Client console.
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

// ---Core Log Macros---
#define HR_CORE_TRACE(...)	::Hare::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HR_CORE_INFO(...)	::Hare::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HR_CORE_WARN(...)	::Hare::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HR_CORE_ERROR(...)	::Hare::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HR_CORE_FATAL(...)	::Hare::Log::GetCoreLogger()->fatal(__VA_ARGS__)
// ---------------------

// ---Client Log Macros---
#define HR_TRACE(...)		::Hare::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HR_INFO(...)		::Hare::Log::GetClientLogger()->info(__VA_ARGS__)
#define HR_WARN(...)		::Hare::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HR_ERROR(...)		::Hare::Log::GetClientLogger()->error(__VA_ARGS__)
#define HR_FATAL(...)		::Hare::Log::GetClientLogger()->fatal(__VA_ARGS__)
// ----------------------
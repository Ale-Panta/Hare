#include "hrpch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hare
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Create Engine console
		s_CoreLogger = spdlog::stdout_color_mt("HARE");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Create Client console
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
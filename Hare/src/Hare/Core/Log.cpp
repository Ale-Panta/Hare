#include "hrpch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace std;
using namespace spdlog;

namespace Hare
{
	shared_ptr<logger> Log::s_CoreLogger;
	shared_ptr<logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		set_pattern("%^[%T] %n: %v%$");

		// Create Engine console
		s_CoreLogger = stdout_color_mt("HARE");
		s_CoreLogger->set_level(level::trace);

		// Create Client console
		s_ClientLogger = stdout_color_mt("APP");
		s_ClientLogger->set_level(level::trace);
	}
}
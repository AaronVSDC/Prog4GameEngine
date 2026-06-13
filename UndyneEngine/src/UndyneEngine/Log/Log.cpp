#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


namespace UndyneEngine::Log
{
	void init()
	{
		//begin color,{thread}, [timestamp],logger name, actual message, end color
		spdlog::set_pattern("%^{%t} [%T] %n %l: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_CoreLogger->info("LOGGER INITIALIZED");
	}


}
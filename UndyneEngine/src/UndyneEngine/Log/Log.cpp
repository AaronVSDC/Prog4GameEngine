#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


namespace UndyneEngine
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::init()
	{
		//begin color,{thread}, [timestamp],logger name, actual message, end color
		spdlog::set_pattern("%^{%t} [%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("UNDYNE ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_CoreLogger->info("LOGGER INITIALIZED");
	}


}
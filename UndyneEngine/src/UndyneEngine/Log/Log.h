#ifndef LOG_H
#define LOG_H

//std
#include <memory>

#include "spdlog/spdlog.h"

namespace UndyneEngine
{
	namespace Log
	{
		void init();

		inline std::shared_ptr<spdlog::logger> s_CoreLogger;
		inline std::shared_ptr<spdlog::logger> s_ClientLogger;

	}
}

#ifdef UDE_DEBUG

#define UDE_CORE_CRITICAL(...) ::UndyneEngine::Log::s_CoreLogger->critical(__VA_ARGS__)  
#define UDE_CORE_ERROR(...)    ::UndyneEngine::Log::s_CoreLogger->error(__VA_ARGS__)
#define UDE_CORE_WARN(...)     ::UndyneEngine::Log::s_CoreLogger->warn(__VA_ARGS__) 
#define UDE_CORE_TRACE(...)    ::UndyneEngine::Log::s_CoreLogger->trace(__VA_ARGS__)  
#define UDE_CORE_INFO(...)     ::UndyneEngine::Log::s_CoreLogger->info(__VA_ARGS__) 
								
#define UDE_CRITICAL(...)	   ::UndyneEngine::Log::s_ClientLogger->critical(__VA_ARGS__) 
#define UDE_ERROR(...)	       ::UndyneEngine::Log::s_ClientLogger->error(__VA_ARGS__)
#define UDE_WARN(...)	       ::UndyneEngine::Log::s_ClientLogger->warn(__VA_ARGS__) 
#define UDE_TRACE(...)	       ::UndyneEngine::Log::s_ClientLogger->trace(__VA_ARGS__) 
#define UDE_INFO(...)	       ::UndyneEngine::Log::s_ClientLogger->info(__VA_ARGS__)

#define UDE_INITIALIZE_LOGGER  ::UndyneEngine::Log::init();

#endif

#ifdef UDE_RELEASE

#define UDE_CORE_CRITICAL 
#define UDE_CORE_ERROR    
#define UDE_CORE_WARN     
#define UDE_CORE_TRACE
#define UDE_CORE_INFO

#define UDE_CRITICAL
#define UDE_ERROR    
#define UDE_WARN	       
#define UDE_TRACE	        
#define UDE_INFO	 

#define UDE_INITIALIZE_LOGGER


#endif


#endif


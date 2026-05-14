#ifndef LOG_H
#define LOG_H

//std
#include <memory>

#include "..\Core\Core.h"
#include "spdlog\spdlog.h" 

namespace UndyneEngine
{
	class Log final
	{
	public:
		Log(Log& other) = delete; 
		Log(Log&& other) = delete; 
		Log& operator=(Log& rhs) = delete; 
		Log& operator=(Log&& rhs) = delete; 

		static UNDYNE_ENGINE_API void init(); 

		static UNDYNE_ENGINE_API std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		static UNDYNE_ENGINE_API std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

#ifdef UDE_DEBUG

#define UDE_CORE_CRITICAL(...) ::UndyneEngine::Log::getCoreLogger()->critical(__VA_ARGS__)  
#define UDE_CORE_ERROR(...)    ::UndyneEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define UDE_CORE_WARN(...)     ::UndyneEngine::Log::getCoreLogger()->warn(__VA_ARGS__) 
#define UDE_CORE_TRACE(...)    ::UndyneEngine::Log::getCoreLogger()->trace(__VA_ARGS__)  
#define UDE_CORE_INFO(...)     ::UndyneEngine::Log::getCoreLogger()->info(__VA_ARGS__) 
								
#define UDE_CRITICAL(...)	   ::UndyneEngine::Log::getClientLogger()->critical(__VA_ARGS__) 
#define UDE_ERROR(...)	       ::UndyneEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define UDE_WARN(...)	       ::UndyneEngine::Log::getClientLogger()->warn(__VA_ARGS__) 
#define UDE_TRACE(...)	       ::UndyneEngine::Log::getClientLogger()->trace(__VA_ARGS__) 
#define UDE_INFO(...)	       ::UndyneEngine::Log::getClientLogger()->info(__VA_ARGS__)

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


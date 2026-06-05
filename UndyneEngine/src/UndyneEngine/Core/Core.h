#ifndef CORE_H
#define CORE_H


#ifdef UDE_PLATFORM_WINDOWS
	#ifdef UDE_BUILD_DLL
		#define UNDYNE_API __declspec(dllexport)
		#else
		#define UNDYNE_API __declspec(dllimport)
	#endif
#else
	#error Undyne Engine only supports Windows.
#endif


#endif

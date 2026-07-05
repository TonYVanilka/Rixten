#pragma once

#ifdef _WIN32 // Windows

	#ifdef RIXTEN_EXPORTS
		#define RIXTEN_API __declspec(dllexport)
	#else
		#define RIXTEN_API __declspec(dllimport)
	#endif // RIXTEN_EXPORTS

#elif defined(__GNUC__) || defined(__clang__) // Linux and Mac

    #define RIXTEN_API __attribute__((visibility("default")))

#else
    #error Compiler is not available! 
	#define RIXTEN_API
#endif
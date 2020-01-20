#pragma once

#ifdef SLD_PLATFORM_WINDOWS
	#ifdef SLD_BUILD_DLL
		#define SALAD_API __declspec(dllexport)
	#else
		#define SALAD_API __declspec(dllimport)
	#endif
#else
	#error Salad only supports Windows!
#endif
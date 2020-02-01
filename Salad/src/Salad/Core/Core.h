#pragma once

#include <memory>

#ifdef SLD_PLATFORM_WINDOWS
	#if SLD_DYNAMIC_LINK
		#ifdef SLD_BUILD_DLL
			#define SALAD_API __declspec(dllexport)
		#else
			#define SALAD_API __declspec(dllimport)
		#endif
	#else
		#define SALAD_API
	#endif
#else
	#error Salad only supports Windows!
#endif

#ifdef SLD_DEBUG
	#define SLD_ENABLE_ASSERTS
#endif

#ifdef SLD_ENABLE_ASSERTS
	#define SLD_ASSERT(x, ...) { if(!x) {SLD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SLD_CORE_ASSERT(x, ...) { if(!x) {SLD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SLD_ASSERT(x, ...)
	#define SLD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (x << 1)

#define SLD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Salad {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
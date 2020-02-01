#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Salad {

	class SALAD_API Log
	{
	public:
		Log();
		~Log();

		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define SLD_CORE_TRACE(...) ::Salad::Log::getCoreLogger()->trace(__VA_ARGS__)
#define SLD_CORE_INFO(...)  ::Salad::Log::getCoreLogger()->info(__VA_ARGS__)
#define SLD_CORE_WARN(...)  ::Salad::Log::getCoreLogger()->warn(__VA_ARGS__)
#define SLD_CORE_ERROR(...) ::Salad::Log::getCoreLogger()->error(__VA_ARGS__)
#define SLD_CORE_FATAL(...) ::Salad::Log::getCoreLogger()->fatal(__VA_ARGS__)

#define SLD_TRACE(...) ::Salad::Log::getClientLogger()->trace(__VA_ARGS__)
#define SLD_INFO(...)  ::Salad::Log::getClientLogger()->info(__VA_ARGS__)
#define SLD_WARN(...)  ::Salad::Log::getClientLogger()->warn(__VA_ARGS__)
#define SLD_ERROR(...) ::Salad::Log::getClientLogger()->error(__VA_ARGS__)
#define SLD_FATAL(...) ::Salad::Log::getClientLogger()->fatal(__VA_ARGS__)

#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Salad {

	struct ProfilerResult {
		std::string name;
		long long start, end;
		uint32_t threadId;
	};

	struct ProfilingSession {
		std::string name;
	};

	class Profiler {
	
		friend class ProfilerTimer;

	public:
		Profiler() : 
			m_CurrentSession(nullptr),
			m_ProfileCount(0)
		{}

		void beginSession(const std::string& name, const std::string& filepath = "results-profiling.json") {
			m_OutputStream.open(filepath);
			writeHeader();
			m_CurrentSession = new ProfilingSession{ name };
		}

		void endSession() {
			writeFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		static Profiler& get() {
			static Profiler profiler;
			return profiler;
		}
	private:
		void writeHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void writeProfile(const ProfilerResult& result) {
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.end - result.start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.threadId << ",";
			m_OutputStream << "\"ts\":" << result.start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void writeFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

	private:
		ProfilingSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	class ProfilerTimer {

	public:
		ProfilerTimer(const char* name) :
			m_Name(name), 
			m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~ProfilerTimer(){
			if (!m_Stopped)
				stop();
		}

		void stop() {
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

			uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Profiler::get().writeProfile({ m_Name, start, end, threadId });

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#define SLD_PROFILING_ENABLED 1
#ifdef SLD_PROFILING_ENABLED
	#define SLD_PROFILER_BEGIN(name, filepath) ::Salad::Profiler::get().beginSession(name, filepath)
	#define SLD_PROFILER_END() ::Salad::Profiler::get().endSession();
	#define SLD_PROFILER_SCOPE(name) ::Salad::ProfilerTimer timer##__LINE__(name);
	#define SLD_PROFILER_FUNC() SLD_PROFILER_SCOPE(__FUNCSIG__)
#else
	#define SLD_PROFILER_BEGIN(name, filepath)
	#define SLD_PROFILER_END()
	#define SLD_PROFILER_SCOPE(name)
	#define SLD_PROFILER_FUNC()
#endif

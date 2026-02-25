
#ifndef MINIGIN_H
#define MINIGIN_H

#include <string>
#include <functional>
#include <filesystem>
#include <chrono>
namespace dae
{
	class Minigin final
	{
		bool m_quit{};
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	
		int m_DesiredFPS = 60; 
		int m_FrameTimeMs = 1000 / m_DesiredFPS; 
		const float m_FixedTimeStep = 0.02f;
		float m_Lag = 0.f; 
		float m_DeltaTime;
		std::chrono::steady_clock::time_point m_CurrentTime; 
		std::chrono::steady_clock::time_point m_LastTime;
	};
}

#endif
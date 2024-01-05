#pragma once
#include<thread>
#include<chrono>
class Timer
{
private:
	std::chrono::steady_clock::time_point Last;
	[[nodiscard]] __inline bool Check(unsigned ms) const
	{
		const auto curTime = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(curTime - Last).count() >= ms;
	}
public:
	Timer()
	{
		this->Last = std::chrono::steady_clock::now();
	}
	__inline bool Initialize();
	__inline bool Run(unsigned ms)
	{
		if (this->Check(ms))
		{
			this->Last = std::chrono::steady_clock::now();
			return true;
		}
		return false;
	}
};
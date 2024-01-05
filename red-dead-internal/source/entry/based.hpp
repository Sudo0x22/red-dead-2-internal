#pragma once
#include"../utils/utils.hpp"
#include"../interfaces/interfaces.hpp"

class EntryPoint
{
private:
	bool m_bInitialize = false;
public:
	__inline void Load()
	{
		while (!this->m_bInitialize)
		{
			printf("WOW\n");
			this->m_bInitialize = true;
		}
	}
	__inline void Unload()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
public:
	__inline bool ShouldUnload()
	{
		if (MyRtlKeyPress(VK_END) & 0x1) { this->m_bShouldUnload = true; }
		return m_bShouldUnload;
	}
private:
	bool m_bShouldUnload = false;
}; inline EntryPoint* pEntryPoint = nullptr;
#pragma once
#include"menu/menu.hpp"
#include"player/player.hpp"
#include"horse/horse.hpp"
#include"recovery/recovery.hpp"

class Features
{
public:
	__inline bool Initialize() 
	{
		static bool m_bInitialize = false;
		while (!m_bInitialize)
		{
			F::pPlayer->Run();
			F::pHorse->Run();
			F::pRecovery->Run();
		}
		return true;
	}
}; inline Features g_Features; 
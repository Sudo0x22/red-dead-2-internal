#pragma once
#include"present.hpp"
class Detours
{
private:
	bool m_bHooked = false;
public:
	__inline bool Initialize()
	{
		if (pHook->Initialize() != MH_STATUS::MH_OK) { return false; }
		



		
		if (pHook->EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK) { return false; }
		
		return true;
	}
}; namespace D { inline Detours g_Detours; }
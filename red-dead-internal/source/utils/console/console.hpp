#pragma once
#include"../memory/memory.hpp"
typedef BOOL(__stdcall* _RtlAllocConsole)();
_RtlAllocConsole RtlAllocConsole()
{
	auto hModule = pWinApi->GetModuleName("user32.dll");
	if (!hModule)
		return FALSE;
	_RtlAllocConsole CreateConsole = (_RtlAllocConsole)pWinApi->GetModuleProcAddr(hModule, "AllocConsole");
	CreateConsole();
}
class Console
{
private:
	FILE* file = {};
	const char* console_title = "Red Dead 2 Money Hack";
public:
	__inline void CreateConsole(const char* title)
	{
		RtlAllocConsole();
		freopen_s(&file, "CONOUT$", "w", stdout);
		SetConsoleTitleA(this->console_title);
	}
}; inline Console* pConsole = nullptr;
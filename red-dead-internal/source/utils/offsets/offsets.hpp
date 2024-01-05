#pragma once
#include"../scanner/scanner.hpp"
#define MAKE_SIG(x, mod, patt) x = gUtil_Scanner.Find(mod, patt);
class CUtil_Sigs
{
public:
	__inline bool Initialize() { return true; }
}; inline CUtil_Sigs gUtils_Sigs;
class CUtils_Offsets
{
public:
	uintptr_t module_base = 0x00;
	uintptr_t story_mode_money = 0x00;
public:
	std::vector<uintptr_t> story_mode_money_bytes = { 0x00,0x00 };
public:
	__inline bool Initialize()
	{
		this->module_base = (uintptr_t)pWinApi->GetModuleName("RDR2.exe");
		return true;
	}
}; inline CUtils_Offsets gUtils_Offsets;
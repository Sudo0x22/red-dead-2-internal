#pragma once
#include"../wndproc/wndproc.hpp"
oCursor_t oCursor = 0;

namespace cursor
{
	__inline BOOL WINAPI CursorHook(bool open)
	{
		if (pMenu.m_bMenuToggled)
			return FALSE;
		return originals::oCursor(open);
	}
	__inline bool CallHook(LPVOID nFunc, LPVOID nDet, LPVOID* nOrig)
	{
		if (pHook->CreateHook(nFunc, nDet, nOrig) != MH_OK)
			return false;
		if (pHook->EnableHook(nFunc) != MH_OK)
			return false;
		return true;
	}
	__inline void KillHook(LPVOID nFunc)
	{
		pHook->DisableHook(nFunc);
		pHook->RemoveHook(nFunc);
	}
}
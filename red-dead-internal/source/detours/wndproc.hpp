#pragma once
#include"../features/features.hpp"

namespace HookDetours
{
	namespace WndProc
	{
		LRESULT __stdcall Detour(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam)
		{
			if (pMenu.m_bMenuToggled)
			{
				ImGui_ImplWin32_WndProcHandler(hWnd, uInt, wParam, lParam);
				return TRUE;
			}

			return pWinApiEx->MyRtlCallWindowProc(originals::oWndProc, hWnd, uInt, wParam, lParam);
		}
	}
}
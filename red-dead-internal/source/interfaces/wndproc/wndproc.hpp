#pragma once
#include"../../features/features.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
oWndProc_t oWndProc = 0;

extern "C" {namespace WndProc {
	LRESULT WINAPI Detour(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (pMenu.m_bMenuToggled) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			return TRUE;
		}
		Window::CallWndProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}
}}
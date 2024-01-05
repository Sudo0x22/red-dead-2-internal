#pragma once
#include"present/present.hpp"

extern "C" {namespace modules {
	DWORD WINAPI NtWindowThreadId(HWND hWindow, LPDWORD Pointer) {
		return spoof_call_ex(JmpRbx, __safecall(GetWindowThreadProcessId).get(), hWindow, Pointer); }
	HWND WINAPI NtForgroundWindow() {
		return spoof_call_ex(JmpRbx, __safecall(GetForegroundWindow).get()); }
	DWORD WINAPI NtCurrentProcessID() {
		return spoof_call_ex(JmpRbx, __safecall(GetCurrentProcessId).get());}
}}

extern "C" {namespace threads
{
	NTSTATUS APIENTRY InitPresentHook(LPVOID hBuffer)
	{
		bool Wnd = false;
		while (!Wnd)
		{
			DWORD pID;
			modules::NtWindowThreadId(modules::NtForgroundWindow(), &pID);
			if (modules::NtCurrentProcessID() == pID)
			{
				Handles::pId = modules::NtCurrentProcessID();
				Handles::Handle = GetCurrentProcess;
				Handles::hWindow = modules::NtForgroundWindow();
				Wnd = true;
			}
		}

		bool init_hook = false;
		while (!init_hook)
		{
			if (DX12::Initialize() == true)
			{
				pHook->MakeHook(54,present::CommandList, (LPVOID*)&oCommand);
				pHook->MakeHook(140,  present::hook_present, (LPVOID*)&oPresent);
				init_hook = true;
			}
		}

		return TRUE;
	}
}}
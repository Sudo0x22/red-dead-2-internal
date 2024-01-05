#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include<string>
typedef struct SysMem
{
	DWORD64 dwEP;
	LPVOID dwParam;
} SysMem, * pSysMem;
typedef DWORD(*InitMem)(LPVOID Param);
class Api
{
public:
	typedef BOOL(__stdcall* _RtlSetConsoleTitlA)(LPCSTR);
	typedef BOOL(__stdcall* _RtlCloseHandle)(HANDLE);
	typedef BOOL(__stdcall* _RtlWriteProcessMemory)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
	
public:
	BOOL __stdcall MyRtlSetConsoleTitleA(LPCSTR Title)
	{
		auto hModule = GetModuleHandleA("kernel32.dll");
		_RtlSetConsoleTitlA RtlSetConsoleTitleA = (_RtlSetConsoleTitlA)
			GetProcAddress(hModule, "SetConsoleTitleA");
		return RtlSetConsoleTitleA(Title);
	}
	BOOL __stdcall MyRtlCloseHandle(HANDLE Handle)
	{
		auto hModule = GetModuleHandleA("kernel32.dll");
		_RtlCloseHandle RtlCloseHandle = (_RtlCloseHandle)
			GetProcAddress(hModule, "CloseHandle");
		return RtlCloseHandle(Handle);
	}
	BOOL __stdcall MyRtlWriteProcessMemory(HANDLE hProc, LPVOID hBase, LPCVOID hSrc, SIZE_T hSize, SIZE_T* hBytes)
	{
		auto hModule = GetModuleHandleA("kernel32.dll");
		_RtlWriteProcessMemory RtlWriteProcessMemory = (_RtlWriteProcessMemory)
			GetProcAddress(hModule, "WriteProcessMemory");
		return RtlWriteProcessMemory(hProc, hBase, hSrc, hSize, hBytes);
	}
}; Api* pApi = new Api();
#pragma once
#include"../winapi/winapi.hpp"
typedef void* (__stdcall* _RtlCreateRemoteThread)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
void* __stdcall RtlCreateRemoteThread(HANDLE Handle, LPSECURITY_ATTRIBUTES Attr, SIZE_T Size,
	LPTHREAD_START_ROUTINE StartRoutine, LPVOID Buffer, DWORD Flags, LPDWORD ThreadId)
{
	auto hModule = pWinApi->GetModuleName(skCrypt("kernel32.dll").decrypt());
	if (!hModule)
		return nullptr;
	_RtlCreateRemoteThread _CreateRemoteThread = (_RtlCreateRemoteThread)pWinApi->GetModuleProcAddr(hModule, skCrypt("CreateRemoteThread").decrypt());
	return _CreateRemoteThread(Handle, Attr, Size, StartRoutine, Buffer, Flags, ThreadId);
}
void __stdcall InitThread(pSysMem mMemory)
{
	if (mMemory != NULL && mMemory->dwEP != NULL)
	{
		InitMem CallMem = (InitMem)mMemory->dwEP;
		CallMem(mMemory->dwParam);
	}
}
class Thread
{
private:
	HANDLE hThread = nullptr;
	HMODULE hModule = 0;
	DWORD64 hModuleSize = 0;
	PBYTE hModuleBytes = nullptr;
private:
	__inline HANDLE MakeRemoteThread(HANDLE Handle, LPSECURITY_ATTRIBUTES Attr, SIZE_T Size,
		LPTHREAD_START_ROUTINE StartRoutine, LPVOID Buffer, DWORD Flags, LPDWORD ThreadId)
	{
		this->hThread = spoof_call_ex(JmpRbx, __safecall(CreateRemoteThread).get(), Handle, Attr, Size, StartRoutine, Buffer, Flags, ThreadId);
		pWinApiEx->MyRtlCloseHandle(this->hThread);
		return NULL;
	}
public:
	__inline HANDLE MakeThread(LPTHREAD_START_ROUTINE StartRoutine, LPVOID Buffer, LPDWORD ThreadId)
	{
		this->hModule = pWinApi->GetModuleName(skCrypt("ntdll.dll").decrypt());
		this->hModuleSize = pModuleApi->GetModuleSize((DWORD64)this->hModule);
		this->hModuleBytes = pModuleApi->GetModuleBytes(this->hModule, this->hModuleSize, 0x400);

		DWORD Protect;
		pWinApiEx->MyRtlVirtualProtect(this->hModuleBytes, 0x100, PAGE_EXECUTE_READWRITE, &Protect);
		
		pSysMem mMemory = (pSysMem)pWinApiEx->MyRtlVirtualAlloc(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		mMemory->dwEP = (DWORD64)(StartRoutine);
		mMemory->dwParam = Buffer;
		
		pWinApi->GetMemCpy((LPVOID)this->hModuleBytes, (LPVOID)InitThread, 0x100);
		this->hThread = RtlCreateRemoteThread(pWinApiEx->MyRtlGetCurrentProcess(), 0, 0x100, (LPTHREAD_START_ROUTINE)this->hModuleBytes, mMemory, 0, ThreadId);
		pWinApiEx->MyRtlCloseHandle(this->hThread);
		
		return NULL;
	}
}; Thread* pThread = new Thread();

#pragma once
#include"defines.hpp"
class Process
{
public:
	__inline uintptr_t GetProcessById(const wchar_t* const zsModule)
	{
		PROCESSENTRY32W ProcEntry = {};
		ProcEntry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE ProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (ProcSnap == INVALID_HANDLE_VALUE) return -1;
		if (Process32FirstW(ProcSnap, &ProcEntry) == FALSE)return -1;
		while (Process32NextW(ProcSnap, &ProcEntry))
		{
			if (!wcscmp(ProcEntry.szExeFile, zsModule)) { return (uintptr_t)ProcEntry.th32ProcessID; pApi->MyRtlCloseHandle(ProcSnap); }
		}
		pApi->MyRtlCloseHandle(ProcSnap);
		return 0;
	}
}; Process* pProcess = new Process();
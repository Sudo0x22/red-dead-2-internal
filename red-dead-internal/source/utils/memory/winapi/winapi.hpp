#pragma once
#include"../protection/global.h"
#include"../../../../includes/spoofer/spoofer.hpp"
#pragma section(".text")
__declspec(allocate(".text")) unsigned const char JmpRbx[] = { 0xff, 0x23 };
typedef struct SysMem
{
	DWORD64 dwEP;
	LPVOID dwParam;
}SysMem, * pSysMem;
typedef DWORD(*InitMem)(LPVOID Param);
typedef struct GameMemory
{
	DWORD64 dwGameBase;
}GameMemory, * pGameMemory;
class WinApi
{
public:
	__inline HMODULE GetModuleName(LPCSTR ModuleImage) { return spoof_call_ex(JmpRbx, __safecall(GetModuleHandleA).get(), ModuleImage); }
	__inline FARPROC GetModuleProcAddr(HMODULE hModule, LPCSTR hProcAddr) { return spoof_call_ex(JmpRbx, __safecall(GetProcAddress).get(), hModule, hProcAddr); }
	__inline BOOL ExitHandle(HANDLE Handle) { return spoof_call_ex(JmpRbx, __safecall(CloseHandle).get(), Handle); }
	__inline LPVOID GetMemCpy(void* inst, const void* ptr, size_t size) { return spoof_call_ex(JmpRbx, __safecall(memcpy).get(), inst, ptr, size); }
	__inline LPVOID GetMemSet(void* inst, int value, size_t size) { return spoof_call_ex(JmpRbx, memset, inst, value, size); }
	__inline LPVOID GetVirtualAlloc(LPVOID Inst, SIZE_T Size, DWORD AllocType, DWORD Protect) { return spoof_call_ex(JmpRbx, __safecall(VirtualAlloc).get(), Inst, Size, AllocType, Protect); }
	__inline BOOL GetVirtualProtect(LPVOID Inst, SIZE_T Size, DWORD oldProtect, PDWORD newProtect) { return spoof_call_ex(JmpRbx, __safecall(VirtualProtect).get(), Inst, Size, oldProtect, newProtect); }
	__inline HANDLE GetCurProc() { return spoof_call_ex(JmpRbx, __safecall(GetCurrentProcess).get()); }
	__inline BOOL GetKeyPress(int Code) { return spoof_call_ex(JmpRbx, __safecall(GetAsyncKeyState).get(), Code); }
	__inline VOID FreeLibAndExitThread(HMODULE hModule, DWORD hExitCode) { return spoof_call_ex(JmpRbx, __safecall(FreeLibraryAndExitThread).get(), hModule, hExitCode); }
	__inline BOOL DisableThreads(HMODULE hModule) { return spoof_call_ex(JmpRbx, __safecall(DisableThreadLibraryCalls).get(), hModule); }
	__inline LONG_PTR SetWndLongPtrA(HWND hWnd, int value, LONG_PTR ptr) { return spoof_call_ex(JmpRbx, SetWindowLongPtrA, hWnd, value, ptr); }
public:
	int GetMsgBox(HWND hWnd, LPCSTR Text, LPCSTR Title, UINT uInt) { return spoof_call_ex(JmpRbx, __safecall(MessageBoxA).get(), hWnd, Text, Title, uInt); }
}; WinApi* pWinApi = new WinApi();

class WinApiEx : public WinApi
{
private:
	HMODULE hModule = 0;
	LPCSTR User32dll = __("user32.dll");
	LPCSTR Kernel32dll = __("kernel32.dll");
public:
	typedef HMODULE(__stdcall* _RtlGetModuleHandle)(LPCSTR);
	typedef LPVOID(__stdcall* _RtlVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
	typedef BOOL(__stdcall* _RtlVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
	typedef HANDLE(__stdcall* _RtlGetCurrentProcess)();
	typedef BOOL(__stdcall* _RtlDisableThreads)(HMODULE);
	typedef BOOL(__stdcall* _RtlCloseHandle)(HANDLE);
	typedef LRESULT(__stdcall* _RtlCallWindowProc)(WNDPROC, HWND, UINT, WPARAM, LPARAM);
	typedef DWORD(__stdcall* _RtlGetProcId)(HANDLE);
	typedef DWORD(__stdcall* _RtlGetCurrentProcId)();
	typedef HWND(__stdcall* _RtlCreateWindow)(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
public:
	HMODULE __stdcall MyRtlGetModuleHandle(LPCSTR ModuleImage)
	{
		this->hModule = this->GetModuleName("kernel32.dll");
		if (!this->hModule)
			return NULL;
		_RtlGetModuleHandle RtlGetModuleHandle = (_RtlGetModuleHandle)
			this->GetModuleProcAddr(this->hModule, "GetModuleHandleA");
		return RtlGetModuleHandle(ModuleImage);
	}
	LPVOID __stdcall MyRtlVirtualAlloc(LPVOID Inst, SIZE_T Size, DWORD AllocType, DWORD Protect)
	{
		auto hModule = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule) return nullptr;
		_RtlVirtualAlloc RtlVirtualAlloc = (_RtlVirtualAlloc)pWinApi->GetModuleProcAddr(hModule, skCrypt("VirtualAlloc").decrypt());
		return RtlVirtualAlloc(Inst, Size, AllocType, Protect);
	}
	BOOL __stdcall MyRtlVirtualProtect(LPVOID Inst, SIZE_T Size, DWORD OldProtect, PDWORD NewProtect)
	{
		auto hModule = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule) return FALSE;
		_RtlVirtualProtect RtlVirtualProtect = (_RtlVirtualProtect)pWinApi->GetModuleProcAddr(hModule, skCrypt("VirtualProtect").decrypt());
		return RtlVirtualProtect(Inst, Size, OldProtect, NewProtect);
	}
	HANDLE __stdcall MyRtlGetCurrentProcess()
	{
		auto hModule = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule) return FALSE;
		_RtlGetCurrentProcess CurrentProcess = (_RtlGetCurrentProcess)pWinApi->GetModuleProcAddr(hModule, skCrypt("GetCurrentProcess").decrypt());
		return CurrentProcess();
	}
	BOOL __stdcall MyRtlDisableThreads(HMODULE hModule)
	{
		auto hModuleEx = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule) return FALSE;
		_RtlDisableThreads RtlDisableThreads = (_RtlDisableThreads)pWinApi->GetModuleProcAddr(hModuleEx, skCrypt("DisableThreadLibraryCalls").decrypt());
		return RtlDisableThreads(hModule);
	}
	BOOL __stdcall MyRtlCloseHandle(HANDLE Handle)
	{
		auto hModuleEx = pWinApi->GetModuleName("kernel32.dll");
		if (!hModuleEx) return FALSE;
		_RtlCloseHandle RtlCloseHandle = (_RtlCloseHandle)pWinApi->GetModuleProcAddr(hModuleEx, skCrypt("CloseHandle").decrypt());
		return RtlCloseHandle(Handle);
	}
	LRESULT __stdcall MyRtlCallWindowProc(WNDPROC WndProc, HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam)
	{
		auto hModule = pWinApi->GetModuleName("user32.dll");
		if (!hModule)
			return 0;
		_RtlCallWindowProc CallWndProc = (_RtlCallWindowProc)pWinApi->GetModuleProcAddr(hModule, skCrypt("CallWindowProcW").decrypt());
		return CallWndProc(WndProc, hWnd, uInt, wParam, lParam);
	}
	DWORD __stdcall MyRtlGetProcId(HANDLE Handle)
	{
		auto hModule = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule)
			return -1;
		_RtlGetProcId GetProcId = (_RtlGetProcId)pWinApi->GetModuleProcAddr(hModule, skCrypt("GetProcessId").decrypt());
		return GetProcId(Handle);
	}
	DWORD __stdcall MyRtlGetCurrProcId()
	{
		auto hModule = pWinApi->GetModuleName("kernel32.dll");
		if (!hModule)
			return -1;
		_RtlGetCurrentProcId GetCurProcId = (_RtlGetCurrentProcId)pWinApi->GetModuleProcAddr(hModule, skCrypt("GetCurrentProcessId").decrypt());
		return GetCurProcId();
	}
	HWND __stdcall MyRtlCreateWindow(DWORD Exit, LPCWSTR Class, LPCWSTR Name, DWORD Style, int x, int y, int w, int h, HWND hWindow, HMENU hMenu, HINSTANCE hInst, LPVOID Param)
	{
		auto hModule = this->GetModuleName("user32.dll");
		if (!hModule)
			return NULL;
		_RtlCreateWindow RtlCreateWindow = (_RtlCreateWindow)
			this->GetModuleProcAddr(hModule, skCrypt("CreateWindowExW").decrypt());
		return RtlCreateWindow(Exit, Class, Name, Style, x, y, w, h, hWindow, hMenu, hInst, Param);
	}
}; WinApiEx* pWinApiEx = new WinApiEx();

class ModuleApi
{
public:
	__inline DWORD64 GetModuleSize(DWORD64 ModuleImage)
	{
		IMAGE_DOS_HEADER dosHeader = *(IMAGE_DOS_HEADER*)ModuleImage;
		IMAGE_NT_HEADERS ntHeaders = *(IMAGE_NT_HEADERS*)(ModuleImage + dosHeader.e_lfanew);
		return (DWORD64)ntHeaders.OptionalHeader.SizeOfImage;
	}
	__inline PBYTE GetModuleBytes(HMODULE hModule, DWORD64 hModuleSize, DWORD64 hHexCode)
	{
		return (PBYTE)hModule + hModuleSize - hHexCode;
	}
}; ModuleApi* pModuleApi = new ModuleApi();
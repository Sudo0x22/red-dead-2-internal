#include"entry/based.hpp"
bool __stdcall GetGlobals(HMODULE hModule)
{
	global::g_hInst = hModule;
	global::g_hGameImage = (DWORD64)pWinApi->GetModuleName(NULL);
	global::g_dwModuleSize = pModuleApi->GetModuleSize((DWORD64)hModule);
	return true;
}
void __stdcall MainThread(HMODULE hModule)
{
	static bool m_bInitialized = false;
	while (!m_bInitialized)
	{
		gUtils_Offsets.Initialize();
		g_Features.Initialize();
		m_bInitialized = true;
	}
}
bool __stdcall DllMain(HMODULE hModule, DWORD hReasons, LPVOID hBuffer)
{
	static bool m_bAttached = false;
	if ((hReasons == DLL_PROCESS_ATTACH) && !m_bAttached)
	{
		pWinApiEx->MyRtlDisableThreads(hModule);

		GetGlobals(hModule);
		FuckAntiCheat();

		HANDLE hThread = pThread->MakeThread((LPTHREAD_START_ROUTINE)MainThread, hModule, 0);
		HANDLE hOverlayThread = pThread->MakeThread((LPTHREAD_START_ROUTINE)threads::InitPresentHook, 0, 0);
		if (hThread && hOverlayThread) { m_bAttached = true; pWinApiEx->MyRtlCloseHandle(hThread); pWinApiEx->MyRtlCloseHandle(hOverlayThread); }
	}

	return true;
}
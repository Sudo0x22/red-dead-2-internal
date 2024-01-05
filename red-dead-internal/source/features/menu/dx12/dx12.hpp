#pragma once
#include"../defines/defines.hpp"

WNDCLASSEX WindowClass;
HWND WindowHwnd;

extern "C" {namespace Window {
	BOOL _DeleteWindow(_In_ HWND hWindow) {
		return spoof_call_ex(JmpRbx, __safecall(DestroyWindow).get(), hWindow);
	}
	ATOM WINAPI RegisterWndClas(_In_ const WNDCLASSEX* WndClass) {
		return spoof_call_ex(JmpRbx, RegisterClassEx, WndClass);
	}
	ATOM WINAPI UnregisterWndClass(_In_ LPCWSTR Class, _In_ HINSTANCE hInst) {
		return spoof_call_ex(JmpRbx, UnregisterClass, Class, hInst);
	}
	LRESULT WINAPI CallWndProc(_In_ WNDPROC WndProc, _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		return spoof_call_ex(JmpRbx, CallWindowProcW, WndProc, hWnd, uMsg, wParam, lParam);
	}
	LRESULT WINAPI DefWndProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_  WPARAM wParam, _In_ LPARAM lParam) {
		return spoof_call_ex(JmpRbx, DefWindowProc, hWnd, uMsg, wParam, lParam);
	}
	HWND WINAPI NtCreateWindow(DWORD Exit, LPCWSTR Class, LPCWSTR Name, DWORD Style, int x, int y, int w, int h,
		HWND hWindow, HMENU hMenu, HINSTANCE hInst, LPVOID Param) {
		return spoof_call_ex(JmpRbx, CreateWindowExW, Exit, Class, Name, Style, x, y, w, h, hWindow, hMenu, hInst, Param);
	}
}}

namespace DX12
{
	__inline bool InitWindow()
	{
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = pWinApi->GetModuleName(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = L"DX12";
		WindowClass.hIconSm = NULL;
		Window::RegisterWndClas(&WindowClass);
		WindowHwnd = pWinApiEx->MyRtlCreateWindow(NULL, WindowClass.lpszClassName, L"DirectX12 Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL,
			NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) { return false; }
		return true;

	}
	__inline bool DeleteWindow()
	{
		Window::_DeleteWindow(WindowHwnd);
		Window::UnregisterWndClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) { return false; }
		return true;
	}
	__inline bool Initialize()
	{
		if (InitWindow() == false) { return false; }

		HMODULE D3D12Module = pWinApi->GetModuleName(skCrypt("d3d12.dll").decrypt());
		HMODULE DXGIModule = pWinApi->GetModuleName(skCrypt("dxgi.dll").decrypt());
		if (D3D12Module == NULL || DXGIModule == NULL) {
			DeleteWindow();
			return false;
		}

		void* CreateDXGIFactory = pWinApi->GetModuleProcAddr(DXGIModule, skCrypt("CreateDXGIFactory").decrypt());
		if (CreateDXGIFactory == NULL) {
			DeleteWindow();
			return false;
		}

		IDXGIFactory* Factory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0) {
			DeleteWindow();
			return false;
		}

		IDXGIAdapter* Adapter;
		if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND) {
			DeleteWindow();
			return false;
		}

		void* D3D12CreateDevice =pWinApi->GetModuleProcAddr(D3D12Module, skCrypt("D3D12CreateDevice").decrypt());
		if (D3D12CreateDevice == NULL) {
			DeleteWindow();
			return false;
		}

		ID3D12Device* Device;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0) {
			DeleteWindow();
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12CommandAllocator* CommandAllocator;
		if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12GraphicsCommandList* CommandList;
		if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0) {
			DeleteWindow();
			return false;
		}

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0) {
			DeleteWindow();
			return false;
		}

		Methods = (uint64_t*)::calloc(150, sizeof(uint64_t));
		pWinApi->GetMemCpy(Methods, *(uint64_t**)Device, 44 * sizeof(uint64_t));
		pWinApi->GetMemCpy(Methods + 44, *(uint64_t**)CommandQueue, 19 * sizeof(uint64_t));
		pWinApi->GetMemCpy(Methods + 44 + 19, *(uint64_t**)CommandAllocator, 9 * sizeof(uint64_t));
		pWinApi->GetMemCpy(Methods + 44 + 19 + 9, *(uint64_t**)CommandList, 60 * sizeof(uint64_t));
		pWinApi->GetMemCpy(Methods + 44 + 19 + 9 + 60, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));

		MH_Initialize();

		Device->Release();
		Device = NULL;

		CommandQueue->Release();
		CommandQueue = NULL;

		CommandAllocator->Release();
		CommandAllocator = NULL;

		CommandList->Release();
		CommandList = NULL;

		SwapChain->Release();
		SwapChain = NULL;

		DeleteWindow();
		return true;
	}
}
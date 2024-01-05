#pragma once
#include"../../../hook/hook.hpp"
#include<d3d12.h>
#include<dxgi1_4.h>
#include<d3d11.h>
#include<dxgi.h>
#include"../../../../includes/imgui/imgui.h"
#include"../../../../includes/imgui/imgui_impl_dx12.h"
#include"../../../../includes/imgui/imgui_impl_dx11.h"
#include"../../../../includes/imgui/imgui_impl_win32.h"

typedef LRESULT(APIENTRY* oWndProc_t)(HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(APIENTRY* oPresent_t)(IDXGISwapChain*, UINT, UINT);
typedef BOOL(APIENTRY* oCursor_t)(bool);
typedef void(APIENTRY* oExecuteCommandLists_t)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

class D3D11
{
public:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
public:
	WNDCLASSEX WindowClass;
	HWND WindowHandle = 0;
	HWND hWindow = 0;
}; D3D11* pD3D11 = new D3D11();

namespace originals
{
	oPresent_t oPresent = 0;
	oWndProc_t oWndProc = 0;
	oExecuteCommandLists_t oCmdList = 0;
	oCursor_t oCursor = 0;
}

namespace D3D12
{
	ID3D12Device* Device = nullptr;
	ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
	ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
	ID3D12GraphicsCommandList* CommandList;
	ID3D12CommandQueue* CommandQueue;

	struct _FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12Resource* Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	};

	uint64_t BuffersCounts = -1;
	_FrameContext* FrameContext;

}

namespace Handles
{
	HWND hWindow = 0;
	DWORD pId = 0;
	HANDLE Handle = 0;
}
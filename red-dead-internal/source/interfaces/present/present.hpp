#pragma once
#include"../cursor/cursor.hpp"

oPresent_t oPresent = 0;
oExecuteCommandLists_t oCommand = 0;
LONG_PTR SetWndProc(HWND hWnd, int value, LONG_PTR detour)
{
	return spoof_call_ex(JmpRbx, SetWindowLongPtrW, hWnd, value, detour);
}

extern "C" {namespace present {

	VOID WINAPI CommandList(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
		if (!D3D12::CommandQueue)
			D3D12::CommandQueue = queue;
		oCommand(queue, NumCommandLists, ppCommandLists);
	}

	HRESULT APIENTRY hook_present(IDXGISwapChain3* pSwapChain, UINT Sync, UINT Flags) {
		
		if (!pMenu.m_bInitImGui)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&D3D12::Device)))
			{
				ImGui::CreateContext();

				DXGI_SWAP_CHAIN_DESC Desc;
				pSwapChain->GetDesc(&Desc);
				Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				Desc.OutputWindow = Handles::hWindow;
				Desc.Windowed = ((GetWindowLongPtr(Handles::hWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

				D3D12::BuffersCounts = Desc.BufferCount;
				D3D12::FrameContext = new D3D12::_FrameContext[D3D12::BuffersCounts];

				D3D12_DESCRIPTOR_HEAP_DESC ImGuiRender = {};
				ImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				ImGuiRender.NumDescriptors = D3D12::BuffersCounts;
				ImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				if (D3D12::Device->CreateDescriptorHeap(&ImGuiRender, IID_PPV_ARGS(&D3D12::DescriptorHeapImGuiRender)) != S_OK)
					return oPresent(pSwapChain, Sync, Flags);

				ID3D12CommandAllocator* Allocator;
				if (D3D12::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
					return oPresent(pSwapChain, Sync, Flags);

				for (size_t i = 0; i < D3D12::BuffersCounts; i++) {
					D3D12::FrameContext[i].CommandAllocator = Allocator;
				}

				if (D3D12::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&D3D12::CommandList)) != S_OK ||
					D3D12::CommandList->Close() != S_OK)
					return oPresent(pSwapChain, Sync, Flags);

				D3D12_DESCRIPTOR_HEAP_DESC BackBuffers;
				BackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				BackBuffers.NumDescriptors = D3D12::BuffersCounts;
				BackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				BackBuffers.NodeMask = 1;

				if (D3D12::Device->CreateDescriptorHeap(&BackBuffers, IID_PPV_ARGS(&D3D12::DescriptorHeapBackBuffers)) != S_OK)
					return oPresent(pSwapChain, Sync, Flags);

				const auto Size = D3D12::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE Handle = D3D12::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

				for (size_t i = 0; i < D3D12::BuffersCounts; i++)
				{
					ID3D12Resource* pBackBuffer = nullptr;
					D3D12::FrameContext[i].DescriptorHandle = Handle;
					pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
					D3D12::Device->CreateRenderTargetView(pBackBuffer, nullptr, Handle);
					D3D12::FrameContext[i].Resource = pBackBuffer;
					Handle.ptr += Size;
				}

				ImGui_ImplWin32_Init(Handles::hWindow);

				ImGui_ImplDX12_Init(D3D12::Device, D3D12::BuffersCounts, DXGI_FORMAT_B8G8R8X8_UNORM, D3D12::DescriptorHeapImGuiRender, D3D12::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
					D3D12::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
				ImGui_ImplDX12_CreateDeviceObjects();

				ImGui::GetIO().ImeWindowHandle = Handles::hWindow;
				oWndProc = (WNDPROC)SetWndProc(Handles::hWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc::Detour);
				pMenu.InitStyle();			
				pMenu.m_bInitImGui = true;
			}
			else
				oPresent(pSwapChain, Sync, Flags);
		}

		if (D3D12::CommandQueue == nullptr)
			return oPresent(pSwapChain, Sync, Flags);

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::GetIO().MouseDrawCursor = pMenu.m_bMenuToggled;
		ImGui::GetIO().WantCaptureMouse = pMenu.m_bMenuToggled;

		pMenu.m_bCaptureCursor = ImGui::GetIO().WantCaptureMouse;
		pMenu.m_bRenderCursor = ImGui::GetIO().MouseDrawCursor;

		static time_t curTime = time(nullptr);
		static tm* curCurTime = localtime(&curTime);

		if (MyRtlKeyPress(VK_INSERT) & 0x1) { pMenu.m_bMenuToggled = !pMenu.m_bMenuToggled; }
		if (pMenu.m_bMenuToggled)
		{
			pMenu.Render("settings", 450, 450);

			if (curCurTime->tm_mon == 11 || curCurTime->tm_mon == 0 || curCurTime->tm_mon == 1)
			{
				Snow pSnow; pSnow.RenderSnow();
			}
		}

		if (pMenu.m_bCaptureCursor && pMenu.m_bRenderCursor)
		{
			cursor::CallHook(&SetCursorPos, cursor::CursorHook,
				reinterpret_cast<LPVOID*>(&originals::oCursor));
		}
		else
		{
			cursor::KillHook(&SetCursorPos);
		}

		if (misc::m_bEnableFpsCounter.m_Var == true)
		{
			pFPSWindow.Render(108, 24);
		}

		D3D12::_FrameContext& CurrentFrameContext = D3D12::FrameContext[pSwapChain->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER Barrier;
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = CurrentFrameContext.Resource;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		D3D12::CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
		D3D12::CommandList->ResourceBarrier(1, &Barrier);
		D3D12::CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
		D3D12::CommandList->SetDescriptorHeaps(1, &D3D12::DescriptorHeapImGuiRender);

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), D3D12::CommandList);
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		D3D12::CommandList->ResourceBarrier(1, &Barrier);
		D3D12::CommandList->Close();
		D3D12::CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&D3D12::CommandList));
		return oPresent(pSwapChain, Sync, Flags); 
	}
}}
#pragma once
#include"cursor.hpp"
extern "C" {namespace modules {
	DWORD WINAPI NtWindowThreadId(HWND hWindow, LPDWORD Pointer) {
		return spoof_call_ex(JmpRbx,__safecall(GetWindowThreadProcessId).get(), hWindow, Pointer);
	}
	HWND WINAPI NtForgroundWindow() {
		return spoof_call_ex(JmpRbx, __safecall(GetForegroundWindow).get());
	}
	DWORD WINAPI NtCurrentProcessID() {
		return spoof_call_ex(JmpRbx, __safecall(GetCurrentProcessId).get());
	}
}}

namespace HookDetours
{
	namespace present
	{
		namespace dx12
		{
			namespace cmd_list 
			{
				constexpr uint16_t nIndex = 52;
				VOID __fastcall Detour(ID3D12CommandQueue* pQueue, UINT uInt, ID3D12CommandList* pCommandList)
				{
					if (!D3D12::CommandQueue)
						D3D12::CommandQueue = pQueue;
					return originals::oCmdList(pQueue, uInt, pCommandList);
				}
			}
			constexpr uint16_t nIndex = 140;
			HRESULT __fastcall Detour(IDXGISwapChain3* pSwapChain, UINT Sync, UINT Flags)
			{
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
							return originals::oPresent(pSwapChain, Sync, Flags);

						ID3D12CommandAllocator* Allocator;
						if (D3D12::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
							return originals::oPresent(pSwapChain, Sync, Flags);

						for (size_t i = 0; i < D3D12::BuffersCounts; i++) {
							D3D12::FrameContext[i].CommandAllocator = Allocator;
						}

						if (D3D12::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&D3D12::CommandList)) != S_OK ||
							D3D12::CommandList->Close() != S_OK)
							return originals::oPresent(pSwapChain, Sync, Flags);

						D3D12_DESCRIPTOR_HEAP_DESC BackBuffers;
						BackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
						BackBuffers.NumDescriptors = D3D12::BuffersCounts;
						BackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
						BackBuffers.NodeMask = 1;

						if (D3D12::Device->CreateDescriptorHeap(&BackBuffers, IID_PPV_ARGS(&D3D12::DescriptorHeapBackBuffers)) != S_OK)
							return originals::oPresent(pSwapChain, Sync, Flags);

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
						originals::oWndProc = (WNDPROC)SetWindowLongPtr(Handles::hWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc::Detour);
						pMenu.m_bInitImGui = true;

						printf("got here\n");
					}
					else {
						printf("failed\n");
						originals::oPresent(pSwapChain, Sync, Flags);
					}
				}

				if (D3D12::CommandQueue == nullptr)
					return originals::oPresent(pSwapChain, Sync, Flags);

				ImGui_ImplDX12_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				if (MyRtlKeyPress(VK_INSERT) & 0x1) { printf("key pressed\n"); }
			
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
				return originals::oPresent(pSwapChain, Sync, Flags);
			}
		}

		namespace dx11
		{
			namespace resize_buffer 
			{
				constexpr uint16_t nIndex = 13;
				HRESULT __fastcall Detour(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
				{
					ImGui_ImplDX11_Shutdown();
					ImGui_ImplWin32_Shutdown();
					pD3D11->pRenderTargetView->Release();
					pD3D11->pDeviceContext->Release();
					pD3D11->pDevice->Release();
					pD3D11->pDevice = nullptr;
					return originals::oResizeBuffer(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
				}

			}

			constexpr uint16_t nIndex = 8;
			HRESULT __fastcall Detour(IDXGISwapChain* pSwapChain, UINT uInt, UINT uFlags)
			{
				if (!pMenu.m_bInitImGui)
				{
					if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&pD3D11->pDevice)))
					{
						ImGui::CreateContext();

						pD3D11->pDevice->GetImmediateContext(&pD3D11->pDeviceContext);
						DXGI_SWAP_CHAIN_DESC desc;

						pSwapChain->GetDesc(&desc);
						pD3D11->hWindow = desc.OutputWindow;

						ID3D11Texture2D* pBackBuffer;
						pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

						D3D11_RENDER_TARGET_VIEW_DESC RenderTarget = {};
						pWinApi->GetMemSet(&RenderTarget, 0, sizeof(RenderTarget));

						RenderTarget.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						RenderTarget.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

						pD3D11->pDevice->CreateRenderTargetView(pBackBuffer, &RenderTarget, &pD3D11->pRenderTargetView);
						pBackBuffer->Release();

						ImGui::GetIO().ImeWindowHandle = pD3D11->hWindow;
						ImGui_ImplDX11_CreateDeviceObjects();

						originals::oWndProc = (oWndProcFn)pWinApi->SetWndLongPtrA(pD3D11->hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc::Detour);
						pMenu.Initialize();
						pMenu.InitStyle();
						pMenu.InitFonts();
						pMenu.m_bInitImGui = true;
					}
					else
						return originals::oPresent(pSwapChain, uInt, uFlags);
				}

				ImGui::GetIO().MouseDrawCursor = pMenu.m_bMenuToggled;
				ImGui::GetIO().WantCaptureMouse = pMenu.m_bMenuToggled;

				pMenu.m_bCaptureCursor = ImGui::GetIO().WantCaptureMouse;
				pMenu.m_bRenderCursor = ImGui::GetIO().MouseDrawCursor;

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				static time_t curTime = time(nullptr);
				static tm* curCurTime = localtime(&curTime);

				if (MyRtlKeyPress(VK_INSERT) & 0x1) { pMenu.m_bMenuToggled = !pMenu.m_bMenuToggled; }
				if (pMenu.m_bMenuToggled)
				{
					pMenu.Render("settings", 450,450, 0,0);
					
					if (curCurTime->tm_mon == 11 || curCurTime->tm_mon == 0 || curCurTime->tm_mon==1) 
					{
						Snow pSnow; pSnow.RenderSnow();
					}
				}

				if (pMenu.m_bCaptureCursor && pMenu.m_bRenderCursor) 
				{
					HookDetours::cursor::CallHook(&SetCursorPos, HookDetours::cursor::CursorHook,
						reinterpret_cast<LPVOID*>(&originals::oCursor));
				}
				else 
				{
					HookDetours::cursor::KillHook(&SetCursorPos);
				}

				if (misc::m_bEnableFpsCounter.m_Var == true)
				{
					pFPSWindow.Render(108, 24);
				}

				ImGui::EndFrame();
				ImGui::Render();
				pD3D11->pDeviceContext->OMSetRenderTargets(1, &pD3D11->pRenderTargetView, NULL);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				return originals::oPresent(pSwapChain, uInt, uFlags);
			}
		}

		void Initialize()
		{
			pHook->MakeHook(dx11::nIndex, dx11::Detour, (LPVOID*)&originals::oPresent);
			pHook->MakeHook(dx11::nIndex, dx11::resize_buffer::Detour, (LPVOID*)&originals::oResizeBuffer);
		}

		void InitializeDX12()
		{
			pHook->MakeHook(dx12::cmd_list::nIndex, dx12::cmd_list::Detour, (LPVOID*)&originals::oCmdList);
			pHook->MakeHook(dx12::nIndex, dx12::Detour, (LPVOID*)&originals::oPresent);			
		}
	}
}
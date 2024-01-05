#pragma once
#include"dx11/dx11.hpp"
#include"../vars/vars.hpp"
#include"../../sdk/render.hpp"
typedef __int64(__stdcall* _RtlKeyPress)(int);
__int64 __stdcall MyRtlKeyPress(int code)
{
	auto hModule = pWinApi->GetModuleName(skCrypt("user32.dll").decrypt());
	if (!hModule)
		return 0;
	_RtlKeyPress KeyPress = (_RtlKeyPress)pWinApi->GetModuleProcAddr(hModule, skCrypt("GetAsyncKeyState").decrypt());
	return KeyPress(code);
}
ImVec4 Hex(int hexValue, float alpha)
{
	float r = (hexValue >> 16) & 0xFF;
	float g = (hexValue >> 8) & 0xFF;
	float b = hexValue & 0xFF;
	return ImVec4(r / 255, g / 255, b / 255, alpha);
}
class Snow
{
public:
	__inline void RenderSnow()
	{
		struct SnowFlakeT { int x; int y; };
		static std::vector<SnowFlakeT> vSnowFlakes;
		constexpr int SnowCount = 150;

		static bool bInit = false;
		while (!bInit)
		{
			for (int index = 0; index < SnowCount; index++)
			{
				vSnowFlakes.emplace_back(utils::RandIntSimple(0, ImGui::GetIO().DisplaySize.x), utils::RandIntSimple(0, ImGui::GetIO().DisplaySize.y / 2.f));
			}
			bInit = true;
		}

		for (auto& flake : vSnowFlakes)
		{
			constexpr int drift = 1;
			flake.x = utils::RandIntSimple(-drift, drift);
			flake.y += drift;

			const float alpha = utils::MapFloat(flake.y, 0.0f, ImGui::GetIO().DisplaySize.y / 2.f, 0.5f, 0.0f);
			if (alpha <= 0.f || flake.x >= ImGui::GetIO().DisplaySize.x || flake.x <= 0)
			{
				flake.x = utils::RandIntSimple(0, ImGui::GetIO().DisplaySize.x);
				flake.y = utils::RandIntSimple(0, 150);
			}
			I::pSDK_Render->render_text(flake.x, flake.y, &color.White, "*");
		}
	}
};
class Tabs
{
public:
	__inline void PlayerTab()
	{
		ImGui::Checkbox(player::m_bGodMode.m_sDisplayName, &player::m_bGodMode.m_Var);
		ImGui::Checkbox(player::m_bNoWanted.m_sDisplayName, &player::m_bNoWanted.m_Var);

	}
	__inline void HorseTab() { }
	__inline void RecoveryTab()
	{
		ImGui::Checkbox(recovery::m_bGiveOnlineMoney.m_sDisplayName, &recovery::m_bGiveOnlineMoney.m_Var);
		ImGui::Checkbox(recovery::m_bGiveOnlineRank.m_sDisplayName, &recovery::m_bGiveOnlineRank.m_Var);
	}
	__inline void StoryModeRecovery()
	{
		ImGui::Checkbox(recovery::m_bGiveStoryModeMoney.m_sDisplayName, &recovery::m_bGiveStoryModeMoney.m_Var);
	}
	__inline void VisualTab() {}
	__inline void MiscTab()
	{
		ImGui::Checkbox(misc::m_bEnableFpsCounter.m_sDisplayName, &misc::m_bEnableFpsCounter.m_Var);
	}
	
}; Tabs* pTabs = new Tabs();
class FPSWindow
{
public:
	__inline void Render(float w, float h)
	{
		ImGui::Begin(skCrypt("FPS").decrypt(), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize(ImVec2(w, h));
		ImGui::Text(skCrypt("FPS: %f"), ImGui::GetIO().Framerate);
		ImGui::End();
	}
}; inline FPSWindow pFPSWindow;
class Menu
{
private:
	std::string menu_title = skCrypt("settings").decrypt();
	bool close_button = false;
	unsigned long window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
public:
	bool m_bMenuToggled = false;
	bool m_bInitImGui = false;
	bool m_bCaptureCursor = false;
	bool m_bRenderCursor = false;
public:
	ImFont* TitleFont = nullptr;
	ImFont* VerdanaSmall = nullptr;
	ImFont* IconFont = nullptr;
public:
	__inline void Initialize()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(pD3D11->hWindow);
		ImGui_ImplDX11_Init(pD3D11->pDevice, pD3D11->pDeviceContext);
	}
	__inline void InitStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();

		style.WindowRounding = 4;
		style.ChildRounding = 4;
		style.FrameRounding = 3;
		style.PopupRounding = 3;
		style.GrabRounding = 3;
		style.TabRounding = 3;
		style.ScrollbarRounding = 3;
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.FramePadding = { 6.0f, 6.0f };
		style.ItemSpacing = { 9.0f, 9.0f };
		style.WindowPadding = { 9.0f, 9.0f };
		style.ItemInnerSpacing = { 8.0f, 4.0f };
		style.WindowBorderSize = 1;
		style.FrameBorderSize = 1;
		style.ScrollbarSize = 12.f;
		style.GrabMinSize = 8.f;
		//style.WindowShadowSize = 0.f;

		//style.Colors[ImGuiCol_WindowShadow] = Hex(0xDB4141, 1.0f); // 0x5E61BA

		style.Colors[ImGuiCol_WindowBg] = Hex(0x111111, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = Hex(0x151515, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = Hex(0x191919, 1.0f);

		style.Colors[ImGuiCol_TitleBg] = Hex(0x111111, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = Hex(0x111111, 1.0f);

		style.Colors[ImGuiCol_Border] = Hex(0x9b58ff, 1.0f);
		style.Colors[ImGuiCol_Separator] = Hex(0x1F1F1F, 1.0f);

		style.Colors[ImGuiCol_SliderGrab] = Hex(0x9b58ff, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = Hex(0x9b58ff, 0.8f);

		style.Colors[ImGuiCol_CheckMark] = Hex(0xE8E8E8, 1.0f);
		style.Colors[ImGuiCol_Text] = Hex(0xE8E8E8, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = Hex(0x616161, 1.0f);

		style.Colors[ImGuiCol_Header] = Hex(0x212121, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = Hex(0x444444, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = Hex(0x363636, 1.0f);

		style.Colors[ImGuiCol_FrameBg] = Hex(0x111111, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = Hex(0x111111, 0.9f);
		style.Colors[ImGuiCol_FrameBgActive] = Hex(0x111111, 0.8f);

		style.Colors[ImGuiCol_Button] = Hex(0x111111, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = Hex(0x9b58ff, 0.8f);
		style.Colors[ImGuiCol_ButtonActive] = Hex(0x9b58ff, 0.6f);

		style.Colors[ImGuiCol_Tab] = Hex(0x111111, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = Hex(0x9b58ff, 0.8f);
		style.Colors[ImGuiCol_TabActive] = Hex(0x9b58ff, 0.6f);

		style.Colors[ImGuiCol_ScrollbarBg] = ImColor(0, 0, 0, 0);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(0, 0, 0, 0);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(0, 0, 0, 0);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(0, 0, 0, 0);
	}
	__inline void InitFonts()
	{
		{
			const auto& io = ImGui::GetIO();
			auto FontConfig = ImFontConfig();
			FontConfig.OversampleH = 2;
			constexpr ImWchar FontRange[] = { 0x0020, 0x00FF,0x0400,0x044F,0 };
			this->VerdanaSmall = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\verdana.ttf").decrypt(), 12.0f, &FontConfig, FontRange);
			this->TitleFont = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\verdana.ttf").decrypt(), 20.0f, &FontConfig, FontRange);
			io.Fonts->Build();
		}
	}
	__inline void RenderMenuTabs()
	{
		//ImGui::PushFont(this->VerdanaSmall);
		ImGui::BeginTabBar(skCrypt("##features").decrypt());

		if (ImGui::BeginTabItem(skCrypt("Player").decrypt())) 
		{ 
			pTabs->PlayerTab();
			ImGui::EndTabItem(); 
		}

		if (ImGui::BeginTabItem(skCrypt("Horse").decrypt()))
		{
		
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(skCrypt("Recovery").decrypt()))
		{
			ImGui::BeginTabBar(skCrypt("##Recovery"));
			
			if (ImGui::BeginTabItem(skCrypt("Story Mode").decrypt()))
			{
				pTabs->StoryModeRecovery();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem(skCrypt("Online Mode").decrypt())) {
				pTabs->RecoveryTab();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(skCrypt("Visuals").decrypt()))
		{

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(skCrypt("Misc").decrypt()))
		{
			pTabs->MiscTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		//ImGui::PopFont();
	}
	__inline void Render(const char* title, float w, float h)
	{
		I::pSDK_Render->DrawBackground(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, color.background);
		if (ImGui::Begin(title, &this->close_button, this->window_flags)) 
		{
			ImGui::Separator();
			ImGui::SetWindowSize(ImVec2(w, h));
			this->RenderMenuTabs();
			ImGui::End();
		}
	}
};
inline Menu pMenu;
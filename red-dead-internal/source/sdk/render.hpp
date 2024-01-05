#pragma once
#include<Windows.h>
#include"../../includes/imgui/imgui.h"
#include <stdio.h>
#include <string>
#include "../utils/utils.hpp"

struct Color
{
	DWORD R, G, B, A;
};

class CSDK_Color
{
public:
	Color red = { 255,0,0,255 };
	Color Blue = { 0,0,255,255 };
	Color White = { 255,255,255,255 };
	Color black = { 0,0,0,255 };
public:
	ImColor background = { 15,15,15,150 };
}; CSDK_Color color;

class CSDK_Render
{
public:
	__inline void DrawBackground(float x, float y, int w, int h, ImColor Color) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2{ x, y }, ImVec2(w, h), Color, 0, 0);
	}
	void render_text(int x, int y, Color* color, const char* str, ...)
	{
		va_list(args);
		va_start(args, str);

		CHAR wbuffer[256] = { };
		vsprintf_s(wbuffer, str, args);

		va_end(args);

		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = utils::string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y),
			ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), wbuffer);
	}
};
namespace I { inline CSDK_Render* pSDK_Render = nullptr; }
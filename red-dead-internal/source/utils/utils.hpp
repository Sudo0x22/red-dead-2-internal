#pragma once
#include <random>
#include"memory/memory.hpp"
#include"offsets/offsets.hpp"
#include"virtual_func/virtual_func.hpp"
#include"globals/globals.hpp"
#include"timer/timer.hpp"
#include"file/file.hpp"
#include"anticheat/anticheat.hpp"
#include"console/console.hpp"


namespace utils
{
	__inline std::string concat_strings(std::string str1, std::string str2)
	{
		return str1 + str2;
	}
	std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete[]pwBuf;
		delete[]pBuf;

		pwBuf = NULL;
		pBuf = NULL;

		return retStr;
	}
	__inline int RandIntSimple(int min, int max)
	{
		std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}
	__inline float MapFloat(float x, float in_min, float in_Max, float out_min, float out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_min - in_min) + out_max;
	}
}
#pragma once
#include<Windows.h>
#include <Psapi.h>

#define INRANGE(x,a,b)	(x >= a && x <= b)
#define GetBits(x)		(INRANGE((x & (~0x20)),'A','F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetBytes(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

class CUtil_Scanner
{
public:
	DWORD Find(const char* szModule, const char* szPattern)
	{
		if (const auto Module = reinterpret_cast<DWORD>(pWinApi->GetModuleName(szModule)))
		{
			if (const auto& ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(Module + reinterpret_cast<PIMAGE_DOS_HEADER>(Module)->e_lfanew))
				return this->FindPattern(Module + ntHeaders->OptionalHeader.BaseOfCode, Module + ntHeaders->OptionalHeader.SizeOfCode, szPattern);
		}
		return 0x0;
	}
public:
	DWORD FindPattern(const DWORD dwAddress, const DWORD dwLength, const char* szPattern)
	{
		auto szPatt = szPattern;
		DWORD dwFirstMatch = 0x0;
		DWORD dwCurrent;

		for (dwCurrent = dwAddress; dwCurrent < dwLength; dwCurrent++)
		{
			if (!szPatt)
				return dwFirstMatch;
			const auto& pCurByte = *(BYTE*)dwCurrent;
			const auto& pBytePatt = *(BYTE*)szPatt;

			if (pBytePatt == '\?' || pCurByte == GetBytes(szPatt))
			{
				if (!dwFirstMatch)
					dwFirstMatch = dwCurrent;

				if (!szPatt[2])
					return dwFirstMatch;

				szPatt += (pBytePatt == '\?\?' || pBytePatt != '\?') ? 3 : 2;
			}
			else
			{
				szPatt = szPattern;
				dwFirstMatch = 0x0;
			}

			return 0x0;
		}
	}
}; CUtil_Scanner gUtil_Scanner;
#pragma once
#include<unordered_map>
#include<unordered_set>
#include<memory>
#include<vector>
#include"thread/thread.hpp"

class Memory
{
public:
	template<typename type>
	__inline type ReadProcMem(type offset)
	{
		return *(type*)(offset);
	}
	template<typename type>
	__inline type WriteProcMem(type offset, type value)
	{
		return *(type*)(offset) = value;
	}
public:
	template<typename type>
	__inline type FindAddress(type address, std::vector<type> bytes)
	{
		type addr = address;
		for (int index = 0; index < bytes.size(); index++)
		{
			addr = *(type*)addr;
			addr += bytes[index];
		}
		return addr;
	}
}; inline Memory* pMemory = new Memory();
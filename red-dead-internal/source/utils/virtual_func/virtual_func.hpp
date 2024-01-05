#pragma once
class CUtil_VirtualFunc
{
public:
	template<typename type>
	__inline type Get(void* inst, unsigned int index, const unsigned int offset = 0u)
	{
		return reinterpret_cast<type>(this->GetVTable(inst, offset)[index]);
	}
private:
	void**& GetVTable(void* inst, const unsigned int offset = 0u)
	{
		return *reinterpret_cast<void***>(reinterpret_cast<unsigned int>(inst) + offset);
	}
	const void** GetVTable(const void* inst, const unsigned int offset = 0u)
	{
		return *reinterpret_cast<const void***>(reinterpret_cast<unsigned int>(inst) + offset);
	}
}; inline CUtil_VirtualFunc gUtil_VirtualFunc;
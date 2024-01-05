#include<cstdint>
#include<cassert>
#include"../../includes/kiero/kiero.h"
#include"../utils/utils.hpp"

static uint64_t* Methods = nullptr;
class SpoofHook 
{
public:
	__inline MH_STATUS Initialize() { return spoof_call_ex(JmpRbx, MH_Initialize); }
	__inline MH_STATUS CreateHook(LPVOID nTarget, LPVOID nDetour, LPVOID* nOriginal) { return spoof_call_ex(JmpRbx, MH_CreateHook, nTarget, nDetour, nOriginal); }
	__inline MH_STATUS EnableHook(LPVOID nTarget) { return spoof_call_ex(JmpRbx, MH_EnableHook, nTarget); }
	__inline MH_STATUS DisableHook(LPVOID nTarget) { return spoof_call_ex(JmpRbx, MH_DisableHook, nTarget); }
	__inline MH_STATUS RemoveHook(LPVOID nTarget) { return spoof_call_ex(JmpRbx, MH_RemoveHook, nTarget); }
}; 
class Function : public SpoofHook
{
private:
	void* m_pOriginal = nullptr;
public:
	__inline bool Initialize(void* nTarget, void* pDetour)
	{
		return (this->CreateHook(nTarget, pDetour, &this->m_pOriginal) == MH_OK);
	}
	template<typename HookFn>
	__inline HookFn Original() const
	{
		return reinterpret_cast<HookFn>(this->m_pOriginal);
	}
};
class Table : public SpoofHook
{
private:
	unsigned int** m_pBase = 0u;
	unsigned int   m_pSize = 0u;
	std::unique_ptr<void* []> m_pOriginals = {};
public:
	__inline bool Initialize(const void* pTable) {
		this->m_pBase = (unsigned int**)(pTable);
		while (reinterpret_cast<unsigned int*>(*this->m_pBase)[this->m_pSize])
			this->m_pSize += 1u;
		this->m_pOriginals = std::make_unique<void* []>(this->m_pSize);
		return (this->m_pBase && this->m_pSize);
	}
	__inline bool Hook(void* pDetour, const unsigned int pIndex) {
		if (this->m_pBase && this->m_pSize)
			return (this->CreateHook((*reinterpret_cast<void***>(this->m_pBase))[pIndex], pDetour, &this->m_pOriginals[pIndex]) == MH_OK);
		return false;
	}
public:
	template<typename type>
	__inline type RetOriginal(const unsigned int pIndex) const {
		return reinterpret_cast<type>(this->m_pOriginals[pIndex]);
	}
};
class Hook : public SpoofHook
{
public:
	__inline bool MakeHook(uint16_t nIndex, void* nDetour, void** nOriginal)
	{
		assert(index <= 0 && detour != NULL && original != NULL);
		void* nTarget = (void*)Methods[nIndex];
		if (this->CreateHook(nTarget, nDetour, nOriginal) != MH_OK)
			return false;
		if (this->EnableHook(nTarget) != MH_OK)
			return false;
		return true;
	}
}; Hook* pHook = new Hook();
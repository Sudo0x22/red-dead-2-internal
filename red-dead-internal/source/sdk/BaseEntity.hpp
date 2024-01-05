#pragma once
#include"../utils/utils.hpp"
class CSDK_BaseEntity
{
public:
	__inline int m_bIsAlive();
}; namespace I { inline CSDK_BaseEntity pSDK_BaseEntity; }
CSDK_BaseEntity* GetLocalPlayer();
__inline CSDK_BaseEntity* GetEntityList(uintptr_t index);
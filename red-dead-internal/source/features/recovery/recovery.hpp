#pragma once
#include"../vars/vars.hpp"
class Recovery
{
private:
	__inline void GiveMoney()
	{
		if (!recovery::m_bGiveOnlineMoney.m_Var)
			return;
	}
	__inline void GiveRank()
	{
		if (!recovery::m_bGiveOnlineRank.m_Var)
			return;
	}
	__inline void StoryModeMoney()
	{
		if (!recovery::m_bGiveStoryModeMoney.m_Var)
			return;
	}
public:
	__inline bool Run() { this->GiveRank(); this->GiveMoney(); this->StoryModeMoney(); return true; }
}; namespace F { inline Recovery* pRecovery = nullptr; }
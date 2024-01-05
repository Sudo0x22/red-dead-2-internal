#pragma once
#include"../vars/vars.hpp"
class Player
{
private:
	__inline void GodMode()
	{
		if (!player::m_bGodMode.m_Var)
			return;
		
	}
	__inline void NoWanted()
	{
		if (!player::m_bNoWanted.m_Var)
			return;
	}
public:
	__inline bool Run() 
	{
		this->GodMode();
		this->NoWanted();
		return true;
	}
}; namespace F { inline Player* pPlayer = nullptr; }
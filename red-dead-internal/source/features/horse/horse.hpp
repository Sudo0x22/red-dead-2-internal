#pragma once
#include"../vars/vars.hpp"
class Horse
{
private:
	__inline void GodMode();
	__inline void UnlimitedSpeed();
public:
	__inline bool Run() { return true; }
}; namespace F { inline Horse* pHorse = nullptr; }
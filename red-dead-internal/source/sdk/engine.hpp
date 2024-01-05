#pragma once
#include"BaseEntity.hpp"
struct Matrix { float maxtrix[16]; };
class Vector {};
class Vector2D {};

namespace engine
{
	__inline Matrix GetViewMatrix();
	bool WorldToScreen(Vector& In, Vector2D Out);
}
#pragma once
#include <Windows.h>

namespace shared
{
	inline DWORD base = (DWORD)GetModuleHandleA(NULL);

	inline unsigned int random(unsigned int min, unsigned int max) noexcept
	{
		return min + (rand() % (max - min + 1));
	}
}

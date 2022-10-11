#pragma once
#include <Windows.h>
#include <Psapi.h>

namespace cheat
{
	inline bool infiniteFc = false;
    inline bool infiniteHealth = false;
	inline bool oneHitKill = false;
	inline HANDLE GameProcess = GetCurrentProcess();
	inline DWORD base = 0;

	DWORD GetBaseAddress(const HANDLE hProcess) noexcept;
	void OneHitKillCave() noexcept;
	void HandleCheats() noexcept;
}
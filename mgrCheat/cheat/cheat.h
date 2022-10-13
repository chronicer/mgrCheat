#pragma once
#include <Windows.h>
#include <Psapi.h>

enum GameMenuStatus : unsigned short
{
	InMenu = 0,
	InGame,
	ProcessPause,
	PauseMenu,
	Codec,
	StateDisallowPause,
	CutscenePause,
	VRFail,
	MissionFail,
	SelectWeaponMenu,
	GraphicsSettings,
	AttackHelpScreen
};

namespace cheat
{
	inline bool infiniteFc = false;
    inline bool infiniteHealth = false;
	inline bool oneHitKill = false;
	inline bool infiniteSubWeapon = false;
	inline bool noDamageStat = false;
	inline bool stealth = false;
	inline float heightRate = 0.0f;
	inline bool heightChange = false;
	inline HANDLE GameProcess = GetCurrentProcess();

	DWORD GetBaseAddress(const HANDLE hProcess) noexcept;
	void OneHitKillCave() noexcept;
	void InfiniteRocketsCave() noexcept;
	void InfiniteGrenadeCave() noexcept;
	void HandleCheats() noexcept;
}
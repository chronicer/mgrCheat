#pragma once
#include <Windows.h>
#include <Psapi.h>

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
	inline bool groundCheat = false;
	inline bool groundEnabled = false;
	inline unsigned int groundCheatHotkey = 75;
	inline unsigned int temporaryVisorHotkey = 80;
	inline bool visorSwitch = false;
	inline bool autoHpUp = false;
	inline float ripperZangekiAnimationSpeedRate = 0.0f;
	inline float ripperZangekiGameSpeedRate = 0.0f;
	inline float zangekiAnimationSpeedRate = 0.0f;
	inline float zangekiGameSpeedRate = 0.0f;
	inline float ninjarunSpeedRate = 0.0f;
	inline HANDLE GameProcess = GetCurrentProcess();
	inline DWORD base = 0;
	inline bool OnFocus = false;

	DWORD GetBaseAddress(const HANDLE hProcess) noexcept;
	void HandleCheats() noexcept;
	void LoadConfig() noexcept;
	void SaveConfig() noexcept;
	void MugenZangekiCheat() noexcept;
	void InfiniteHealthCheat() noexcept;
	void OneHitKillCheat() noexcept;
	void InfiniteSubWeaponCheat() noexcept;
	void NoDamageStatCheat() noexcept;
	void StealthCheat() noexcept;
	void HeightChangeCheat() noexcept;
	void GroundCheat() noexcept;
	void TemporaryVisorCheat() noexcept;
	void AutoHPUpCheat() noexcept;
	void ZangekiTime() noexcept;
	void NinjaRunSpeedCheat() noexcept;
}
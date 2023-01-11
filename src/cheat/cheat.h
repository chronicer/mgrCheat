#pragma once
#include <Windows.h>
#include <Psapi.h>
#include "shared.h"

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
	inline float ninjaRunSpeedRate = 0.0f;
	inline bool timeStop = false;
	inline bool timeStopSwitch = false;
	inline unsigned int timeStopHotkey = 84;
	inline bool dealZeroDamage = false;
	inline bool &OnFocus = *(bool*)(shared::base + 0x19D509C);
	inline bool infTimer = false;
	inline int battlePoints = 0;

	void HandleCheats() noexcept;
	void LoadConfig() noexcept;
	void SaveConfig() noexcept;
	void Reset() noexcept;
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
	void NinjaRunSpeedCheat() noexcept;
	void TimeStop() noexcept;
	void Deal0Damage() noexcept;
	void InfVRTimer() noexcept;
	void BattlePointsChange() noexcept;
}
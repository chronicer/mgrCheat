#include "cheat.h"
#include "../injector/injector.hpp"
#include "../IniReader.h"
#include "../enums.h"
#include "../KeyBind.h"

/* TODO #
*  Make functions for each cheat instead (since return will cancel the handle cheats instead of if condition  
*/

DWORD cheat::GetBaseAddress(const HANDLE hProcess) noexcept
{
	if (hProcess == NULL)
		return NULL; // No access to the process

	HMODULE lphModule[1024]; // Array that receives the list of module handles
	DWORD lpcbNeeded(NULL); // Output of EnumProcessModules, giving the number of bytes requires to store all modules handles in the lphModule array

	if (!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
		return NULL; // Impossible to read modules

	TCHAR szModName[MAX_PATH];
	if (!GetModuleFileNameEx(hProcess, lphModule[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		return NULL; // Impossible to get module info

	return (DWORD)lphModule[0]; // Module 0 is apparently always the EXE itself, returning its address
}

DWORD base1 = cheat::GetBaseAddress(GetCurrentProcess());
DWORD OneHitKillCaveExit = base1 + 0x68EE3A;
DWORD InfiniteRocketsCaveExit = base1 + 0x5499F9;
DWORD InfiniteGrenadeCaveExit = base1 + 0x54D8D6;
DWORD GroundCheatCaveExit = base1 + 0xE6B464;

GameMenuStatus GameMenuStat = (GameMenuStatus)injector::ReadMemory<unsigned int>(cheat::base + 0x17E9F9C);

void __declspec(naked) OneHitKillCave() noexcept
{
	__asm {
		mov eax, [ecx+0x870]
		sub [ecx+0x870], eax
		jmp OneHitKillCaveExit
	}
}

void __declspec(naked) InfiniteRocketsCave() noexcept
{
	__asm {
		mov [ecx + 0x68], eax
		mov edx, [ecx + 0x60]
		dec edx
		mov [ecx + 0x5C], edx
		jmp InfiniteRocketsCaveExit
	}
}

void __declspec(naked) InfiniteGrenadeCave() noexcept
{
	__asm {
		mov eax, [ecx + 0x58]
		mov [ecx + 0x54], eax
		mov eax, [ecx + 0x54]
		jmp InfiniteGrenadeCaveExit
	}
}

void __declspec(naked) GroundCheatCave()
{
	__asm {
			cmp cheat::groundEnabled, 1
			je enabled
			jmp otherwise

			enabled:
				mov [eax], 2
				jmp GroundCheatCaveExit

			otherwise:
				mov [eax], 0
				jmp GroundCheatCaveExit
		
	}
}

void cheat::MugenZangekiCheat() noexcept
{
	if (infiniteFc)
	{
		unsigned int GameplayFlags = (GameFlags)injector::ReadMemory<unsigned int>(base + 0x17EA090);
		GameplayFlags |= MugenZangeki;
		injector::WriteMemory<unsigned int>(base + 0x17EA090, GameplayFlags);
	}
	else
	{
		unsigned int GameplayFlags = (GameFlags)injector::ReadMemory<unsigned int>(base + 0x17EA090);
		GameplayFlags &= ~MugenZangeki;
		injector::WriteMemory<unsigned int>(base + 0x17EA090, GameplayFlags);
	}
}

void cheat::InfiniteHealthCheat() noexcept
{
	if (infiniteHealth) // its just patches the game, but doesn't write the value into
	{
		injector::MakeNOP(base + 0x787859, 6, true);
		injector::MakeNOP(base + 0x787865, 6, true);
	}
	else
	{
		unsigned char rawBytes1[6] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(base + 0x787859, rawBytes1, 6, true);

		unsigned char rawBytes2[6] = { 0x29, 0x91, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(base + 0x787865, rawBytes2, 6, true);
	}
}

void cheat::OneHitKillCheat() noexcept
{
	if (oneHitKill)
	{
		injector::MakeJMP(base + 0x68EE34, &OneHitKillCave, true); // 29 81 70 08 00 00
		injector::MakeNOP(base + 0x68EE39, 1, true);
	}
	else
	{
		unsigned char rawBytes[6] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(base + 0x68EE34, rawBytes, 6, true);
	}
}

void cheat::InfiniteSubWeaponCheat() noexcept
{
	if (infiniteSubWeapon)
	{
		injector::MakeJMP(base + 0x5499F3, &InfiniteRocketsCave, true);
		injector::MakeNOP(base + 0x5499F8, 1, true);

		injector::MakeJMP(base + 0x54D8D0, &InfiniteGrenadeCave, true);
		injector::MakeNOP(base + 0x54D8D5, 1, true);
	}
	else
	{
		unsigned char rocketArray[6] = { 0x89, 0x41, 0x68, 0x89, 0x51, 0x5C };
		unsigned char grenadeArray[6] = { 0xFF, 0x49, 0x54, 0x8B, 0x41, 0x54 };

		injector::WriteMemoryRaw(base + 0x5499F3, rocketArray, 6, true);
		injector::WriteMemoryRaw(base + 0x54D8D0, grenadeArray, 6, true);
	}
}

void cheat::NoDamageStatCheat() noexcept
{
	unsigned char raw[1] = { 0x01 };
	unsigned char patched[1] = { 0x00 };
	injector::WriteMemoryRaw(base + 0x81B482, noDamageStat ? patched : raw, 1, true);
}

void cheat::StealthCheat() noexcept
{
	unsigned char patched[2] = { 0xEB, 0x19 };
	unsigned char original[2] = { 0x74, 0x19 };
	injector::WriteMemoryRaw(base + 0x849286, stealth ? patched : original, 2, true);
}

void cheat::HeightChangeCheat() noexcept
{
	if (GameMenuStat != InGame || !OnFocus)
		return;

	if (heightChange)
	{
		DWORD player = injector::ReadMemory<DWORD>(base + 0x19C1490);
		if (player && player > 0x4000)
		{
			float height = injector::ReadMemory<float>(player + 0x54);
			if (GetAsyncKeyState(VK_ADD) & 1)
			{
				height += heightRate;
				injector::WriteMemory<float>(player + 0x54, height);
			}
			else if (GetAsyncKeyState(VK_SUBTRACT) & 1)
			{
				height -= heightRate;
				injector::WriteMemory<float>(player + 0x54, height);
			}
		}
	}
}

void cheat::GroundCheat() noexcept
{
	if (groundCheat)
	{
		if (KeyBind::IsKeyPressed(groundCheatHotkey) && GameMenuStat == InGame && OnFocus)
			groundEnabled = !groundEnabled;

		injector::MakeJMP(base + 0xE6B45E, &GroundCheatCave, true);
		injector::MakeNOP(base + 0xE6B463, 1, true);
		injector::MakeNOP(base + 0x4E98CD, 3, true);
	}
	else
	{
		unsigned char rawBytes[6] = { 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00 };
		unsigned char rawBytes1[3] = { 0x89, 0x46, 0x10 };

		injector::WriteMemoryRaw(base + 0xE6B45E, rawBytes, 6, true);
		injector::WriteMemoryRaw(base + 0x4E98CD, rawBytes1, 3, true);
	}
}

void cheat::TemporaryVisorCheat() noexcept
{
	if (KeyBind::IsKeyPressed(temporaryVisorHotkey) && OnFocus)
	{
		unsigned int flags = injector::ReadMemory<unsigned int>(base + 0x17EA094);

		visorSwitch = (flags & 0x40) == 0;

		if (visorSwitch)
			flags |= 0x40;
		else
			flags &= ~0x40;

		injector::WriteMemory<unsigned int>(base + 0x17EA094, flags);
	}
}

void cheat::AutoHPUpCheat() noexcept
{
	if (autoHpUp)
	{
		unsigned int GameplayFlags = (GameFlags)injector::ReadMemory<unsigned int>(base + 0x17EA090);
		GameplayFlags |= AutoHPUp;
		injector::WriteMemory<unsigned int>(base + 0x17EA090, GameplayFlags);
	}
	else
	{
		unsigned int GameplayFlags = (GameFlags)injector::ReadMemory<unsigned int>(base + 0x17EA090);
		GameplayFlags &= ~AutoHPUp;
		injector::WriteMemory<unsigned int>(base + 0x17EA090, GameplayFlags);
	}
}

void cheat::NinjaRunSpeedCheat() noexcept
{
	DWORD player = injector::ReadMemory<DWORD>(base + 0x19C1490);
	if (!player)
		return;

	ninjarunSpeedRate = injector::ReadMemory<float>(player + 0x53E0);
}

void cheat::ZangekiTimeStopCheat() noexcept
{
	DWORD player = injector::ReadMemory<DWORD>(base + 0x19C1490);
	float defaultValue[4] = { 0.8000000119f, 0.400000006f, 0.8000000119f, 0.03999999911f };
	float changedValue[4] = { 1.0f, 0.00010f, 1.0, 0.00010f };
	if (!player)
		return;

	injector::WriteMemory<float>(player + 0x4060, zangekiTimeStop ? changedValue[0] : defaultValue[0]);
	injector::WriteMemory<float>(player + 0x4064, zangekiTimeStop ? changedValue[1] : defaultValue[1]);
	injector::WriteMemory<float>(player + 0x4068, zangekiTimeStop ? changedValue[2] : defaultValue[2]);
	injector::WriteMemory<float>(player + 0x406C, zangekiTimeStop ? changedValue[3] : defaultValue[3]);
}

bool once = false;
void cheat::HandleCheats() noexcept
{
	base = GetBaseAddress(GetCurrentProcess());
	GameMenuStat = (GameMenuStatus)injector::ReadMemory<unsigned int>(base + 0x17E9F9C);
	OnFocus = injector::ReadMemory<bool>(base + 0x19D509C); // we will process hotkey while ONFOCUS and INGAME state
	if (!once)
	{
		LoadConfig();
		once = true;
	}
	// Player
	MugenZangekiCheat();
	InfiniteHealthCheat();
	InfiniteSubWeaponCheat();
	HeightChangeCheat();
	TemporaryVisorCheat();
	AutoHPUpCheat();
	NinjaRunSpeedCheat();
	ZangekiTimeStopCheat();

	// Enemies
	OneHitKillCheat();

	// Battle
	NoDamageStatCheat();
	StealthCheat();
	
	// Entities
	GroundCheat();
}

void cheat::LoadConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	infiniteFc = iniReader.ReadInteger("Player", "InfFuelContainer", 0) == 1;
	infiniteHealth = iniReader.ReadInteger("Player", "InfHealth", 0) == 1;
	infiniteSubWeapon = iniReader.ReadInteger("Player", "InfSubWeapon", 0) == 1;
	heightChange = iniReader.ReadInteger("Player", "HeightChange", 0) == 1;
	heightRate = iniReader.ReadFloat("Player", "HeightRate", 0.0f);
	temporaryVisorHotkey = iniReader.ReadInteger("Player", "VisorHotkey", 80);
	autoHpUp = iniReader.ReadInteger("Player", "AutoHpUp", 0) == 1;
	zangekiTimeStop = iniReader.ReadInteger("Player", "ZangekiTimeStop", 0) == 1;

	groundCheat = iniReader.ReadInteger("Entities", "GroundCheatEnabled", 0) == 1;
	groundCheatHotkey = iniReader.ReadInteger("Entities", "GroundCheatHotkey", 75);

	oneHitKill = iniReader.ReadInteger("Enemies", "OneHitKill", 0) == 1;

	noDamageStat = iniReader.ReadInteger("Battle", "NoDamageStat", 0) == 1;
	stealth = iniReader.ReadInteger("Battle", "Stealth", 0) == 1;
}

void cheat::SaveConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	iniReader.WriteInteger("Player", "InfFuelContainer", infiniteFc);
	iniReader.WriteInteger("Player", "InfHealth", infiniteHealth);
	iniReader.WriteInteger("Player", "InfSubWeapon", infiniteSubWeapon);
	iniReader.WriteInteger("Player", "HeightChange", heightChange);
	iniReader.WriteFloat("Player", "HeightRate", heightRate);
	iniReader.WriteInteger("Player", "VisorHotkey", temporaryVisorHotkey);
	iniReader.WriteInteger("Player", "AutoHpUp", autoHpUp);
	iniReader.WriteInteger("Player", "ZangekiTimeStop", zangekiTimeStop);

	iniReader.WriteInteger("Entities", "GroundCheatEnabled", groundCheat);
	iniReader.WriteInteger("Entities", "GroundCheatHotkey", groundCheatHotkey);

	iniReader.WriteInteger("Enemies", "OneHitKill", oneHitKill);

	iniReader.WriteInteger("Battle", "NoDamageStat", noDamageStat);
	iniReader.WriteInteger("Battle", "Stealth", stealth);
}
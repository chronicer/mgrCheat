#include "cheat.h"
#include "../injector/injector.hpp"

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

void __declspec(naked) cheat::OneHitKillCave() noexcept
{
	__asm {
		mov eax, [ecx+0x870]
		sub [ecx+0x870], eax
		jmp OneHitKillCaveExit
	}
}

void __declspec(naked) cheat::InfiniteRocketsCave() noexcept
{
	__asm {
		mov [ecx + 0x68], eax
		mov edx, [ecx + 0x60]
		dec edx
		mov [ecx + 0x5C], edx
		jmp InfiniteRocketsCaveExit
	}
}

void __declspec(naked) cheat::InfiniteGrenadeCave() noexcept
{
	__asm {
		mov eax, [ecx + 0x58]
		mov [ecx + 0x54], eax
		mov eax, [ecx + 0x54]
		jmp InfiniteGrenadeCaveExit
	}
}

void cheat::HandleCheats() noexcept
{
	DWORD base = GetBaseAddress(GetCurrentProcess());
	GameMenuStatus GameMenuStat = (GameMenuStatus)injector::ReadMemory<unsigned short>(base + 0x17E9F9C);
	bool OnFocus = injector::ReadMemory<bool>(base + 0x19D509C); // we will process hotkey while ONFOCUS and INGAME state
	if (GameMenuStat == InGame && OnFocus)
	{
		if (infiniteFc)
		{
			unsigned int flags = injector::ReadMemory<unsigned int>(base + 0x17EA090);
			flags |= (1 << 1);
			injector::WriteMemory<unsigned int>(base + 0x17EA090, flags);
		}
		else
		{
			unsigned int flags = injector::ReadMemory<unsigned int>(base + 0x17EA090);
			flags &= ~(1 << 1);
			injector::WriteMemory<unsigned int>(base + 0x17EA090, flags);
		}

		if (infiniteHealth)
		{
			injector::MakeNOP(base + 0x787859, 6, true);
			injector::MakeNOP(base + 0x787865, 6, true);
		}
		else
		{
			unsigned char rawBytes1[] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
			injector::WriteMemoryRaw(base + 0x787859, rawBytes1, 6, true);

			unsigned char rawBytes2[] = { 0x29, 0x91, 0x70, 0x08, 0x00, 0x00 };
			injector::WriteMemoryRaw(base + 0x787865, rawBytes2, 6, true);
		}

		if (oneHitKill)
		{
			injector::MakeJMP(base + 0x68EE34, &OneHitKillCave, true); // 29 81 70 08 00 00
			injector::MakeNOP(base + 0x68EE39, 1, true);
		}
		else
		{
			unsigned char rawBytes[] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
			injector::WriteMemoryRaw(base + 0x68EE34, rawBytes, 6, true);
		}

		if (infiniteSubWeapon)
		{
			injector::MakeJMP(base + 0x5499F3, &InfiniteRocketsCave, true);
			injector::MakeNOP(base + 0x5499F8, 1, true);

			injector::MakeJMP(base + 0x54D8D0, &InfiniteGrenadeCave, true);
			injector::MakeNOP(base + 0x54D8D5, 1, true);
		}
		else
		{
			unsigned char rocketArray[] = { 0x89, 0x41, 0x68, 0x89, 0x51, 0x5C };
			unsigned char grenadeArray[] = { 0xFF, 0x49, 0x54, 0x8B, 0x41, 0x54 };

			injector::WriteMemoryRaw(base + 0x5499F3, rocketArray, 6, true);
			injector::WriteMemoryRaw(base + 0x54D8D0, grenadeArray, 6, true);
		}

		if (noDamageStat)
		{
			unsigned char patched[] = { 0xB8, 0x00 };
			injector::WriteMemoryRaw(base + 0x81B481, patched, 2, true);
		}
		else
		{
			unsigned char original[] = { 0xB8, 0x01 };
			injector::WriteMemoryRaw(base + 0x81B481, original, 2, true);
		}

		if (stealth)
		{
			unsigned char patched[] = { 0xEB, 0x19 };
			injector::WriteMemoryRaw(base + 0x849286, patched, 2, true);
		}
		else
		{
			unsigned char original[] = { 0x74, 0x19 };
			injector::WriteMemoryRaw(base + 0x849286, original, 2, true);
		}

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
}
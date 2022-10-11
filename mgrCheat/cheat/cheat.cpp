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

void __declspec(naked) cheat::OneHitKillCave() noexcept
{
	__asm {
		mov eax, [ecx+0x870]
		sub [ecx+0x870], eax
		jmp OneHitKillCaveExit
	}
}

void cheat::HandleCheats() noexcept
{
	base = GetBaseAddress(GetCurrentProcess());
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
}
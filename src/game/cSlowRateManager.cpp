#include "cSlowRateManager.h"

#include <Windows.h>
#include "../cheat/cheat.h"
#include "cSlowRateManager.h"

static DWORD base = cheat::base;
cSlowRateManager& g_pcSlowRateManager = *(cSlowRateManager*)(base + 0x19D9160);
cSlowRateManager* m_cSlowRateManager = &g_pcSlowRateManager;

void cSlowRateManager::SetSlowRate(SlowRateType SlowType, float SlowTime) noexcept
{
	DWORD address = base + 0xA03A70;
	((void(__thiscall*)(cSlowRateManager*, SlowRateType, float))address)(this, SlowType, SlowTime);
}

cSlowRateManager* GetcSlowRateManager() noexcept
{
	DWORD address = base + 0xA03960;
	return ((cSlowRateManager * (__stdcall*)())address)();
}

void cSlowRateManager_SetSlowRate(SlowRateType SlowType, float SlowTime) noexcept
{
	DWORD address = base + 0x532020;
	((void(__cdecl*)(SlowRateType, float))address)(SlowType, SlowTime);
}

void cSlowRateManager_Reset() noexcept
{
	DWORD address = base + 0x11EDC20;
	((void(__cdecl*)())address)();
}

float cSlowRateManager::GetSlowRate(SlowRateType SlowType) noexcept
{
	if (SlowType < 4)
		return this->m_fSlowRate[SlowType].m_fRate;
	else
		return 1.0f;
}

float cSlowRateManager::GetCalculatedSlowRate(SlowRateType SlowType) noexcept
{
	if (SlowType < 4)
		return this->m_fSlowRate[SlowType].m_fCalculatedRate;
	else
		return 1.0f;
}

void cSlowRateManager::ResetSlowRate() noexcept
{
	DWORD address = base + 0xA03B10;
	((void(__thiscall*)(cSlowRateManager*))address)(this);
}
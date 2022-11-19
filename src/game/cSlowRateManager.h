#pragma once

enum SlowRateType : int
{
	GAMEWORLDSPEED = 0x0,
	PLSPEED = 0x1,
	WORLDSLOWTIME = 0x2,
	PARTICLELIFETIME = 0x3,
};

class cSlowRateManager
{
public:
	void* vtable;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	struct
	{
		float m_fRate;
		float m_fDefaultRate;
		float m_fActualRate;
		float m_fCalculatedRate;
	} m_fSlowRate[4];
	float m_nTickTime;
	float field_80;
	float field_84;
	float field_88;
	float field_8C;
	int field_90;

	void SetSlowRate(SlowRateType SlowType, float SlowTime) noexcept;
	float GetSlowRate(SlowRateType SlowType) noexcept;
	float GetCalculatedSlowRate(SlowRateType SlowType) noexcept;
	void ResetSlowRate() noexcept;
};

void cSlowRateManager_Reset() noexcept;
void cSlowRateManager_SetSlowRate(SlowRateType SlowType, float SlowTime) noexcept;
cSlowRateManager* GetcSlowRateManager() noexcept;